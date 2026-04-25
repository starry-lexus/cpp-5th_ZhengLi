import asyncio
import json
import re
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Literal

import flet as ft
import ollama
import pandas as pd
from ollama import AsyncClient, ResponseError
from pydantic import BaseModel, Field, ValidationError, create_model


LANGUAGE_CONFIG = """
1. 如果用户以某种语言提问，则你必须用该语言回答；
2. 如果用户以某几种语言提问，则你必须用内容占比最多的语言回答；
3. 如果用户要求以某种语言回答，则你必须忽略上述要求，然后用该语言回答；
4. 如果用户未指定回答的语言，但用中文提问，则无论其中是否包含其他语言，你都必须用中文回答。
""".strip()

DEFAULT_MODELS = {
    "kimi-k2.6:cloud",
    "qwen3.6:35b",
    "qwen3.6:27b",
    "glm-5.1:cloud",
    "gemma4:31b",
}

TYPE_MAP: dict[str, type] = {
    "str": str,
    "bool": bool,
    "int": int,
    "float": float,
}

CONTEXT_OPTIONS = ["4K", "8K", "16K", "32K", "64K", "128K", "256K"]


class MultiSelectPanel(ft.Container):
    def __init__(self, title: str, on_change=None):
        super().__init__()
        self.title = title
        self.on_change = on_change
        self.options: list[str] = []
        self.selected: set[str] = set()
        self.box = ft.Column(spacing=4)
        self.summary = ft.Text("未选择", size=12, color=ft.Colors.GREY_600)
        self.tile = ft.ExpansionTile(
            title=ft.Text(title, weight=ft.FontWeight.W_600),
            subtitle=self.summary,
            controls=[self.box],
            initially_expanded=False,
        )
        self.border = ft.Container(
            border=ft.border.all(1, ft.Colors.OUTLINE),
            border_radius=8,
            padding=8,
            content=self.tile,
        )
        self.content = self.border

    def set_invalid(self, invalid: bool):
        self.border.border = ft.border.all(1, ft.Colors.RED if invalid else ft.Colors.OUTLINE)

    def set_options(self, options: list[str], keep_selected: bool = True):
        options = list(dict.fromkeys(options))
        self.options = options
        if keep_selected:
            self.selected = {v for v in self.selected if v in self.options}
        else:
            self.selected = set()

        self.box.controls.clear()
        for item in self.options:
            c = ft.Checkbox(
                label=item,
                value=item in self.selected,
                on_change=lambda e, k=item: self._toggle(k, e.control.value),
            )
            self.box.controls.append(c)
        self._refresh_summary()

    def _toggle(self, key: str, checked: bool):
        if checked:
            self.selected.add(key)
        else:
            self.selected.discard(key)
        self._refresh_summary()
        if self.on_change:
            self.on_change()

    def _refresh_summary(self):
        if not self.selected:
            self.summary.value = "未选择"
        elif len(self.selected) <= 4:
            self.summary.value = ", ".join(sorted(self.selected))
        else:
            self.summary.value = f"已选择 {len(self.selected)} 项"
        self.update()


@dataclass
class DynamicField:
    name: str
    type_name: str
    description: str


class ResultFieldRow(ft.Container):
    def __init__(self, on_delete):
        super().__init__()
        self.name_tf = ft.TextField(label="字段名", expand=2, hint_text="如 is_related")
        self.type_dd = ft.Dropdown(
            label="类型",
            options=[ft.dropdown.Option(k) for k in TYPE_MAP.keys()],
            value="str",
            expand=1,
        )
        self.desc_tf = ft.TextField(label="描述", expand=3, hint_text="字段语义")
        self.del_btn = ft.IconButton(icon=ft.Icons.DELETE_OUTLINE, tooltip="删除字段", on_click=lambda _: on_delete(self))
        self.content = ft.Row([self.name_tf, self.type_dd, self.desc_tf, self.del_btn], vertical_alignment=ft.CrossAxisAlignment.CENTER)

    def to_dynamic_field(self) -> DynamicField | None:
        name = (self.name_tf.value or "").strip()
        tname = (self.type_dd.value or "str").strip()
        desc = (self.desc_tf.value or "").strip()
        if not name:
            return None
        if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name):
            return None
        if tname not in TYPE_MAP:
            return None
        return DynamicField(name=name, type_name=tname, description=desc)


class MetadataAnalyzerApp(ft.Container):
    def __init__(self, page: ft.Page):
        super().__init__(expand=True)
        self.page = page
        self.page.title = "元信息分析"
        self.page.scroll = ft.ScrollMode.AUTO
        self.page.theme_mode = ft.ThemeMode.LIGHT

        self.is_analyzing = False
        self.models: set[str] = set(DEFAULT_MODELS)
        self.local_models: set[str] = set()
        self.current_headers: list[str] = []
        self.dynamic_field_error = ""

        self.file_picker = ft.FilePicker(on_result=self._on_file_picked)
        self.pick_target: str | None = None
        self.page.overlay.append(self.file_picker)

        self._build_ui()
        self.refresh_models()
        self.validate_all()

    def _build_ui(self):
        # 模型设置
        self.model_dd = ft.Dropdown(label="选择模型", editable=True, on_change=lambda _: self._on_model_changed(), expand=True)
        self.model_action_btn = ft.IconButton(icon=ft.Icons.CLOUD_DOWNLOAD, tooltip="拉取模型", on_click=lambda _: self.page.run_task(self.pull_or_delete_selected_model))
        self.model_custom_hint = ft.TextField(label="自定义模型名（回车拉取）", on_submit=lambda e: self.page.run_task(self.pull_model_by_name((e.control.value or "").strip())), expand=True)
        self.model_pull_pb = ft.ProgressBar(visible=False)
        self.model_pull_text = ft.Text("", size=12)

        self.think_switch = ft.Switch(label="启用思考", value=False, on_change=lambda _: self.validate_all())
        self.think_depth = ft.Dropdown(
            label="思考深度",
            options=[ft.dropdown.Option(k) for k in ["low", "medium", "high"]],
            value="medium",
            disabled=True,
            width=180,
        )
        self.show_thinking_switch = ft.Switch(label="显示思考信息", value=False)

        self.temp_tf = ft.TextField(label="温度", value="0.0", width=160, on_change=lambda _: self.validate_all())
        self.concurrent_slider = ft.Slider(min=1, max=10, divisions=9, value=1, label="{value}", width=240)
        self.context_dd = ft.Dropdown(label="上下文长度", options=[ft.dropdown.Option(k) for k in CONTEXT_OPTIONS], value="256K", width=180)

        # 路径设置
        self.input_tf = ft.TextField(label="输入表格", expand=True, on_change=lambda _: self.on_input_path_change())
        self.input_browse = ft.ElevatedButton("浏览", on_click=lambda _: self.pick_file("input"))
        self.input_err = ft.Text("", color=ft.Colors.RED_600, size=12)

        self.read_fields = MultiSelectPanel("读取字段", on_change=self._on_read_fields_change)
        self.analyze_fields = MultiSelectPanel("分析字段", on_change=self._on_analyze_fields_change)

        self.output_tf = ft.TextField(label="输出表格", expand=True, on_change=lambda _: self.validate_all())
        self.output_browse = ft.ElevatedButton("浏览", on_click=lambda _: self.pick_file("output"))
        self.output_err = ft.Text("", color=ft.Colors.RED_600, size=12)
        self.output_hint = ft.Text("", color=ft.Colors.BLUE_700, size=12)

        self.extra_fields = MultiSelectPanel("额外字段")

        # easyScholar
        self.easyscholar_key = ft.TextField(label="EasyScholar 密钥", password=True, can_reveal_password=True)
        self.journal_col_dd = ft.Dropdown(label="期刊名称列", options=[], width=260)

        # 提示词
        self.system_prompt = ft.TextField(label="系统提示词", value=LANGUAGE_CONFIG, multiline=True, min_lines=4, max_lines=10)
        self.user_prompt = ft.TextField(label="用户提示词模板", multiline=True, min_lines=8, max_lines=20, hint_text="可使用 {field_name} 占位符")

        # 页面1：字段配置
        self.dynamic_fields_col = ft.Column(spacing=8)
        self.dynamic_field_err_text = ft.Text("", color=ft.Colors.RED_600, size=12)
        self.add_field_btn = ft.OutlinedButton("添加字段", icon=ft.Icons.ADD, on_click=lambda _: self.add_dynamic_field_row())
        self.page1 = ft.Column(
            controls=[
                ft.Row([ft.Text("页面1：字段定义", size=20, weight=ft.FontWeight.BOLD), self.add_field_btn]),
                self.dynamic_fields_col,
                self.dynamic_field_err_text,
            ],
            spacing=10,
            scroll=ft.ScrollMode.AUTO,
            expand=True,
        )

        # 页面2：监控
        self.token_text = ft.Text("Token 消费：prompt=0, completion=0")
        self.thinking_text = ft.Text("", selectable=True)
        self.model_output_text = ft.Text("", selectable=True)
        self.analysis_pb = ft.ProgressBar(value=0)
        self.analysis_status = ft.Text("未开始")

        self.page2 = ft.Column(
            controls=[
                ft.Text("页面2：分析监控", size=20, weight=ft.FontWeight.BOLD),
                self.token_text,
                ft.Text("思考信息"),
                ft.Container(border=ft.border.all(1, ft.Colors.OUTLINE), border_radius=6, padding=8, content=self.thinking_text, height=120),
                ft.Text("模型输出"),
                ft.Container(border=ft.border.all(1, ft.Colors.OUTLINE), border_radius=6, padding=8, content=self.model_output_text, height=160),
                self.analysis_pb,
                self.analysis_status,
            ],
            spacing=8,
            visible=False,
            expand=True,
        )

        # 按钮
        self.reset_btn = ft.OutlinedButton("重置", on_click=lambda _: self.reset_all())
        self.analyze_btn = ft.FilledButton("分析", on_click=lambda _: self.page.run_task(self.start_analysis), disabled=True)

        left = ft.Container(
            expand=2,
            padding=12,
            content=ft.Column(
                controls=[
                    ft.Text("模型设置", size=18, weight=ft.FontWeight.BOLD),
                    ft.Row([self.model_dd, self.model_action_btn]),
                    self.model_custom_hint,
                    self.model_pull_pb,
                    self.model_pull_text,
                    ft.Row([self.think_switch, self.think_depth, self.show_thinking_switch]),
                    ft.Row([self.temp_tf, ft.Text("最大并发量"), self.concurrent_slider, self.context_dd], wrap=True),
                    ft.Divider(),
                    ft.Text("路径设置", size=18, weight=ft.FontWeight.BOLD),
                    ft.Row([self.input_tf, self.input_browse]),
                    self.input_err,
                    self.read_fields,
                    self.analyze_fields,
                    ft.Row([self.output_tf, self.output_browse]),
                    self.output_err,
                    self.output_hint,
                    self.extra_fields,
                    ft.Divider(),
                    ft.Text("EasyScholar 配置", size=18, weight=ft.FontWeight.BOLD),
                    self.easyscholar_key,
                    self.journal_col_dd,
                    ft.Divider(),
                    ft.Text("提示词设置", size=18, weight=ft.FontWeight.BOLD),
                    self.system_prompt,
                    self.user_prompt,
                    ft.Row([self.reset_btn, self.analyze_btn], alignment=ft.MainAxisAlignment.END),
                ],
                spacing=8,
                scroll=ft.ScrollMode.AUTO,
            ),
        )

        right = ft.Container(
            expand=3,
            padding=12,
            content=ft.Column([self.page1, self.page2], expand=True),
        )

        self.add_dynamic_field_row()
        self.content = ft.Row([left, ft.VerticalDivider(width=1), right], expand=True)

    def add_dynamic_field_row(self):
        row = ResultFieldRow(on_delete=self.remove_dynamic_field_row)
        self.dynamic_fields_col.controls.append(row)
        self.update()

    def remove_dynamic_field_row(self, row: ResultFieldRow):
        if row in self.dynamic_fields_col.controls:
            self.dynamic_fields_col.controls.remove(row)
            self.update()

    def reset_all(self):
        self.is_analyzing = False
        self.model_custom_hint.value = ""
        self.model_pull_pb.visible = False
        self.model_pull_text.value = ""
        self.think_switch.value = False
        self.show_thinking_switch.value = False
        self.think_depth.value = "medium"
        self.temp_tf.value = "0.0"
        self.concurrent_slider.value = 1
        self.context_dd.value = "256K"
        self.input_tf.value = ""
        self.output_tf.value = ""
        self.current_headers = []
        self.input_err.value = ""
        self.output_err.value = ""
        self.output_hint.value = ""
        self.read_fields.set_options([], keep_selected=False)
        self.analyze_fields.set_options([], keep_selected=False)
        self.extra_fields.set_options([], keep_selected=False)
        self.easyscholar_key.value = ""
        self.journal_col_dd.options = []
        self.journal_col_dd.value = None
        self.system_prompt.value = LANGUAGE_CONFIG
        self.user_prompt.value = ""
        self.dynamic_fields_col.controls.clear()
        self.add_dynamic_field_row()
        self.page1.visible = True
        self.page2.visible = False
        self.analysis_pb.value = 0
        self.analysis_status.value = "未开始"
        self.token_text.value = "Token 消费：prompt=0, completion=0"
        self.thinking_text.value = ""
        self.model_output_text.value = ""
        self.refresh_models()
        self.validate_all()
        self.update()

    def refresh_models(self):
        try:
            listed = ollama.list().models
            self.local_models = {m.model for m in listed}
        except Exception:
            self.local_models = set()

        self.models = set(DEFAULT_MODELS) | self.local_models
        ordered = sorted(self.models)
        self.model_dd.options = [ft.dropdown.Option(m) for m in ordered]
        if ordered and (self.model_dd.value not in self.models):
            self.model_dd.value = ordered[0]
        self._refresh_model_action_icon()
        self._on_model_changed()
        self.update()

    def _refresh_model_action_icon(self):
        m = (self.model_dd.value or "").strip()
        if not m:
            self.model_action_btn.icon = ft.Icons.CLOUD_DOWNLOAD
            self.model_action_btn.tooltip = "拉取模型"
            return
        if m in self.local_models:
            self.model_action_btn.icon = ft.Icons.DELETE_OUTLINE
            self.model_action_btn.tooltip = "删除本地模型"
        elif m.endswith(":cloud"):
            self.model_action_btn.icon = ft.Icons.CLOUD_DOWNLOAD
            self.model_action_btn.tooltip = "使用云端模型"
        else:
            self.model_action_btn.icon = ft.Icons.DOWNLOAD
            self.model_action_btn.tooltip = "下载模型"

    def _on_model_changed(self):
        self._refresh_model_action_icon()
        model = (self.model_dd.value or "").strip()
        think_supported = False
        if model:
            try:
                info = ollama.show(model)
                caps = info.get("capabilities", []) if isinstance(info, dict) else []
                think_supported = "thinking" in caps
            except Exception:
                think_supported = False
        self.think_switch.disabled = not think_supported
        if not think_supported:
            self.think_switch.value = False
        self.think_depth.disabled = not self.think_switch.value
        self.validate_all()
        self.update()

    async def pull_or_delete_selected_model(self):
        model = (self.model_dd.value or "").strip()
        if not model:
            return
        if model in self.local_models:
            try:
                ollama.delete(model)
                self.model_pull_text.value = f"已删除：{model}"
            except Exception as e:
                self.model_pull_text.value = f"删除失败：{e}"
            self.refresh_models()
            return
        await self.pull_model_by_name(model)

    async def pull_model_by_name(self, model_name: str):
        if not model_name:
            return
        self.model_pull_pb.visible = True
        self.model_pull_pb.value = None
        self.model_pull_text.value = f"正在拉取模型：{model_name}"
        self.update()

        try:
            iterator = ollama.pull(model_name, stream=True)
            while True:
                part = await asyncio.wait_for(asyncio.to_thread(next, iterator, None), timeout=60)
                if part is None:
                    break
                total = part.get("total") or 0
                completed = part.get("completed") or 0
                status = part.get("status") or ""
                prog = min(1.0, completed / total) if total else None
                self.model_pull_pb.value = prog
                self.model_pull_text.value = f"拉取中 {f'{prog:.0%}' if isinstance(prog, float) else ''} {status}".strip()
                self.update()
        except asyncio.TimeoutError:
            self.model_pull_pb.visible = False
            self.model_pull_text.value = "拉取超时，请检查网络连接后重试。"
            self.update()
            return
        except Exception as e:
            self.model_pull_pb.visible = False
            self.model_pull_text.value = f"无效的模型名，请从 https://ollama.com/search 复制模型名。错误：{e}"
            self.update()
            return

        self.model_pull_pb.visible = False
        self.model_pull_text.value = f"模型拉取成功：{model_name}"
        self.refresh_models()
        self.model_dd.value = model_name
        self.update()

    def pick_file(self, target: str):
        self.pick_target = target
        self.file_picker.pick_files(allow_multiple=False)

    def _on_file_picked(self, e: ft.FilePickerResultEvent):
        if not e.files:
            return
        p = e.files[0].path
        if self.pick_target == "input":
            self.input_tf.value = p
            self.on_input_path_change()
        elif self.pick_target == "output":
            self.output_tf.value = p
            self.validate_all()
            self.update()

    def on_input_path_change(self):
        p = Path((self.input_tf.value or "").strip())
        ok, msg = self._validate_input_path(p)
        if ok:
            self.input_err.value = ""
            self.input_tf.border_color = None
            self.load_headers_from_input(p)
        else:
            self.input_err.value = msg
            self.input_tf.border_color = ft.Colors.RED
            self.current_headers = []
            self.read_fields.set_options([])
            self.analyze_fields.set_options([])
            self.extra_fields.set_options([])
            self.journal_col_dd.options = []
        self.validate_all()
        self.update()

    def _validate_input_path(self, p: Path) -> tuple[bool, str]:
        if not p:
            return False, "请输入输入文件路径"
        if p.suffix.lower() not in {".csv", ".xls", ".xlsx"}:
            return False, "仅支持 .csv / .xls / .xlsx"
        if not p.exists():
            return False, "输入文件不存在"
        return True, ""

    def _validate_output_path(self, p: Path) -> tuple[bool, str]:
        if not str(p).strip():
            return False, "请输入输出文件路径"
        if p.suffix.lower() not in {".csv", ".xls", ".xlsx"}:
            return False, "输出仅支持 .csv / .xls / .xlsx"
        parent = p.parent
        if parent and not parent.exists():
            return False, "输出目录不存在"
        return True, ""

    def load_headers_from_input(self, p: Path):
        try:
            if p.suffix.lower() == ".csv":
                headers = pd.read_csv(p, nrows=0).columns.tolist()
            else:
                headers = pd.read_excel(p, nrows=0).columns.tolist()
            headers = [str(h) for h in headers if str(h).strip()]
            self.current_headers = headers
            self.read_fields.set_options(headers, keep_selected=False)
            self.analyze_fields.set_options(headers, keep_selected=False)
            self.extra_fields.set_options([], keep_selected=False)
            self.journal_col_dd.options = [ft.dropdown.Option(h) for h in headers]
            if headers and not self.journal_col_dd.value:
                self.journal_col_dd.value = headers[0]
        except Exception as e:
            self.input_err.value = f"读取标题行失败：{e}"
            self.input_tf.border_color = ft.Colors.RED

    def _on_read_fields_change(self):
        selected = sorted(self.read_fields.selected)
        self.analyze_fields.set_options(selected, keep_selected=True)
        self._on_analyze_fields_change()
        self.validate_all()
        self.update()

    def _on_analyze_fields_change(self):
        extras = sorted(set(self.read_fields.selected) - set(self.analyze_fields.selected))
        self.extra_fields.set_options(extras, keep_selected=True)
        self.validate_all()
        self.update()

    def _parse_temperature(self) -> float | None:
        try:
            v = float((self.temp_tf.value or "").strip())
            if 0.0 <= v <= 2.0:
                self.temp_tf.border_color = None
                return v
        except Exception:
            pass
        self.temp_tf.border_color = ft.Colors.RED
        return None

    def _get_dynamic_result_model(self) -> type[BaseModel] | None:
        fields: dict[str, tuple[type, Any]] = {}
        used_names = set()
        self.dynamic_field_error = ""
        for row in self.dynamic_fields_col.controls:
            if not isinstance(row, ResultFieldRow):
                continue
            f = row.to_dynamic_field()
            if not f:
                self.dynamic_field_error = "字段名必须为合法标识符（字母/下划线开头，仅含字母数字下划线）。"
                continue
            if f.name in used_names:
                self.dynamic_field_error = f"字段名重复：{f.name}"
                return None
            used_names.add(f.name)
            py_t = TYPE_MAP[f.type_name]
            fields[f.name] = (py_t, Field(..., description=f.description or f.name))
        if not fields:
            if not self.dynamic_field_error:
                self.dynamic_field_error = "请至少添加一个有效输出字段。"
            return None
        return create_model("Result", **fields)

    def validate_all(self):
        self.think_depth.disabled = not self.think_switch.value

        in_ok, in_msg = self._validate_input_path(Path((self.input_tf.value or "").strip()))
        out_ok, out_msg = self._validate_output_path(Path((self.output_tf.value or "").strip()))
        temp_ok = self._parse_temperature() is not None

        self.input_err.value = "" if in_ok else in_msg
        self.input_tf.border_color = None if in_ok else ft.Colors.RED
        self.output_err.value = "" if out_ok else out_msg
        self.output_tf.border_color = None if out_ok else ft.Colors.RED
        out_suffix = Path((self.output_tf.value or "").strip()).suffix.lower()
        if out_ok and out_suffix in {".xls", ".xlsx"}:
            self.output_hint.value = "分析过程中为保证崩溃可恢复的追加写入, Excel 目标不会生成 .xlsx, 而是直接产出同名 .csv 文件。"
        else:
            self.output_hint.value = ""

        self.read_fields.set_invalid(len(self.read_fields.selected) == 0)
        self.analyze_fields.set_invalid(len(self.analyze_fields.selected) == 0)

        has_schema = self._get_dynamic_result_model() is not None
        self.dynamic_field_err_text.value = self.dynamic_field_error
        system_ok = bool((self.system_prompt.value or "").strip())
        user_ok = bool((self.user_prompt.value or "").strip())
        model_ok = bool((self.model_dd.value or "").strip())

        ready = all([
            in_ok,
            out_ok,
            temp_ok,
            model_ok,
            system_ok,
            user_ok,
            len(self.read_fields.selected) > 0,
            len(self.analyze_fields.selected) > 0,
            has_schema,
            not self.is_analyzing,
        ])
        self.analyze_btn.disabled = not ready
        self.update()

    async def _chat_structured(
        self,
        client: AsyncClient,
        model: str,
        think: Literal["low", "medium", "high"] | bool,
        messages: list[dict[str, str]],
        result_model: type[BaseModel],
        temperature: float,
        num_ctx: int,
    ) -> tuple[BaseModel | None, dict[str, Any]]:
        try:
            response = await client.chat(
                model=model,
                think=think,
                messages=messages,
                format=result_model.model_json_schema(),
                options={"temperature": temperature, "num_ctx": num_ctx},
            )
            raw = (response.message.content or "").strip()
            meta = {
                "prompt_eval_count": getattr(response, "prompt_eval_count", None) or 0,
                "eval_count": getattr(response, "eval_count", None) or 0,
                "thinking": getattr(response.message, "thinking", "") if hasattr(response, "message") else "",
                "raw": raw,
            }
            try:
                return result_model.model_validate_json(raw), meta
            except ValidationError:
                pass
            raw2 = re.sub(r"^```(?:json)?\s*|\s*```$", "", raw, flags=re.IGNORECASE | re.DOTALL).strip()
            try:
                data = json.loads(raw2)
                return result_model.model_validate(data), meta
            except Exception as pe:
                return None, {**meta, "error": f"解析结构化输出失败：{pe}"}
        except ResponseError as e:
            return None, {"error": f"ResponseError: {e}"}
        except Exception as e:
            return None, {"error": f"UnexpectedError: {e}"}

    async def start_analysis(self):
        self.validate_all()
        if self.analyze_btn.disabled:
            return

        self.is_analyzing = True
        self.validate_all()
        self.page1.visible = False
        self.page2.visible = True
        self.analysis_status.value = "准备中..."
        self.analysis_pb.value = 0
        self.token_text.value = "Token 消费：prompt=0, completion=0"
        self.thinking_text.value = ""
        self.model_output_text.value = ""
        self.update()

        input_path = Path((self.input_tf.value or "").strip())
        output_path = Path((self.output_tf.value or "").strip())
        read_cols = sorted(self.read_fields.selected)
        analyze_cols = sorted(self.analyze_fields.selected)
        extra_cols = sorted(self.extra_fields.selected)
        model = (self.model_dd.value or "").strip()
        temperature = float(self.temp_tf.value)
        max_concurrent = int(self.concurrent_slider.value)
        think: Literal["low", "medium", "high"] | bool = self.think_depth.value if self.think_switch.value else False

        ctx_value = self.context_dd.value or "256K"
        num_ctx = int(ctx_value.replace("K", "")) * 1024

        result_model = self._get_dynamic_result_model()
        if result_model is None:
            self.analysis_status.value = "字段定义无效"
            self.is_analyzing = False
            self.validate_all()
            self.update()
            return

        try:
            if input_path.suffix.lower() == ".csv":
                df = pd.read_csv(input_path, usecols=read_cols)
            else:
                df = pd.read_excel(input_path, usecols=read_cols)
        except Exception as e:
            self.analysis_status.value = f"读取输入失败：{e}"
            self.is_analyzing = False
            self.validate_all()
            self.update()
            return

        sem = asyncio.Semaphore(max_concurrent)
        lock = asyncio.Lock()
        client = AsyncClient()

        prompt_template = self.user_prompt.value
        system_prompt = self.system_prompt.value

        total = len(df)
        done = 0
        prompt_tokens = 0
        completion_tokens = 0
        header_written = output_path.exists() and output_path.stat().st_size > 0

        async def worker(row_values: tuple[Any, ...]):
            nonlocal done, prompt_tokens, completion_tokens, header_written
            async with sem:
                row_map = dict(zip(read_cols, row_values))
                row_dict = {k: row_map.get(k, "") for k in analyze_cols}
                safe_map = {k: ("" if pd.isna(v) else str(v)) for k, v in row_dict.items()}
                try:
                    user_content = prompt_template.format(**safe_map)
                except Exception:
                    user_content = prompt_template + "\n" + "\n".join([f"{k}: {safe_map[k]}" for k in analyze_cols])

                messages = [
                    {"role": "system", "content": system_prompt},
                    {"role": "user", "content": user_content},
                ]

                result, meta = await self._chat_structured(
                    client=client,
                    model=model,
                    think=think,
                    messages=messages,
                    result_model=result_model,
                    temperature=temperature,
                    num_ctx=num_ctx,
                )

                async with lock:
                    done += 1
                    self.analysis_pb.value = done / total if total else 1
                    self.analysis_status.value = f"分析中 {done}/{total}"

                    if result is not None:
                        payload = result.model_dump()
                    else:
                        payload = {name: None for name in result_model.model_fields.keys()}

                    out_obj = {**payload}
                    for c in extra_cols:
                        out_obj[c] = row_map.get(c, None)

                    out_df = pd.DataFrame([out_obj])
                    header_needed = not header_written
                    if output_path.suffix.lower() == ".csv":
                        out_df.to_csv(output_path, mode="a", index=False, header=header_needed, encoding="utf-8-sig")
                    else:
                        # 为保证崩溃恢复能力，xls/xlsx 目标路径采用同名 csv 追加写入
                        fallback = output_path.with_suffix(".csv")
                        out_df.to_csv(fallback, mode="a", index=False, header=header_needed, encoding="utf-8-sig")
                    header_written = True

                    prompt_tokens += meta.get("prompt_eval_count", 0)
                    completion_tokens += meta.get("eval_count", 0)
                    self.token_text.value = f"Token 消费：prompt={prompt_tokens}, completion={completion_tokens}"

                    if self.show_thinking_switch.value:
                        think_txt = str(meta.get("thinking", "") or "")
                        if think_txt:
                            self.thinking_text.value = think_txt
                    self.model_output_text.value = str(meta.get("raw", "") or meta.get("error", ""))
                    self.update()

        active_tasks: set[asyncio.Task] = set()
        window = max(max_concurrent * 4, 8)
        for row in df.itertuples(index=False, name=None):
            active_tasks.add(asyncio.create_task(worker(row)))
            if len(active_tasks) >= window:
                _, pending_tasks = await asyncio.wait(active_tasks, return_when=asyncio.FIRST_COMPLETED)
                active_tasks = set(pending_tasks)

        if active_tasks:
            await asyncio.gather(*active_tasks, return_exceptions=True)

        self.analysis_status.value = "分析完成"
        self.is_analyzing = False
        self.validate_all()
        self.update()


def main(page: ft.Page):
    app = MetadataAnalyzerApp(page)
    page.add(app)


if __name__ == "__main__":
    ft.app(target=main)

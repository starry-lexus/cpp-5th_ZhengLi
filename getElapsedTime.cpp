/**计算一段代码的运行时间
 * 具体测试代码及各种方法的优缺点和注意事项等见main函数部分,这里只给出一些结论性的建议
 * 1.计算时间时,为避免数据抖动,建议直接多次执行exe文件,而不是每次用IDE的"运行"指令来输出时间差,这样可以免受IDE的"生成"
 * 或"构建"过程(热启动)的影响;
 * 2.在Windows下,获取的时间精度一般最多只能达到微秒级;如果你的程序要求获得的时间精度为纳秒级,建议使用Linux系统;如果要求
 * 精度为毫秒级,那么clock函数无疑是最佳选择,它的返回值相当稳定
 * 3.尽管微软极力推荐使用QueryPerformanceCounter和QueryPerformanceFrequency来获取高精度时间,然而从测试结果来看,
 * 它的返回值是4种高精度时间函数中最不稳定的,相对而言chrono和timespec就比较稳定
 *@author 岛中客、眼中你
 *@date 2023/2/6
 */
#include <iostream>
#include <ctime>
#include <windows.h>
#include <chrono>

int JosephProblem(int n, int m) {
    int survivor = 0;
    for (int i = 1; i <= n; i++)
        survivor = (survivor + m) % i;
    return survivor + 1;
}

int main() {
    /*方法一: clock()
     * 头文件: time.h
     * 返回值: 未加工的程序启动时开始经过的处理器时间
     * 返回类型: time_t
     * 精度: 毫秒(Windows)或微秒(兼容POSIX的系统)
     * CLOCKS_PER_SEC: 宏定义,在window系统中是1000,在兼容POSIX的系统上是1000000,用于将时间差转换为秒
     * 注意: clock()返回的值可能会发生回卷现象,但在64位Windows系统中,gcc对time_t的定义为long long int,
     * CLOCKS_PER_SEC为1000,则它将在约9223372036854776秒(约292269102.59年)发生回卷,我们可以认为回卷
     * 现象不可能发生
     */
    time_t start, end;
    start = clock();
    std::cout << JosephProblem(100000, 5) << std::endl;
    end = clock();
    std::cout << "Elapsed time: " << (end - start) << "ms" << std::endl;

    /*方法二: GetTickCount()或timeGetTime()
     * 头文件: windows.h
     * 返回值: 自系统启动以来已用过的毫秒数
     * 返回类型: DWORD
     * 精度: 10 ~ 18ms
     * 注意: ①如果系统连续运行49.71天(2^32 - 1ms),时间将变为0,若要避免此问题,则请使用GetTickCount64函数,
     * 或者在使用前检查溢出条件;②如果有一天你用到OpenCV,那么就会发现它里面有一个getTickCount函数,不要搞混;
     * ③当程序运行时间低于18ms时,用此函数计算的时间差将没有意义,另外,即使系统连续运行时间没有超过49.71天,计算
     * 出的时间差仍然有可能是0,因为此函数是每隔10 ~ 18ms发送一次,并不是实时发送
     */
    DWORD start1, end1;
    start1 = GetTickCount();
    std::cout << JosephProblem(100000, 5) << std::endl;
    end1 = GetTickCount();
    std::cout << "Elapsed time: " << (end1 - start1) << "ms" << std::endl;
    SYSTEMTIME st;
    GetSystemTime(&st);

    /*方法三: time()
     * 头文件: time.h
     * 返回值: 纪元开始经过的当前系统日历时间
     * 返回类型: time_t
     * 精度: 秒
     */
    start = 0, end = 0;
    time(&start);
    JosephProblem(100000, 5);
    time(&end);
    std::cout << "Elapsed time: " << (end - start) << "s" << std::endl;

    /*方法四: QueryPerformanceCounter()
     * 头文件: windows.h/profileapi.h
     * 返回类型: LARGE_INTEGER结构体
     * 精度: 微秒(与平台计数器的精度有关)
     * QueryPerformanceFrequency(): 获取计数器频率,若其为10000000,则其精度可达到0.1微秒
     * QueryPerformanceCounter():   节拍计数器
     */
    LARGE_INTEGER t0, t1, f;
    QueryPerformanceFrequency(&f);          //暂存计数器频率
    QueryPerformanceCounter(&t0);     //开始计时
    std::cout << JosephProblem(100000, 5) << std::endl;
    QueryPerformanceCounter(&t1);     //结束计时
    double time = static_cast<double>((t1.QuadPart - t0.QuadPart) * 1000000) / static_cast<double>(f.QuadPart);
    std::cout << "Elapsed time(QPC): " << time << "us" << std::endl;

    /*方法五: 时间结构体timeval
     * 定义:
    struct timeval{
	    long tv_sec;
	    long tv_usec;
    };
     * 头文件: time.h
     * 精度: 微秒
     * 相关函数原型: int gettimeofday (struct timeval *p, struct timezone *z)
     */
    struct timeval tv{0, 0};
    mingw_gettimeofday(&tv, nullptr);
    long start4 = tv.tv_sec * 1000000 + tv.tv_usec;
    std::cout << JosephProblem(100000, 5) << std::endl;
    mingw_gettimeofday(&tv, nullptr);
    long end4 = tv.tv_sec * 1000000 + tv.tv_usec;
    std::cout << "Elapsed time(timeval): " << (end4 - start4) << "us\n";

    /*方法六: 结构体steady_clock
     * 头文件: <chrono>
     * 精度: 纳秒
     * 相关类介绍:
     * steady_clock是决不会调整的单调时钟
     * template<class Rep,class Period = std::ratio<1>>
     * class duration;类模板: 表示时间间隔,它由 Rep 类型的计次数和计次周期组成，其中计次周期是一个编译期有理数常量，
     * 表示从一个计次到下一个的秒数。存储于 duration 的数据仅有 Rep 类型的计次数。若 Rep 是浮点数，则duration
     * 能表示小数的计次数。 Period 被包含为时长类型的一部分，且只在不同时长间转换时使用。
     * std::ratio: 计次周期,即每秒的次数,例如std::ratio<1, 1000000>表示1秒1000000次
     * 建议: windows平台获取的纳秒不准确,最好只用到微秒级; Linux和Mac可以较为精确地获取到纳秒
     */
    auto start2 = std::chrono::steady_clock::now();
    std::cout << JosephProblem(100000, 5) << std::endl;
    auto end2 = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration<double, std::ratio<1, 1000000>>(end2 - start2);
    std::cout << "Elapsed time(chrono): " << elapsed.count() << "us\n";

    /*方法七: 时间结构体timespec
     * 说明:①windows系统并不符合POSIX标准,因而按道理是用不了clock_gettime的,然而gcc/g++却提供了这一函数;如果在连接
     * 阶段显示"undefined reference to 'clock_gettime'",那就说明你下载的MinGW没有包含运行时库,如果你使用的IDE是
     * CLion,将工具链中的工具集改为"捆绑的MinGW"即可,你也可以从 https://winlibs.com/ 处下载包含UCRT的最新版MinGW;
     * ②如果你使用的是Visual Studio 2022(MSVC),则可用符合C11标准的timespec_get函数代替;③如果你使用的是其他编译器,
     * 请仔细阅读使用手册
     * 头文件: time.h
     * 返回类型: int
     * 相关函数原型:
     * 1.extern int clock_gettime(clockid_t __clock_id, struct timespec *__tp) __THROW;
     *   __clock_id: 时钟类型,POSIX标准定义了下面四种基本类型,Linux系统有扩充类型
     *   CLOCK_REALTIME: 系统实时时间,从UTC1970-1-1 0:0:0开始计时
     *   CLOCK_MONOTONIC: 从系统启动这一刻开始计时,不受系统时间影响
     *   CLOCK_PROCESS_CPUTIME_ID: 从本进程到当前代码,CPU所花费的时间.不是进程开始到当前代码的时间
     *   CLOCK_THREAD_CPUTIME_ID: 从本线程到当前代码,CPU所花费的时间.不是线程开始到当前代码的时间
     * 2.int timespec_get(struct timespec *ts, int base);
     *   #define TIME_UTC  //implementation-defined
     * 精度: 纳秒
     * 注意: ①采用win32线程模式的gcc仅提供了timespec_get函数,而采用POSIX线程模式的gccc提供了lock_gettime和
     * timespec_get函数;这两个文件解压缩后大小分别为605MB左右和1GB左右②msvc提供的是timespec_get函数;③但是一方面Window系统不能很好地支持获取纳秒级时间(数
     * 据抖动太大),另一方面,获取的时间精度受CPU或硬件计数器影响,而且这个结构体中保存的时间其实是模拟结果(Windows)
     * .这就导致获取的时间没有什么参考价值或者参考价值不大.
     */
    struct timespec start3{0, 0}, end3{0, 0};
    timespec_get(&start3, TIME_UTC);        //建议使用C11标准的timespec_get函数
    //clock_gettime(CLOCK_MONOTONIC, &start3);
    std::cout << JosephProblem(100000, 5) << std::endl;
    //clock_gettime(CLOCK_MONOTONIC, &end3);
    timespec_get(&end3, TIME_UTC);
    time_t duration = (end3.tv_sec - start3.tv_sec) * 1000000000 + (end3.tv_nsec - start3.tv_nsec);
    std::cout << "Elapsed time(timespec): " << duration << "ns\n";

    /*方法八: x86_x64指令rdtsc/rdtscp
     * 使用方法见 https://learn.microsoft.com/zh-cn/cpp/intrinsics/rdtsc?view=msvc-170&viewFallbackFrom=vs-2019
     * 指令含义: read TSC(Time Stamp Counter),即读取保存在EDX:EAX的64位时间戳
     * 头文件: intrin.h
     * 函数原型: uint64_t __rdtsc();  当然你也可以通过内嵌汇编代码实现此函数功能
     * 精度: 纳秒(现代CPU主频基本都能达到GHZ)
     * 缺陷:
     * ①CPU的频率不恒定: ①如今的台式或笔记本电脑均支持电源管理,这就导致在节能、均衡和性能模式下CPU的频率均不同;②如果你的电脑支持超频,
     * 那CPU频率会有所增加;③是否插电源和后台任务数量也会影响CPU的实际频率;④在多核处理器中,程序不一定每次都在一个核心上工作,从核心启动
     * 到达到稳定频率,这期间的增量不一定恒定,尽管奔腾及以上处理器支持恒定增量,且你可用汇编或机器指令将此程序设定在一个核心上工作,但这样得到
     * 的是是虚假的,也不支持跨平台.
     * ②TSC的值不连续: 由于无法保证多核处理器的TSC是同步刷新的,所以得到的TSC抖动很厉害(数据浮动大)
     * ③rtsc指令乱序执行: 这个问题很好解决,只需改用rtscp指令、插入序列化指令CPUID 或 memory barrier
     * ④内核TSC实时速率不好获取: 请前往Stack Overflow等论坛搜索获取方法,但基本都是基于Linux
     * ⑤输出结果的前摇较长
     * ⑥实际结果也与编译器有关: 用CLion + gcc 12.2.0得到的结果变化幅度特别大,但用Visual Studio 2022自带的MSVC编译器得到的结果却
     * 比较稳定
     * 其他缺陷或建议见
     * https://learn.microsoft.com/zh-cn/windows/win32/dxtecharts/game-timing-and-multicore-processors
     * https://learn.microsoft.com/zh-cn/windows/win32/sysinfo/acquiring-high-resolution-time-stamps
     * 综上所述,不建议使用rtsc/rtscp指令计算程序运行时间
     */
    return 0;
}
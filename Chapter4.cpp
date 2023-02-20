/**@brief 在类的设计中,本人有时会设计一些题目没有明确要求的属性或函数,仅作参考,你可根据自己的理解删除或增加
 * @author 岛中客、眼中你
 * @date 2023/1/4
 */
#include <iostream>
#include <string>
#include <utility>

#define PI 3.14159265359
using namespace std;

class Dog {
public:
    Dog(string owner, string breed, unsigned int age = 0, double weight = 0.2) :
            owner(std::move(owner)), breed(std::move(breed)), age(age), weight(weight) {}

    void show() {
        cout << owner << ", your dog is a/an " << breed << ". It is " << age << " year(s) old " <<
             "and weights " << weight << " kg." << endl;
    }

    string getOwner() { return owner; }

    string getBreed() { return breed; }

    unsigned int getAge() const { return age; }

    double getWeight() const { return weight; }

private:
    string owner;       //狗主人
//    string feature;     //主要特征描述
    string breed;       //品种
    unsigned int age;   //年龄
    double weight;      //体重,单位kg
};

//Point类:由于不涉及深拷贝,故不必自行实现拷贝构造函数
class Point {
public:
    explicit Point(double xx = 0, double yy = 0) {    //构造函数
        x = xx;
        y = yy;
    }

    double getX() const {
        return x;
    }

    double getY() const {
        return y;
    }

private:
    double x, y;
};

class Rectangle {
public:
    Rectangle(Point p1, Point p2) : leftBottom(p1), rightBottom(p2) {}

    double getLength() { return abs(rightBottom.getX() - leftBottom.getX()); }

    double getWidth() { return abs(rightBottom.getY() - leftBottom.getY()); }

private:
    Point leftBottom, rightBottom;      //矩形的左下角与右下角两点的坐标
};

class Date {
public:
    explicit Date(int y = 2023, unsigned m = 1, unsigned d = 1);

    bool isValid() const;          //类的实例(对象)是否有效
    static bool isLeap(int y);

    int getYear() const { return year; }

    unsigned int getMonth() const { return month; }

    unsigned int getDay() const { return day; }

    void setYear(int y) { year = y; }

    void setMonth(unsigned int m) { month = m; }

    void setDay(unsigned int d) { day = d; }

    void showTime() const;

private:
    int year: 23;          //取值范围为[-2^22 2^22 - 1]
    unsigned month: 4;     //取值范围为[0,15]
    unsigned day: 5;     //取值范围为[0,31]
};

Date::Date(int y, unsigned int m, unsigned int d) : year(y), month(m), day(d) {
    /*关于Date类的构造函数的参数合法性的检查与处理
     * 1.目标:
     * 年份:①当其值为-3200~3199且不为0的整数时,程序可准确判断出该年是否为闰年;②当其值为-2^22~2^22-1且不为0的整数时,
     *     程序可正常执行;否则,不实例化Date类
     * 月份:取值范围是1~12,超出此范围则不实例化Date类
     * 天数:1~31. 1、3、5、7、8、10和12月为31天;4、6、9和11月为30天. 平年2月为28天,闰年2月为29天.不符合上述要求时不实例化Date类
     * 2.处理的两类方法
     * 方法一:抛出异常.这是通知对象构造失败的唯一方法,但是该对象的析构函数将不会被调用,如果处理不当,则可能发生内存泄漏;且这个异常得由
     * 调用者捕获并处理
     * 方法二:暂时忽略全部错误.构造完成后用isValid函数返回构造函数的有效性
     * 3.由于目前尚未学习高级异常,故而采用方法二
     */
}

bool Date::isValid() const {
    bool flag = false;
    if (year != 0 && day != 0) {
        switch (month) {
            case 2: {
                if (isLeap(year)) {
                    if (day <= 29)
                        flag = true;
                } else {
                    if (day <= 28)
                        flag = true;
                }
            }
                break;
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                flag = true;
                break;
            case 4:
            case 6:
            case 9:
            case 11: {
                if (day != 31) flag = true;
            }
                break;
            default:
                break;
        }
    }
    return flag;
}

bool Date::isLeap(int y) {
    /*关于年份的一些说明
     * 1.地球公转周期,即以恒星为参照物,称为恒星年,为365天6时9分5秒;若以太阳为参照物,则称为回归年,为365天5时48分46秒.
     * 恒星年为地球真正的公转周期.为便于编写历法,平年采用365天,少的时间用闰年来补足
     * 2.若年份绝对值小于3200,则"四年一闰;百年不闰,四百年再闰".当年份绝对值大于等于3200时,若其能整除3200,则一般为平年;
     * 若能整除172800,则是闰年.例如86400不是闰年.此外,每过8000年又会有大约1天的误差.最后,地球的公转速度是越来越慢的,
     * 采用观测值比估算值更明智.综上所述,距离本年太远(前或后)的年份用更为"精确"的估算值意义不大
     * 3.由于没有公元0年,所以判断公元前年份的闰平性时只需先将年份减1,然后用第2条的方法判断即可
     */
    if (y < 0)
        y = abs(y) - 1;
    if (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0))
        return true;
    else
        return false;
}

void Date::showTime() const {
    cout << year << "/" << month << "/" << day << endl;
}

enum Gender {
    MALE, FEMALE
};

class Employee {
public:
    //构造函数、拷贝构造函数、析构函数
    explicit Employee(string num = "invalid", Gender g = MALE, Date b = Date(2023, 1, 1), string id = "invalid")
            : jobNO(std::move(num)), gender(g), birthday(b), ID(std::move(id)) {}

    Employee(Employee &employee) = default;

    ~Employee() = default;

    //get函数
    const string &getJobNo() const { return jobNO; }

    Gender getGender() const { return gender; }

    const string &getID() const { return ID; }

    //set函数,其余的数据暂不提供set函数
    void setGender(Gender g = MALE) { gender = g; }

    //其他接口
    void enterInfo();

    void showInfo();

    bool isValid();

private:
    //除生日外,不对其他数据进行合法性检查
    string jobNO;       //工号
    Gender gender;      //性别
    Date birthday;      //出生日期
    string ID;          //身份证号码
};

void Employee::showInfo() {
    cout << "Gender:   ";
    switch (gender) {
        case MALE:
            cout << "male" << endl;
            break;
        case FEMALE:
            cout << "female" << endl;
            break;
        default:
            break;
    }

    //Date类并未重载<<运算法,所以只能如此输入或改造Date类的showTime函数
    cout << "JobNO.:   " << jobNO << endl;
    cout << "Birthday: " << birthday.getYear() << "/" << birthday.getMonth() << "/" << birthday.getDay() << endl;
    cout << "ID:       " << ID << endl;
}

void Employee::enterInfo() {
    cout << "Enter your Gender please (0 --- male   1 --- female):";
    int sex;
    cin >> sex;
    while (sex != 0 && sex != 1) {
        cout << "Enter your Gender please (0 --- male   1 --- female):";
        cin >> sex;
    }
    gender = Gender(sex);

    cout << "Enter your job number please:";
    cin >> jobNO;

    /*如果要同时支持两种及以上的日期格式,则需要对字符串进行截取和转化.下面提供一些思路
     * 1.自编函数.依次扫描字符串中的字符,根据分隔符提取年月日
     * 2.调用库函数.先用string接受用户输入的日期,确定出分隔符的下标,然后根据分隔符编写sscanf函数直接转换为年月日字符串,
     * 最后使用stoi分别将其转换为int型变量
     */
    cout << "Enter your birthday please (y/m/d):";      //为简化函数,仅支持一种日期格式
    int y, m, d;
    scanf("%d/%d/%d", &y, &m, &d);
    birthday = Date(y, m, d);

    cout << "Enter your ID please:";
    cin >> ID;      //不对身份证号码的长度等进行检查
}

bool Employee::isValid() {
    //暂时仅根据生日的合法性来标记人员信息的合法性,若后续检查了所有数据的合法性,则可修改此函数
    return birthday.isValid();
}

class DataType {
public:
    explicit DataType(int a = 0, double b = 0, char c = '\0') : integer(a), decimal(b), ch(c) {}

    void showInfo() const {
        cout << integer << " " << decimal << " " << ch << endl;
    }

private:
    int integer;
    double decimal;
    char ch;
};

class Circle {
public:
    explicit Circle(double r = 0) : radius(r) {}

    double getArea() const { return PI * radius * radius; }

private:
    double radius;
};

class Tree {     //树不存在返老还童,所以将ages和years设为unsigned int可避免参数检查
public:
    explicit Tree(unsigned ages = 0) : ages(ages) {}

    void grow(unsigned years) { ages += years; }

    unsigned age() const { return ages; }

private:
    unsigned ages;
};

enum MWL {
    _32bit, _64bit
};    //字长(Machine Word-Length)
enum Core {
    Solo, Dual, Quad
};        //核心数:单核、双核、四核
class CPU {
private:
    unsigned CF: 31;       //时钟频率(Clock Frequency),单位Hz
    MWL mwl: 1;
    Core core: 2;
    unsigned HyperThreading: 1;
};

class Complex {
public:
    Complex(double r = 0.0, double i = 0.0) : real(r), imaginary(i) {}

    void add(Complex c) {
        real += c.real;
        imaginary += c.imaginary;
    }

    void show() const {
        cout << real << "+" << imaginary << "i" << endl;
    }

private:
    double real, imaginary;      //复数的实部和虚部
};

int main() {
    //4-1:public和private的作用是设置类中成员的访问权限;公有类型成员可以在类外访问,而私有类型成员只能在类内访问

    //4-2:protect关键字与private关键字的访问权限类似,仅在继承等时有所区别

    //4-3:构造函数用于初始化成员数据,析构函数用于类消亡前的清理或善后工作

    //4-4:数据成员可以是公有的,例如可使用结构体将多个数据类型组合在一起;成员函数可以是私有的,例如中间操作函数可以设为私有的.

    //4-5:可以不同.因为同一类的不同对象除了属于同一类之外没有其他交集,其中的数据成员和成员函数当然不同

    /*4-6:复制构造函数是一种能够复制本类对象的构造函数,其形参是本类对象的引用,作用是用已存在的对象去初始化另一个同类对象.
    被调用的情况:①用已存在的对象去初始化另一个同类对象;②函数形参为对象,调用函数时,进行形参与实参结合时;
               ③函数的返回值类型是对象
    */

    //4-7:在初始化对象时,复制构造函数即可进行浅拷贝,也能进行深拷贝;若没有重载=,则=调用的就是浅拷贝型复制构造函数

    //4-8:
    Dog lele("Li Hua", "Chinese rural dog", 3, 9.8);
    cout << "lele's age is " << lele.getAge() << endl;
    cout << "lele's weight is " << lele.getWeight() << endl;
    lele.show();

    //4-9:①Rectangle类并不检查两点是否在矩形的副对角线上;②不要求矩形的长度大于宽度.本人认为这两点均应由调用者处理
    Rectangle a(Point(1, 2), Point(3, 4));
    cout << "The area of rectangle a is " << a.getLength() * a.getWidth() << endl;

    //4-10:Employee类可完善的地方非常多,本人仅说明了一部分,要是你感兴趣的话,可以自行实现.私以为,大可不必在此浪费时间
    Employee Jack;
    Jack.enterInfo();
    if (Jack.isValid())
        Jack.showInfo();
    else
        cerr << "\nError: invalid birthday." << endl;

    /*4-11:
     * 修改4-9的Rectangle类
     * ①增加长度length和宽度width属性
     * ②增加私有成员函数setSide:根据两点坐标计算出长度和宽度以初始化length和width
     *   增加公共成员函数getArea:return length * width;
     */

    //4-12:此题容易让人误解成用户输入一个由整数、浮点数和字符组成的字符串,构造函数分类型处理此数据.如此一来,此题就复杂了
    DataType dt;
    dt.showInfo();
    dt = DataType(1, 2.3, 'c');
    dt.showInfo();

    //4-13:
    Circle c(1);
    cout << "Area: " << c.getArea() << endl;

    //4-14:
    Tree t;
    cout << "Your Tree is " << t.age() << " year(s) old." << endl;
    t.grow(10);
    cout << "Your Tree is " << t.age() << " year(s) old." << endl;

    /*4-15:
     * 1.解答:见./UML/4_15.png
     * 2.UML类图自动生成工具
     *   ①IDE:Visual Studio:选中源文件-->右键-->查看类图;JetBrains全家桶:选中类文件-->右键-->生成图表,
     *     IDEA和PyCharm都可以,但CLion 2022.3.1却不行
     *   ②插件:下载plantUML integration,用plantUML语言编写(类似编程),其支持很多常见IDE.请自行搜索教程
     *   ③专业UML绘制软件:Visual Paradigm、processOn、Aastah、Visio、draw.io
     *   推荐:Visual Paradigm.它可以运行在主流IDE中运行(Eclipse/NetBeans/IntelliJ IDEA/Visual Studio/Android Studio),
     *   支持UML和源代码双向转换(支持十几种语言),强大的文档生成器...详见https://www.visual-paradigm.com/cn/
     * 3.如无特殊说明,以后的UML图均由Visual Paradigm自动生成,且保存在UML文件夹中.
     */

    /*4-16:
     * 1.解答:见./UML/4_16.png
     * 2.继承
     *   2.1一般语法:
     *             class 派生类/子类名 : [继承方式] 基类/父类名{派生类/子类新增成员}
     *   2.2继承方式:
     *   相同点:①派生类均无法访问基类的private成员;②派生类并不会继承基类的构造函数、拷贝构造函数和析构函数,但是派生类可以调用
     *   不同点:
     *   ①public继承方式:    基类中的public和protected成员的属性在派生类中均不变
     *   ②protected继承方式: 基类中的public和protected成员的属性在派生类中均变为protected
     *   ③private继承方式:   基类中的public和protected成员的属性在派生类中均变为private
     */

    /*4-17:
     * 1.解答:见./UML/4_17.png
     * 2.分析:题目已经指出了答案，即CourseSchedule ------> Course
     */

    /*4-18:
     * 1.解答:见4_18.png
     * 2.分析:①（双向）关联.每个院系至少要有1个教师作为其成员，所以类Teacher的重数为0..n；每个教师可以属于0个或多个（其实是任意个）
     * 院系需要保存成员信息，所以类Department的重数为*。②聚集.聚集对象department与被聚集对象teacher之间可以分割，它们有自己的生命
     * 周期，即每个院系都有教师，但院系解散并不影响教师的存在。
     * 3.思考:如果学校档案库存有所有教师的档案，则院系仅需保存教师的姓名和教师编号，增删改查等操作基本都能通过档案库实现，教师的信息一般
     * 是以年为单位变动，所以也不必担心给档案库造成较大的服务压力。所以，在这种情况下，院系和教师之间也就不存在依赖关系了。
     */

    /*4-19:
     * 在64位Windows 10 操作系统中,使用CLion 2022.3.1集成开发环境(32位)和GNU C++ Compiler 12.2.0编译器(w64 11.0)编译
     * 1.运行结果: 8
     * 2.解释:如果将类CPU的mwl属性改为31位,整个类将占65位,那么运行结果变为12,表明存储第65位时系统填充了31位,这说存储字长取决于
     * IDE的位数,存储字长为32位,数据按边界对齐方式存储,至于寻址方式和指令字长,则需要查看汇编代码或机器码,感兴趣的话可以自行研究.
     */
    cout << "Size of CPU: " << sizeof(CPU) << endl;

    /*4-20:题目字打错了,应该是定义复数类Complex
     * 分析:①c1调用构造函数完成了初始化;②用4.5初始化c2,此过程发生了从double到Complex的隐式类型转换,所以构造函数前面不能有
     * explicit关键字;③c1和c2均有自己的数据域,题目实际要求将c2的实部和虚部分别加到c1的实部和虚部上;④无需解释
     */
    Complex c1(3, 5);
    Complex c2 = 4.5;
    c1.add(c2);
    c1.show();

    //4-21:BLUE的值是102

    //4-22: Assigning to 'Weekday' from incompatible type 'int'
    enum Weekday {
        SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY
    };
    Weekday weekday = FRIDAY;
    int temp = 5;
    //weekday = temp;

    return 0;
}
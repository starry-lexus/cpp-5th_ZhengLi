/**
 *@author 岛中客、眼中你
 *@date 2023/1/13
 */
#include <iostream>
#include <cstring>
#include <cassert>
#include <string>

using namespace std;

const double PI = 3.1415926;

class SimpleCircle {     //CLion:此文件过于复杂,无法进行数据流分析
private:
    int *itsRadius;
public:
    SimpleCircle() { itsRadius = new int(0); }

    explicit SimpleCircle(int radius) { itsRadius = new int(radius); }

    SimpleCircle(const SimpleCircle &sc) {
        itsRadius = new int(*(sc.itsRadius));
    }

    ~SimpleCircle() { delete itsRadius; }

    int getRadius() { return *itsRadius; }

    double getArea() { return PI * (*itsRadius) * (*itsRadius); }

    double getCircumference() { return 2 * PI * (*itsRadius); }
};

void reverse(string &s) {
    if (s.length() < 2)
        return;
    else {
        /*s.substr(1, s.length() - 2)返回的是一个basic_string<char>类型的临时对象,所以其不能被引用或
         * 用指针指向,从而也就无法将其传给函数reverse以便直接操控s的中间字符串
         */
        string in = s.substr(1, s.length() - 2);    //暂存截取的中间字符串
        reverse(in);    //逆置in
        s = s[s.length() - 1] + in + s[0];   //将逆置后的in与原字符串的首尾字符拼接成最终结果
    }
}

class MyString {
private:
    char *str;      //字符数组首地址
    unsigned int size;
public:
    friend ostream &operator<<(ostream &os, const MyString &s);

    friend istream &operator>>(istream &is, MyString &s);

    MyString();

    MyString(const char *s);        //这里允许字符数组、字符串字面量(不包括string)向char*隐式转换,所以不加explicit
    MyString(const MyString &rhs);

    ~MyString() {
        delete[] str;
        size = 0;
    }

    unsigned int length() const { return size; }

    MyString &operator=(const MyString &s);

    MyString operator+(const MyString &s) const;      //不返回常对象的理由见函数的实现部分
    MyString &operator+=(const MyString &s);

    const char &operator[](unsigned int index) const {      //"[]"运算符的const版本,满足常对象的需求
        assert(index >= 0 && index <= size);        //检查下标是否越界,建议用第12章的异常处理
        return str[index];
    }

    char &operator[](unsigned int index) {           //返回引用是为了支持赋值操作,例如s[1] = 'a';
        assert(index >= 0 && index <= size);        //检查下标是否越界,建议用第12章的异常处理
        return str[index];
    }
};

MyString::MyString() {
    str = new char[1];
    str[0] = '\0';
    size = 0;
}

MyString::MyString(const char *s) {
    size = strlen(s);
    str = new char[size + 1];
    strcpy(str, s);
}

MyString::MyString(const MyString &rhs) {
    size = rhs.size;
    str = new char[size + 1];
    strcpy(str, rhs.str);
}

MyString &MyString::operator=(const MyString &s) {
    if (&s != this) {             //如果不是自我赋值
        if (size != s.size) {     //如果本对象中数组大小与s不同,那么释放原数组的内存,然后为其重新分配内存
            delete[] str;       //删除原有内存
            size = s.size;      //设置本对象的数组大小
            str = new char[size + 1];   //重新分配内存
        }
        strcpy(str, s.str);
    }
    return *this;               //返回当前对象的引用
}

MyString MyString::operator+(const MyString &s) const {
    /*为什么不返回常对象？
     * 众所周知,在C++中,含算术运算符的表达式只能用作右值,即可以有c = a + b,但不能有a + b = c,然而可以出现(a + b) = c,这其实是
     * 将c赋值给一个临时无名对象,这没有任何意义.为了避免出现这种无意义的赋值情况,可以在重载+、-、*或/等算数运算符时选择返回常对象,
     * 但本人认为这会降低代码的可读性,另外,既然C++标注允许出现这种无意义的赋值情况出现,那我们又何必禁止呢?
     */
    MyString temp;
    temp.size = size + s.size;
    temp.str = new char[temp.size + 1];

    //这两个函数的返回值都是Dest,如果改为先拼接后复制,那么str就被改变了
    strcat(strcpy(temp.str, str), s.str);

    return temp;    //返回临时对象
}

MyString &MyString::operator+=(const MyString &s) {
    /* 1.你也可以用循环语句完成字符串的复制,但这无疑会增加代码量
     * 2.一种比较笨的办法:创建临时对象temp,将str和s.str中的字符先拼接后复制到temp.str中,然后将temp赋值给*this,最后返回*this.
     * 显然,这种多此一举的做法不仅增加了时空开销,而且代码也不够简洁
     */
    char *temp = str;               //暂存本对象字符数组的首地址
    size += s.size;
    str = new char[size + 1];       //重新分配空间
    strcat(strcpy(str, temp), s.str);
    delete[] temp;                  //删除原有空间
    return *this;
}

ostream &operator<<(ostream &os, const MyString &s) {
    for (int i = 0; i < s.size; i++)
        os << (s.str)[i];               //不要将os替换为cout,否则程序就不能支持其他输出流了
    return os;
}

istream &operator>>(istream &is, MyString &s) {
    is >> s.str;
    s.size = strlen(s.str);
    return is;
}

void T_Matrix(double a[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < i; j++) {       //若改为j < 3,则数组会恢复原样
            swap(a[i][j], a[j][i]);
        }
    }
}

void T_Matrix(double **a, double **at, unsigned rowSize, unsigned columnSize) {
    for (int i = 0; i < rowSize; i++)
        for (int j = 0; j < columnSize; j++)
            at[j][i] = a[i][j];
}

void printMatrix(double **matrix, unsigned rowSize, unsigned columnSize) {
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < columnSize; j++) {
            cout << matrix[i][j] << "  ";
        }
        cout << endl;
    }
}

class Employee {
public:
    Employee();

    Employee(const char *name, const char *city, const char *street, const char *postalCode);

    Employee(const Employee &employee);

    ~Employee();

    void setName(const char *newName);

    void display();

private:
    char *name;
    char *city;
    char *street;
    char *postalCode;
};

Employee::Employee() {
    name = new char[1];
    city = new char[1];
    street = new char[1];
    postalCode = new char[1];
    name[0] = '\0';
    city[0] = '\0';
    street[0] = '\0';
    postalCode[0] = '\0';
}

Employee::Employee(const char *name, const char *city, const char *street, const char *postalCode) {
    this->name = new char[strlen(name)];
    this->city = new char[strlen(city)];
    this->street = new char[strlen(street)];
    this->postalCode = new char[strlen(postalCode)];
    strcpy(this->name, name);
    strcpy(this->city, city);
    strcpy(this->street, street);
    strcpy(this->postalCode, postalCode);
}

Employee::Employee(const Employee &employee) {
    name = new char[strlen(employee.name)];
    city = new char[strlen(employee.city)];
    street = new char[strlen(employee.street)];
    postalCode = new char[strlen(employee.postalCode)];
    strcpy(name, employee.name);
    strcpy(city, employee.city);
    strcpy(street, employee.street);
    strcpy(postalCode, employee.postalCode);
}

Employee::~Employee() {
    delete[] name;
    delete[] city;
    delete[] street;
    delete[] postalCode;
}

void Employee::setName(const char *newName) {
    char *temp = name;
    name = new char[strlen(newName)];
    strcpy(name, newName);
    delete[] temp;
}

void Employee::display() {
    char *n = name, *c = city, *s = street, *p = postalCode;
    while (*n != '\0') {
        cout << *n;
        n++;
    }
    cout << "  ";
    while (*c != '\0') {
        cout << *c;
        c++;
    }
    cout << "  ";
    while (*s != '\0') {
        cout << *s;
        s++;
    }
    cout << "  ";
    while (*p != '\0') {
        cout << *p;
        p++;
    }
    cout << endl;
}

int main() {
    //6-1:一共有10 * 5 * 15 = 750

    //6-2:第一个元素是a[0],最后一个元素是a[19]

    //6-3:
    int a1[] = {1, 2, 3, 4, 5};

    //6-4: sizeof(oneArray) / sizeof(oneArray[0])

    /*6-5:在本题中,两种for循环的特点如下
     * ①基于范围的for循环:适合遍历,迭代语句比较难写或可读写较差,若要初始化,则需引入变量
     * ②普通for循环:需要找到一个关系式来初始化
     */
    int b[5][3];
    /*
    int init = 1;
    for (auto & i : b)
        for (int & j : i)
            j = init++;
    */
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            b[i][j] = 3 * i + j + 1;
        }
    }

    //6-6:*作为一元运算符时用于解析变量地址,&作为一元运算符时用于取变量地址

    /*6-7:指针在C/C++中就是地址;指针中存储的地址是间接地址,这个地址中的值是内容,假设int类型的数值10
     * 存储在地址为1000的存储单元中,则指针存储的地址就是1000,这个地址(1000)存储的就是10
     */

    //6-8:申请创建数组类型的对象较为方便int *p = new int[10];

    //6-9: '\0'

    //6-10:
    cout << "Enter 5 integers please:";
    int arr[5];
    for (int &i: arr)
        cin >> i;
    for (int &i: arr)
        cout << i << " ";
    cout << endl;

    /*6-11:
     * 1.区别:①引用相当于阉割版的指针常量,但引用比指针常量更加高层,类似于封装起来的类,它将地址隐藏了起来;②可以对指针常量取地址,
     * 但无法对引用取地址;③普通指针可以多次赋值,而引用只能在初始化时指定被引用的对象,之后就不能更改;④若要实现数据的双向传输或
     * 减小传递开销,则使用引用做函数参数要比用指针更贱方便、安全,可读性也更好
     * 2.只能使用指针,而不能使用引用的情况;
     *   ①需要通过分支语句确定出指针所指对象,或需要在中途改变它所指的对象
     *   ②指针的值可能为空指针
     *   ③需要使用函数指针
     *   ④需要存储用new动态创建的的对象或数组
     *   ⑤函数参数或返回类型为数组
     */

    /*6-12:
     * float *pfloat;
     * char *pstr;
     * struct Customer *pcus;
     */

    //6-13: cout << *fp << endl;

    //6-14:均占8字节
    double d = 1.2, *fd = &d;
    cout << "fd: " << sizeof(fd) << "B\n" << "*fd: " << sizeof(*fd) << "B" << endl;

    /*6-15:
     * const int* p1是指向整型常量的指针,通过p1无法修改其所指变量的值;int * const p2是指向整型的指针常量,一旦初始化,
     * 就不能再指向其他变量
     */

    //6-16:
    int a, *p = &a, &r = a;
    *p = 10;
    r = 5;

    //6-17:指针变量p未经初始化便使用,程序存在安全隐患

    /*6-18:
     * 1.存在的问题:函数fn1中动态申请了内存,但在使用后没有进行归还或释放,发生了内存泄漏.
     * 2.修改方法:①返回p,即动态申请的内存的首地址,让调用者用完后释放内存;②给函数fn1增加引用参数,令其传回p;
     *  ③重写此函数,不申请内存
     * 建议:显然,方法①更符合题目的考查意图,问题与解决办法一一对应,但在实际中这一工作需要由程序员保护实施,风险较大,
     * 所以,尽量不要写此类函数
     */

    /*6-19:
     * long (*f1)(int);            //直接声明函数指针
     * typedef long (*f2)(int);    //使用typedef
     * long (A::*f1)(int);
     */

    //6-20:
    SimpleCircle s1, s2(5), s3(s2);
    cout << "Circle\t" << "Radius\t" << "Area\t" << " Circumference" << endl;
    cout << "s1    \t" << s1.getRadius() << "\t" << s1.getArea() << "\t " << s1.getCircumference() << endl;
    cout << "s2    \t" << s2.getRadius() << "\t" << s2.getArea() << "\t " << s2.getCircumference() << endl;
    cout << "s3    \t" << s3.getRadius() << "\t" << s3.getArea() << "\t " << s3.getCircumference() << endl;

    /*6-21:To handle yourself, use your head; to handle others, use your heart.
     * 注意:若使用>>读取句子,则须跳过中间的空格;若使用getline读取句子,则须清除流中残留的空白字符(除非前面没有输出语句)
     */
    //清空缓冲区
    cin.clear();
    cin.sync();

    cout << "Enter an English sentence please:";
    string str;
    int cnt = 0;
    getline(cin, str);
    for (char i: str)
        if (isalpha(i))
            cnt++;
    cout << "The total of letters in the sentence: " << cnt << endl;

    /*6-22:
     * 错误做法:修改题目,例如将函数的返回类型修改为string,从而降低难度
     * 分析:递归算法的核心在于分解问题
     * ①若字符串s为空或者只有一个字符,则算法结束;
     * ②否则,先交换s[0]和s[s.length() - 1]),然后截取其余字符并将其作为新的s,回到步骤①;
     * 说明:由于string的substr函数的返回值类型为basic_string,所以尽管算法思想不变,但具体实现的代码却无法与之一一对应;
     * 若想将算法思想直接翻译为具体代码,则需自行实现返回值类型为string的字符串提取函数或设法将string类的substr函数的
     * 返回值类型修改为string(暂时不清楚能否通过继承后重写的方法)
     */
    string ts;
    cout << "Enter a string please:";
    getline(cin, ts);
    reverse(ts);
    cout << "Reverse:" << ts << endl;

    //6-23:你也可以将这8个成绩先写入数组,然后读取数组中的成绩,最后计算出平均成绩.但这不免有些多此一举了
    double total = 0.0, score;
    cout << "Enter the scores of 8 students:";
    for (int i = 0; i < 8; i++) {
        cin >> score;
        total += score;
    }
    cout << "The average score is " << total / 8 << endl;

    /*6-24:
     * 1.说明:类MyString仅实现了题目中明确要求的功能,未进行更多完善
     * 2.完善思路:自行实现string类的所有公共接口,未简化代码,可以调用<cstring>中的strcat/strncat/strcpy/strncpy/
     * strlen/strcmp/strchr(返回字符首次出现的位置)/strrchr/strstr/...更多说明信息请参考
     * https://zh.cppreference.com/w/cpp/header/cstring
     * 由此可见.<cstring>中的函数已经实现了string的大部分功能,剩余功能你可自行实现,当然你也可以选择不调用库函数,只是
     * 代码量稍多,性能可能较差.之后你可以根据自己的想法再添加其他函数,例如让用户根据实际情况选择更加适合的字符串匹配算法
     * (简单匹配算法和KMP算法)
     */
    char str1[] = "world";
    MyString s10 = "hello ", s20 = str1, s30, s40("jack"), s50(str1), s60 = s10;
    cout << "s1: " << s10 << "\t" << s10.length() << endl;
    cout << "s2: " << s20 << "\t" << s20.length() << endl;
    cout << "s3: " << s30 << "\t" << s30.length() << endl;
    cout << "s4: " << s40 << "\t" << s40.length() << endl;
    cout << "s5: " << s50 << "\t" << s50.length() << endl;
    cout << "s6: " << s60 << "\t" << s60.length() << endl;
    cout << "Enter a string please:";
    cin >> s30;
    cout << "s6: " << s30 << "\t" << s30.length() << endl;
    cout << s10 + s20 << endl;
    s10 += s20;
    cout << "s1: " << s10 << "\t" << s10.length() << endl;
    cout << s10[10] << endl;
    s10[10] = 'x';
    cout << s10 << endl;

    //6-25:矩阵转置即行列互换,题目并未明确指出函数的返回类型,故而可以选择直接修改数组或返回数组
    cout << "Enter a matrix of 3*3:";
    double matrix[3][3];
    for (auto &i: matrix) {
        for (double &j: i) {
            cin >> j;
        }
    }
    for (auto &i: matrix) {
        for (double j: i) {
            cout << j << "  ";
        }
        cout << endl;
    }

    T_Matrix(matrix);

    cout << endl;
    for (auto &i: matrix) {
        for (double j: i) {
            cout << j << "  ";
        }
        cout << endl;
    }

    /*6-26:
     * 1.分析:①题目要求由用户输入矩阵的行数和列数,即二维数组的行和列都是变量,由于在定义多维数组时,非第一维必须都是常量表达式,
     * 所以只能使用动态分配内存的方式来定义;②严格来说,C++并没有多维数组,n维数组就是元素为n-1维数组的一维数组,基于此,我们只
     * 需要一维一维地申请数组即可,对于二维数组,就是先申请行指针(共有row个),然后为每一行申请空间(申请大小为col,循环row次),
     * 在释放内存时先循环释放行空间(一维数组),再释放行指针(也是一维数组);③理论上矩阵转置函数可以有两种方式---返回一个二维数组
     * 和传入两个二维数组,但是前者的函数原型复杂、且需要主调函数释放内存(释放语句可读性较差,要是"忘记释放"则会造成内存泄漏),
     * 相比之下后者的函数原型简单,且符合"申请者释放内存"的理想情况;④动态申请的二维数组实际上是指针数据(其结构见P209图6-9(a)),
     * 而矩阵并不都是方阵,所以不能用原矩阵的空间存储转置后的矩阵(通过修改输出语句来强行节省空间不是个好主意)
     * 2.其他方法:既然C++中的二维数组的存储结构与一维数组的相同,只是解释和访问方式不同,那么我们也可以继续用一维数组存储,然后将输入
     * 输出语句全部放在函数中,主调函数只需传入一维数组(数组长度为行*列)和它的行和列即可。但这种方法的缺点也很多:①矩阵是二维,参数
     * 却是一维数组,不符合直观感受;②矩阵元素下标表达式、转置表达式和输出表达式不好找;③被调函数可以仅控制输入输出语句就可完成"转置";
     * ④如果主调函数想用一个二维数组接受结果而不允许改变原数组的值,此函数就会直接作废;⑤要是让主调函数用一维数组接受转置矩阵,那么
     * 被调函数就没有存在的必要,因为主调函数的输出语句决定了被调函数的返回结果是否
     */
    cout << "Enter the size of row and column of the matrix respectively:";
    unsigned row, col;
    cin >> row >> col;

    cout << "Enter the matrix orderly:";
    auto **m = new double *[row];       //创建原矩阵行指针
    auto **mt = new double *[col];      //创建转置矩阵的行指针
    for (int i = 0; i < row; i++)
        m[i] = new double[col];         //为每一行分配空间
    for (int i = 0; i < col; i++)
        mt[i] = new double[row];        //为转置矩阵的每一行分配空间

    //按线性代数中矩阵元素的下标顺序读取
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            cin >> m[i][j];

    cout << "Origin:" << endl;
    printMatrix(m, row, col);

    T_Matrix(m, mt, row, col);
    cout << "Transpose:" << endl;
    printMatrix(mt, col, row);      //注意转置矩阵存储空间的申请参数和打印参数

    //释放空间
    for (int i = 0; i < row; i++)
        delete[] m[i];
    delete[] m;

    for (int i = 0; i < col; i++)
        delete[] mt[i];
    delete[] mt;

    /*6-27:
     * 1.关于私有数据类型:尽管直接使用string存储这4个属性很方便,但是这样一来类Employee或其对象就得占用128字节空间,而如果采用
     * 字符指针(数据)动态申请和释放内存,那么就只会占用32字节空间,另外,这也算是对动态内存管理知识点的巩固.当然,这就让函数的设计
     * 变得复杂许多,这只是其中一种解题思路.
     * 2.关于数据有效性判断:这并不是本题应该考虑的问题,要判断城市名、街道名和邮编是否为杜撰以及它们之间是否匹配,可以使用GUI编程
     * 让其变为选项,也可借助数据库或文件进行判断等
     */
    Employee e1, e2("张三", "北京市", "海淀区万寿路街道", "100017");
    e1.display();
    e2.display();
    e1.setName("李四");
    e1.display();

    /*6-28:
     * 1.对6_10.cpp的修改结果见6_10r.cpp,对6_16.cpp的修改结果见6_16r.cpp
     * 2.由此可见,在进行数据的双向传输时,更适合传引用参数;在保存动态创建的临时对象的首地址时,使用指针更好
     */

    /*6-29:
     * 1.作用机理:程序用reinterpret_cast将int型一维数组的首地址(int*)强制转换成了double*,前者占4字节,后者占8字节,
     * 在执行完*p = 5之后,双精度浮点数5就会覆盖掉int型的arr[0]和arr[1]
     * 2.避免此类情况(类型不安全)的方法:除非有非常特殊的底层用途,否则不要用reinterpret_cast
     */

    /*6-30:
     * 1.reinterpret_cast一般用在非常特殊的底层
     * 2.const_static只用于将常指针转换为普通指针、将常引用转换为普通引用和某些特殊场合(例如P248~P249的例子)
     * 3.static_cast用于其他情况,最通用
     */

    return 0;
}
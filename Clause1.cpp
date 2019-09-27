/**
 * @file Clause1.cpp
 * @brief
 * @author dcl
 * @version 1.0.0.0
 * @date 2019-09-24
 */
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
using namespace std;

/**
 * @brief 模板推导中，初始化表达式的引用性会被忽略
 */

/**
 * @brief 类型推导，无非就是三种：
 *      1. T,按值传递： template <typename T> void func1(T param);   func(expr),exper的const和volatile会被忽略，因为exper并不能决定副本是否可以修改
 *      2. T&/T*,按引用传递： template <typename T> void func1(T& param);   func(expr),exper的const和volatile会被继承，因为向引用型别传入对象时期望保持不可修改的属性
 *      2. T&&,按万能引用传递： template <typename T> void func1(T&& param);   func(expr),exper为左值，T会被推导为引用类型（唯一可推导出T为引用类型的情况）
 */


/**
 * @brief 除了上述模板推导的主流情况，还有一个边缘情况值得了解：数组型别有别于指针型别，但由于在很多语境下数组会退化成指向其首元素的指针
 *      1. template <class T> void func1(T param);   const char name [] = "abcde"; name型别为const char[6],但是func1(name) name是个数组，但是T的型别却推导成const char*；
 *         尽管无法声明真正的数组型别的形参，但却可将形参声明成数组的引用： template <class T> void func2(T& param); func2(name),T的类型推导为const char[6].有意思的是可以
 *         借助该特性推导出数组的个数实现如 array_size;
 *      2. 函数型别也同样会退化成函数指针,推导规则和数组相同
 */

//函数模板，可用来推导出数组含有的元素个数
template <typename T, size_t N>
size_t array_size(T (&ar)[N])
{
    cout << "array size is "  << N << endl; 
    return N;
}

template<typename T>
class TD;

decltype(auto) f() //返回int&
{
    int x = 0;
    return (x);
}

auto f1() //返回 int
{
    int x = 0;
    return (x);
}

class Widget{
    public:
        struct DataType
        {
            DataType()
            {
                cout << "构造啦～～～" << endl;
            }

            DataType(DataType& t) 
            {
                cout << "copy constructor\n";
            }

            DataType(DataType&& t) 
            {
                cout << "move constructor\n";
            }

            ~DataType()
            {
                cout << "析构啦～～～" << endl;
            }

            DataType& operator=(const DataType& t)
            {
                cout << "assigan constructor" << endl;
            }
            int i = 0;
        };

        //using DataType = std::vector<int>;

        Widget() = default;

        DataType data() &
        {
            cout << "左值调用" << endl;
            return _vec;
        }
        
        DataType&& data() &&
        {
            cout << "右值调用" << endl;
            return std::move(_vec); 
            //return _vec;
        }

        //DataType data() &&
        //{
            //return std::move(_vec);
        //}
    private:
        DataType _vec;
};

template<class T>
class MyAlloc
{

};

template<typename T>
struct MyAllocaList
{
    typedef std::list<T, MyAlloc<T>> type;
};

template<typename T>
struct MyAllocaList1 : public  std::list<T, MyAlloc<T>>
{
};

std::vector<int> vec = { 0,1,2,3,4 };

template<typename Container, typename Index>
auto& Access(Container& c, Index i)
{
    return c[i];
}




int main(int argc, char** argv)
{
    //Widget tt;
    //Widget::DataType w1;
    //w1 = tt.data();
    
    //Widget::DataType w1 = tt.data();
    //Widget::DataType w2 = Widget().data();
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    auto it = std::find(vec.cbegin(), vec.cend(), 2);
    //TD<decltype(vec.cbegin())> td;
    //TD<decltype(it)> td;
   
    int i = 5;
    const int* const p = &i;
    TD<decltype(p)> td;
    auto test = p;
    TD<decltype(test)> td;
#if 0
    Widget::DataType&& w2 = Widget().data();
    //TD<decltype(Widget().data())> t;
    Access(vec, 0) = 5;
    for (auto& t : vec)
    {
        cout << t << "   end!" << endl;
    }
    int a[] = { 0,1,2,3,4,5 };
    char c[] = { 'a','b','c','d'};
    array_size(a);
    array_size(c);

    cout << "-----------\n";
    const int& ri = 5;
    const Widget& wd();
    cout << typeid(ri).name() << endl; //typeid使用的是T推导规则 ，因此推导出类型为int
    cout << typeid(wd).name() << endl;//typeid使用的是T推导规则 ，因此推导出类型为Widget
   //TD<decltype(wd)> t; //推导出真正的类型const widget&
   //TD<decltype(ri)> t;
    cout << "-----------\n";
#endif 
    return 0;
}

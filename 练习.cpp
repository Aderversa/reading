#include<array>
#include<iostream>
#include<thread>
#include<complex>
#include<vector>
#include<array>
#include<unordered_set>
//用非类型参数模板泛化Stack类
template<typename T,auto MaxSize>
class Stack
{
public:
	using size_type = decltype(MaxSize);
private:
	std::array<T, MaxSize>elems;
	size_type numElems;
public:
	Stack();
	void push(T const& elem);
	void pop();
	const T& top()const;
	bool empty()const
	{
		return numElems == 0;
	}
	size_type size()const
	{
		return numElems;
	}
};
//变参模板
template<typename T,typename... Types>
void print(T firstArg, Types... args)
{
	std::cout << firstArg << '\n';
	print(args...);
}
//变参模板可与非变参模板重载,如下：这个非变参模板的重载成了上面变参递归的结束条件
template<typename T>
void print(T arg)
{
	std::cout << arg << '\n';
}
//(sizeof...),运算符,计算参数包所含参数的数目
template<typename T, typename... Types>
void print(T firstArg, Types... args)
{
	std::cout << firstArg << '\n';
	std::cout << sizeof...(Types) << '\n';//计算剩余类型的数目
	std::cout << sizeof...(args) << '\n';//计算剩余args的数量
	print(args...);
}
//将sizeof...与变参模板结合使用
//但当只穿一个参数是会出问题，args为空但print(args...)依旧会被实例化，但此时没有不接受参数的print()
template<typename T, typename... Types>
void print(T firstArg, Types... args)
{
	std::cout << firstArg << '\n';
	if (sizeof...(args) > 0)
		print(args...);
}
//折叠表达式
template<typename... T>
auto foldSum(T... s)
{
	return (... + s);//(((s1+s2)+s3)...+sn)
}
//几乎所以的二元运算符都可以用于折叠表达式
//变参模板的使用
//第一，向一个由库启动的thread传递参数
std::thread t(foo, 42, "hello");//call foo(42,"hello") in a separate
//向一个由智能指针管理的，在堆中创建的对象的构造函数传递参数
auto sp = std::make_shared<std::complex<float>>(4.2, 7.7);
//向一个被push进vector中的对象的构造函数传递参数

//变参模板中...左右不能紧跟数字，如
template<typename... T>
void addOne(const T&... args)
{
	//print(args + 1...);ERROR
	print(args + 1 ...);//OK
	print((args + 1)...);//ok
}
//下面例子可以判断所有参数包中参数的类型是否相同
template<typename T1, typename... TN>
constexpr bool isHomogenerous(T1, TN...)
{
	return (std::is_same<T1, TN>::value&&...);
}
//变参下标
//例子：函数通过一组变参下标来访问第一个参数中相应的元素：
template<typename C, typename... Idx>
void printElems(const C& coll, Idx... idx)
{
	print(coll[idx]);
}
/*当调用了std::vector<std::string>coll={"sof","sok","sksk","sik"};
* printElems(coll,2,0,3);
* 相当于调用了print(coll[2],coll[0],coll[3]);
*/
//也可以将非类型模板参数声明成参数包
template<std::size_t... Idx,typename C>
void printElems(const C& coll)
{
	//此是Idx不再是类型名而是变量
	print(coll[Idx]);
}

//变参类模板
//类模板也是可以变参的。一个重要例子是，通过多个模板参数指定了class相应数据成员的类型
template<typename... Elements>class Tuple;
Tuple<int, std::string, char>t;
//还可以指定对象可能包含的类型
template<typename... Types>
class Varient;
Varient<int, std::string, char>v;
//也可以将class定义成代表一组下标的类型
template<std::size_t... idx>
class Indices {};
//可以用它定义一个通过print()打印std::array或者std::tuple中元素的函数，具体打印哪些元素由编译阶段的get<>从给定的下标中获取
template<typename T,std::size_t... idx>
void printByIdx(T t, Indices<idx...>)
{
	print(std::get<idx>(t));
}
//可以这样使用：std::array<std::string,5>arr={"Hello","my","new","!",""World};
//printByIdx(arr,Indices<0,4,3>());

//变参推断指引-----暂时不理解该部分
//变参基类及其使用
class Customer
{
private:
	std::string name;
public:
	Customer(std::string const& n):name(n){}
	std::string getName()const { return name; }
};
struct CustomerEq
{
	bool operator() ( Customer const& c1, Customer const& c2 )const 
	{
		return c1.getName() == c2.getName();
	}
};
struct CustomerHash
{
	std::size_t operator()(Customer const& c)const
	{
		return std::hash<std::string>()(c.getName());
	}
};
template<typename... Bases>
struct Overloader :Bases...
{
	using Bases::operator()...;
};
int main()
{
	using CustomerOP = Overloader<CustomerHash, CustomerEq>;
	std::unordered_set<Customer, CustomerHash, CustomerEq>coll1;
	std::unordered_set<Customer, CustomerOP, CustomerOP>coll2;
}
#include<array>
#include<iostream>
#include<thread>
#include<complex>
#include<vector>
#include<array>
#include<unordered_set>
//�÷����Ͳ���ģ�巺��Stack��
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
//���ģ��
template<typename T,typename... Types>
void print(T firstArg, Types... args)
{
	std::cout << firstArg << '\n';
	print(args...);
}
//���ģ�����Ǳ��ģ������,���£�����Ǳ��ģ������س��������εݹ�Ľ�������
template<typename T>
void print(T arg)
{
	std::cout << arg << '\n';
}
//(sizeof...),�����,���������������������Ŀ
template<typename T, typename... Types>
void print(T firstArg, Types... args)
{
	std::cout << firstArg << '\n';
	std::cout << sizeof...(Types) << '\n';//����ʣ�����͵���Ŀ
	std::cout << sizeof...(args) << '\n';//����ʣ��args������
	print(args...);
}
//��sizeof...����ģ����ʹ��
//����ֻ��һ�������ǻ�����⣬argsΪ�յ�print(args...)���ɻᱻʵ����������ʱû�в����ܲ�����print()
template<typename T, typename... Types>
void print(T firstArg, Types... args)
{
	std::cout << firstArg << '\n';
	if (sizeof...(args) > 0)
		print(args...);
}
//�۵����ʽ
template<typename... T>
auto foldSum(T... s)
{
	return (... + s);//(((s1+s2)+s3)...+sn)
}
//�������ԵĶ�Ԫ����������������۵����ʽ
//���ģ���ʹ��
//��һ����һ���ɿ�������thread���ݲ���
std::thread t(foo, 42, "hello");//call foo(42,"hello") in a separate
//��һ��������ָ�����ģ��ڶ��д����Ķ���Ĺ��캯�����ݲ���
auto sp = std::make_shared<std::complex<float>>(4.2, 7.7);
//��һ����push��vector�еĶ���Ĺ��캯�����ݲ���

//���ģ����...���Ҳ��ܽ������֣���
template<typename... T>
void addOne(const T&... args)
{
	//print(args + 1...);ERROR
	print(args + 1 ...);//OK
	print((args + 1)...);//ok
}
//�������ӿ����ж����в������в����������Ƿ���ͬ
template<typename T1, typename... TN>
constexpr bool isHomogenerous(T1, TN...)
{
	return (std::is_same<T1, TN>::value&&...);
}
//����±�
//���ӣ�����ͨ��һ�����±������ʵ�һ����������Ӧ��Ԫ�أ�
template<typename C, typename... Idx>
void printElems(const C& coll, Idx... idx)
{
	print(coll[idx]);
}
/*��������std::vector<std::string>coll={"sof","sok","sksk","sik"};
* printElems(coll,2,0,3);
* �൱�ڵ�����print(coll[2],coll[0],coll[3]);
*/
//Ҳ���Խ�������ģ����������ɲ�����
template<std::size_t... Idx,typename C>
void printElems(const C& coll)
{
	//����Idx���������������Ǳ���
	print(coll[Idx]);
}

//�����ģ��
//��ģ��Ҳ�ǿ��Ա�εġ�һ����Ҫ�����ǣ�ͨ�����ģ�����ָ����class��Ӧ���ݳ�Ա������
template<typename... Elements>class Tuple;
Tuple<int, std::string, char>t;
//������ָ��������ܰ���������
template<typename... Types>
class Varient;
Varient<int, std::string, char>v;
//Ҳ���Խ�class����ɴ���һ���±������
template<std::size_t... idx>
class Indices {};
//������������һ��ͨ��print()��ӡstd::array����std::tuple��Ԫ�صĺ����������ӡ��ЩԪ���ɱ���׶ε�get<>�Ӹ������±��л�ȡ
template<typename T,std::size_t... idx>
void printByIdx(T t, Indices<idx...>)
{
	print(std::get<idx>(t));
}
//��������ʹ�ã�std::array<std::string,5>arr={"Hello","my","new","!",""World};
//printByIdx(arr,Indices<0,4,3>());

//����ƶ�ָ��-----��ʱ�����ò���
//��λ��༰��ʹ��
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
#include<iostream>
using namespace std;

#define tab "\t"
#define delimiter "\n--------------------------------------\n"

class Element
{
	int Data;			// значение элемента
	Element* pNext;		// Pointer to Next - указатель на следующий элемент
public:
	Element(int Data, Element* pNext = nullptr) :Data(Data), pNext(pNext)
	{
		cout << "EConstructor:\t" << this << endl;
	}
	~Element()
	{
		cout << "EDestructor:\t" << this << endl;
	}
	friend class ForwardList;
};

class ForwardList
{
	Element* Head;	// голова списка, указывает на начальный элемент списка
public:
	ForwardList()
	{
		// конструктор по умолчанию, который создает пустой список
		Head = nullptr;
		cout << "LConstructor:\t" << this << endl;
	}
	~ForwardList()
	{
		cout << "LDestructor:\t" << this << endl;
	}

	//			Добавление элементов:
	void push_front(int Data)
	{
		// 1) создаем новый элемент:
		Element* New = new Element(Data); // оператор new вызывает конструктор класса

		// 2) пристыковываем новый элемент к началу списка:
		New->pNext = Head;

		// 3) голову перенаправляем на новый элемент:
		Head = New;
	}
	void push_back(int Data)
	{
		// т.к. push_back() НЕ умеет работать с пустым списком, мы проверяем,
		// если список пуст, вызываем метод push_front(), который УМЕЕТ работать с пустым списком.
		if (Head == nullptr)return push_front(Data);

		// 1) создаем новый элемент:
		Element* New = new Element(Data);

		// 2) доходим до конца списка:
		Element* Temp = Head;
		
		// если в temp будет nullptr, то цикл прекращается, но чтобы мы остановились в последнем элементе, 
		// (Temp указывает на целиковый элемент, а нам нужен только адрес, поэтому)
		// то пишем (Temp->pNext):
		while (Temp->pNext)
			Temp = Temp->pNext; // -> не прямой доступ, разыменовывает указатель, что у нас по этому адресу

		// 3) после того как мы оказались в конце списка, можно добавлять новый элемент в конец:
		Temp->pNext = New;
	}

	//			Methods:
	void print()const
	{
		Element* Temp = Head;	// Temp - это итератор
								// итератор - это указатель, 
								// при помощи которого мы можем получить доступ к элементам нашей структуры данных)
								// то, чем мы будет перебирать список
		// первый добавленный элемент находится в конце (стек)
		while (Temp)
		{
			cout << Temp << tab << Temp->Data << tab << Temp->pNext << endl;
			Temp = Temp->pNext;	// переход на следующий элемент 
		}
	}
};

void main()
{
	setlocale(LC_ALL, "");

	int n;
	cout << "Введите воличество элементов списка: "; cin >> n;

	ForwardList list;
	for (int i = 0; i < n; i++)
	{
		list.push_front(rand() % 100);
	}
	list.print();
	list.push_back(123);
	list.print();
}
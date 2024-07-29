#include<iostream>
using namespace std;
using std::cout;
using std::cin;
using std::endl;

#define tab "\t"
#define delimiter "\n--------------------------------------\n"

class Element
{
	int Data;			// значение элемента
	Element* pNext;		// Pointer to Next - указатель на следующий элемент
	static int count;
public:
	Element(int Data, Element* pNext = nullptr) :Data(Data), pNext(pNext)
	{
		count++;
		cout << "EConstructor:\t" << this << endl;
	}
	~Element()
	{
		count--;
		cout << "EDestructor:\t" << this << endl;
	}
	friend class ForwardList;
};
int Element::count = 0;

class ForwardList
{
	Element* Head;	// голова списка, указывает на начальный элемент списка
	int size;
public:
	int get_size()const
	{
		return size;
	}
	ForwardList()
	{
		// конструктор по умолчанию, который создает пустой список
		Head = nullptr;
		size = 0;
		cout << "LConstructor:\t" << this << endl;
	}
	// чтобы запретить неявное преобразование при строчке:
	// ForwardList list = 5; в main
	explicit ForwardList(int size):ForwardList()
	{
		while (size--)push_front(0);
	}
	ForwardList(const ForwardList& other) :ForwardList()
	{
		*this = other; // повторно используем код CopyAssignment
		cout << "CopyConstructor:\t" << this << endl; // перебирает все элементы и записывает в новый список
	}
	ForwardList(ForwardList&& other) :ForwardList()
	{
		*this = std::move(other); // функ. std::move() принудительно вызывает MoveAssignment для класса
		cout << "MoveConstructor:\t" << endl;
	}
	~ForwardList()
	{
		while (Head)pop_back();
		cout << "LDestructor:\t" << this << endl;
	}

	//			Operators:
	ForwardList& operator=(const ForwardList& other)
	{
		if (this == &other) return *this;
		this->~ForwardList();
		for (Element* Temp = other.Head; Temp; Temp = Temp->pNext)
			push_back(Temp->Data);
		cout << "CopyAssignment:\t" << this << "\n";
		return *this;
	}
	ForwardList& operator=(ForwardList&& other)
	{
		if (this == &other)return *this;
		this->Head = other.Head;
		this->size = other.size;
		other.Head = nullptr;
		other.size = 0;
		cout << "MoveAssignment:\t" << this << endl;
	}

	// [] возвращают значение по индексу
	// два варианта чтобы мы смогли не только получить, но и записать
	int& operator[](int index)
	{
		Element* Temp = Head;
		for (int i = 0; i < index; i++)Temp = Temp->pNext;
		return Temp->Data;
	}
	const int& operator[](int index)const
	{
		Element* Temp = Head;
		for (int i = 0; i < index; i++)Temp = Temp->pNext;
		return Temp->Data;
	}

	//			Adding elements:
	void push_front(int Data)
	{
		// 1) создаем новый элемент:
		Element* New = new Element(Data); // оператор new вызывает конструктор класса

		// 2) пристыковываем новый элемент к началу списка:
		New->pNext = Head;

		// 3) голову перенаправляем на новый элемент:
		Head = New;

		size++;
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

		size++;
	}
	void insert(int Index, int Data)
	{
		if (Index > size)
		{
			cout << "Error: out of range" << endl;
			return;
		}
		if (Index == 0)return push_front(Data);

		// 1. созд. нужный эл-т
		Element* New = new Element(Data);

		// 2. доходим до нужного эл-та
		Element* Temp = Head;
		for (int i = 0; i < Index - 1; i++)
		{
			//if (Temp->pNext == nullptr)break;
				Temp = Temp->pNext;
		}

		// 3. вставляем элемент по индексу
		New->pNext = Temp->pNext;
		Temp->pNext = New;

		size++;
	}

	//			Removing elements:
	void pop_front()
	{
		if (Head == nullptr)return;

		// 1. запоминаем адрес удаляемого эл-та
		Element* erased = Head;
		// 2. исключаем удаляемый эл-т из списка
		Head = Head->pNext;
		// 3. удаляем первый элемент из памяти
		delete erased;

		/*
		--------------------------------------------------------------------
		new - создает объект в дин. памяти
		new[] - создает массив объектов в дин.памяти

		delete - удаляет один объект из дин. памяти
		delete[] - удаляет массив объектов из дин. памяти

		если один из них был с [], то и другой то же обяз. должен быть с []
		--------------------------------------------------------------------
		*/

		size--;
	}
	void pop_back()
	{
		if (Head->pNext == nullptr)return pop_front();
		// 1. Доходим до предпоследнего элемента списка
		Element* Temp = Head;
		while (Temp->pNext->pNext)Temp = Temp->pNext;

		// 2. Удаляем последний элемент из памяти
		delete Temp->pNext;

		// 3. Обнуляем указатель на последний элемент
		Temp->pNext = nullptr;

		size--;
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
			// А Temp-> - это мы разыменовали указатель temp и вышли на сам элемент
			// 
			// Temp - указатель Temp
			// *(Temp). или Temp-> - элемент Temp
			// Temp->pNext - указатель Temp->pNext
			// *(Temp->pNext). или Temp->pNext-> - элемент Temp->pNext
		}
		cout << "Кол-во элементов в списке: " << size << endl;
		cout << "Общее кол-во элементов : " << Element::count << endl;
	}
};

ForwardList operator+(const ForwardList& left, const ForwardList& right)
{
	ForwardList buffer;
	for (int i = 0; i < left.get_size(); i++)buffer.push_back(left[i]);
	for (int i = 0; i < right.get_size(); i++)buffer.push_back(right[i]);
	return buffer;
}

//#define BASE_CHECK
//#define COUNT_CHECK
//#define SIZE_CONSTRUCTOR_CHECK

void main()
{
	setlocale(LC_ALL, "");
#ifdef BASE_CHECK 
	int n;
	cout << "Введите воличество элементов списка: "; cin >> n;

	ForwardList list;
	for (int i = 0; i < n; i++)
	{
		list.push_front(rand() % 100);
	}
	list.print();

	/*list.push_back(123);
	list.print();

	list.pop_front();
	list.print();*/

	int index;
	int value;
	cout << "индекс: "; cin >> index;
	cout << "значение: "; cin >> value;
	list.insert(index, value);
	list.print();
#endif // BASE_CHECK 

#ifdef COUNT_CHECK
	ForwardList list1;
	list1.push_back(3);
	list1.push_back(5);
	list1.push_back(8);
	list1.push_back(13);
	list1.push_back(21);
	list1.print();

	ForwardList list2;
	list1.push_back(34);
	list1.push_back(55);
	list1.push_back(89);
	list1.print();
#endif // COUNT_CHECK

#ifdef SIZE_CONSTRUCTOR_CHECK
ForwardList list(5);
	for (int i = 0; i < list.get_size(); i++)
	{
		list[i] = rand() % 100;
	}
	for (int i = 0; i < list.get_size(); i++)
	{
		cout << list[i] << endl;
	}
	cout << endl;
#endif // SIZE_CONSTRUCTOR_CHECK

	ForwardList list1;
	list1.push_back(3);
	list1.push_back(5);
	list1.push_back(8);
	list1.push_back(13);
	list1.push_back(21);
	list1.print();

	ForwardList list2;
	list1.push_back(34);
	list1.push_back(55);
	list1.push_back(89);
	list1.print();

	cout << delimiter << endl;
	//ForwardList list3 = list1 + list2; // CopyAssignment
	cout << delimiter << endl;
	ForwardList list3;
	cout << delimiter << endl;
	list3 = list1 + list2; // CopyAssignment
	cout << delimiter << endl;
	list3.print();
}
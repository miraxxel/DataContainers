#include<iostream>
using namespace std;

#define tab "\t"
#define delimiter "\n----------------------------------------------------------\n"

/// //////////////////////////////////////////////////////////////////////////////////////////////////// ///
/// ////////////////				Class Declaration (Объявление класса):				//////////////// ///

template<typename T>class List
{
	// прямо внутри list мы можем описать структуру элемента, т.к. эти элементы, нигде кроме это листа не исп-ся
	class Element
	{
		T Data; // значение эл-та
		Element* pNext; // указатель на след. эл-т
		Element* pPrev; // указатель на предыдущ. эл-т
	public:
		// конструктор и деструктор, нужны чтобы мы видели, когда создаются и удаляются элементы
		Element(T Data, Element* pNext = nullptr, Element* pPrev = nullptr);
		~Element();
		friend class List; // для того, чтобы мы из List, имели прямой доступ к внутренностям Element, 
		// не смотря на то, что второй находится внутри первого.
	}*Head, * Tail; // объекты класса (или указатели на них) можно объявлять непосредственно после описания класса.
	// Element* Head;
	// Element* Tail;
	size_t size; // размер списка;  size_t - это typedef на 'unsigned int'
	// Директива 'typedef' создает псевдоним для существующего типа данных.
	// typedef имеет след. синтаксис:
	//			typedef существующий_тип_данных псевдоним
	class ConstBaseIterator
	{
	protected:
		Element* Temp;
	public:
		ConstBaseIterator(Element* Temp = nullptr) :Temp(Temp){}
		~ConstBaseIterator(){}

		//			Comparison operators:
		bool operator==(const ConstBaseIterator& other)const;
		bool operator!=(const ConstBaseIterator& other)const;

		const T& operator*()const; // вызывается для константного объекта
	};
public:
	// указатель на элемент
	class ConstIterator:public ConstBaseIterator 
	{
	public:
		ConstIterator(Element* Temp = nullptr) :ConstBaseIterator(Temp){}
		~ConstIterator(){}

		ConstIterator& operator++();
		ConstIterator operator++(int);
		ConstIterator& operator--();
		ConstIterator operator--(int);

	};

	class ConstReverseIterator:public ConstBaseIterator
	{
	public:
		ConstReverseIterator(Element* Temp = nullptr) :ConstBaseIterator(Temp){}
		~ConstReverseIterator(){}

		//		Increment/Decrement
		ConstReverseIterator& operator++();
		ConstReverseIterator operator++(int);

		ConstReverseIterator& operator--();
		ConstReverseIterator operator--(int);
	};

	// классы ниже не константные потому что внутри них есть не константный оператор разыменования
	// классы на базе которых они основаны, являются константными и не позволяют изменять содержимое контейнера
	class Iterator :public ConstIterator
	{
	public:
		Iterator(Element* Temp = nullptr):ConstIterator(Temp){}
		~Iterator(){}
		T& operator*();
	};
	class ReverseIterator :public ConstReverseIterator 
	{
	public:
		ReverseIterator(Element* Temp = nullptr):ConstReverseIterator(Temp) {}
		~ReverseIterator() {}
		T& operator*();
	};

	// begin - константный метод возвращаюищий константный итератор
	ConstIterator begin()const;
	Iterator begin();

	ConstIterator end()const;
	Iterator end();

	ConstReverseIterator rbegin()const;
	ReverseIterator rbegin();

	ConstReverseIterator rend()const;
	ReverseIterator rend();

	List();

	List(const std::initializer_list<T>& il);
	List(const List<T>& other);
	~List();

	//			Operators:
	List<T>& operator=(const List<T>& other);

	//			Adding element:
	void push_front(T Data);
	void push_back(T Data);
	void insert(T Data, int Index);

	//			Removing elements
	void pop_front();
	void pop_back();

	//			Methods
	void print()const;
	void reverse_print()const;
};

/// ////////////////					Class Declaration end:							//////////////// ///
/// //////////////////////////////////////////////////////////////////////////////////////////////////// ///

// ------------------------------------------------------------------------------------------------------ //

/// //////////////////////////////////////////////////////////////////////////////////////////////////// ///
/// ////////////////			Class definition (Определение класса):					//////////////// ///

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////						Element methods:							////////////////

// параметры по умолчанию пишутся только в прототипе, а в реализации убираются
template<typename T>List<T>::Element::Element(T Data, Element* pNext, Element* pPrev)
	:Data(Data), pNext(pNext), pPrev(pPrev) {}
template<typename T>List<T>::Element::~Element() {}

////////////////						Element methods:							////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/// --------------------------------------------------------------------------------------------- ///

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////						Iterators methods:							////////////////

//										ConstBaseIterator:
template<typename T>bool List<T>::ConstBaseIterator::operator==(const ConstBaseIterator& other)const
{
	return this->Temp == other.Temp;
}
template<typename T>bool List<T>::ConstBaseIterator::operator!=(const ConstBaseIterator& other)const
{
	return this->Temp != other.Temp;
}

template<typename T>const T& List<T>::ConstBaseIterator::operator*()const // вызывается для константного объекта
{
	return Temp->Data;
}

//										ConstIterator:
template<typename T>typename List<T>::ConstIterator& List<T>::ConstIterator::operator++()
{
	ConstBaseIterator::Temp = ConstBaseIterator::Temp->pNext;
	return *this;
}
template<typename T>typename List<T>::ConstIterator List<T>::ConstIterator::operator++(int)
{
	ConstIterator old = *this;
	ConstBaseIterator::Temp = ConstBaseIterator::Temp->pNext;
	return old;
}
template<typename T>typename List<T>::ConstIterator& List<T>::ConstIterator::operator--()
{
	ConstBaseIterator::Temp = ConstBaseIterator::Temp->pPrev;
	return *this;
}
template<typename T>typename List<T>::ConstIterator List<T>::ConstIterator::operator--(int)
{
	ConstIterator old = *this;
	ConstBaseIterator::Temp = ConstBaseIterator::Temp->pPrev;
	return old;
}

//										ConstReverseIterator
template<typename T>typename List<T>::ConstReverseIterator& List<T>::ConstReverseIterator::operator++()
{
	ConstBaseIterator::Temp = ConstBaseIterator::Temp->pPrev;
	return *this;
}
template<typename T>typename List<T>::ConstReverseIterator List<T>::ConstReverseIterator::operator++(int)
{
	ConstReverseIterator old = *this;
	ConstBaseIterator::Temp = ConstBaseIterator::Temp->pPrev;
	return old;
}
template<typename T>typename List<T>::ConstReverseIterator& List<T>::ConstReverseIterator::operator--()
{
	ConstBaseIterator::Temp = ConstBaseIterator::Temp->pNext;
	return *this;
}
template<typename T>typename List<T>::ConstReverseIterator List<T>::ConstReverseIterator::operator--(int)
{
	ConstReverseIterator old = *this;
	ConstBaseIterator::Temp = ConstBaseIterator::Temp->pNext;
	return old;
}

//										Iterator:
template<typename T>T& List<T>::Iterator::operator*() { return ConstBaseIterator::Temp->Data; }

//										ReverseIterator:
template<typename T>T& List<T>::ReverseIterator::operator*() { return ConstBaseIterator::Temp->Data; }

////////////////						Iterators methods:							////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/// -------------------------------------------------------------------------------------------- ///

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////						List methods:								////////////////
// begin - константный метод возвращаюищий константный итератор
template<typename T>typename List<T>::ConstIterator List<T>::begin()const { return Head; }
template<typename T>typename List<T>::Iterator List<T>::begin()
{
	return Head;
}
template<typename T>typename List<T>::ConstIterator List<T>::end()const
{
	return nullptr;
}
template<typename T>typename List<T>::Iterator List<T>::end()
{
	return nullptr;
}
template<typename T>typename List<T>::ConstReverseIterator List<T>::rbegin()const
{
	return Tail;
}
template<typename T>typename List<T>::ReverseIterator List<T>::rbegin()
{
	return Tail;
}
template<typename T>typename List<T>::ConstReverseIterator List<T>::rend()const
{
	return nullptr;
}
template<typename T>typename List<T>::ReverseIterator List<T>::rend()
{
	return nullptr;
}

template<typename T>List<T>::List() // конструктор по умолчанию, создает пустой список
{
	Head = Tail = nullptr;
	size = 0;
	cout << "LConstructor:\t" << this << endl;
}
// передаем по ссылке, чтобы не отрабатывал конструктор копирования, т.к. initializer_list контейнер, под него не нужно выделять память, нам нужно чтобы он принял в себя переданные объекты
template<typename T>List<T>::List(const std::initializer_list<T>& il) :List()
{
	for (T const* it = il.begin(); it != il.end(); ++it) // префиксный инкремент быстрее, т.к. не создает лишний объект
	{
		push_back(*it);
		//push_front(*it);
	}
}
template<typename T>List<T>::List(const List<T>& other) :List()
{
	*this = other;
	cout << "CopyConstructor:" << this << endl;
}
template<typename T>List<T>::~List()
{
	while (Tail)pop_back();
	cout << "LDestructor:\t" << this << endl;
}

//			Operators:
template<typename T>List<T>& List<T>::operator=(const List<T>& other)
{
	if (this == &other)return *this;
	while (Head)pop_front();
	for (Element* Temp = other.Head; Temp; Temp = Temp->pNext)push_back(Temp->Data);
	cout << "CopyAssignment:\t" << this << endl;
	return *this;
}

//			Adding element:
template<typename T>void List<T>::push_front(T Data)
{
	if (Head == nullptr && Tail == nullptr)
		Head = Tail = new Element(Data);
	else
	{
		//// 1. создаем элемент
		//Element* New = new Element(Data);
		//// 2. привяз. новый эл-т к нач. списка
		//New->pNext = Head;
		//// 3. привяз. головной элемент списка (бывший первый элемент) к новому элементу
		//Head->pPrev = New;
		//// 4. смещаем голову на новый элемент 
		//Head = New;

		Head = Head->pPrev = new Element(Data, Head);
	}

	size++;
}
template<typename T>void List<T>::push_back(T Data)
{
	// сразу же проверяем не пуст ли список
	if (Head == nullptr && Tail == nullptr)
		Head = Tail = new Element(Data);
	else
	{
		//// 1. создаем новый элемент
		//Element* New = new Element(Data);
		//// 2. включаем новый эл-т в список
		//New->pPrev = Tail;
		//Tail->pNext = New;
		//// 3. смещаем хвост на новый элемент
		//Tail = New;	

		Tail = Tail->pNext = new Element(Data, nullptr, Tail);
	}
	size++;
}
// перед index вместо int обычно пишется size_t
template<typename T>void List<T>::insert(T Data, int Index)
{
	if (Index > size)return;
	if (Index == 0)return push_front(Data);
	if (Index == size) return push_back(Data);
	Element* Temp;
	if (Index < size / 2)
	{
		Temp = Head;
		for (int i = 0; i < Index; i++)Temp = Temp->pNext;
	}
	else
	{
		Temp = Tail;
		for (int i = 0; i < size - Index - 1; i++)Temp = Temp->pPrev;
	}

	/*Element* New = new Element(Data);
	New->pNext = Temp;
	New->pPrev = Temp->pPrev;
	Temp->pPrev->pNext = New;
	Temp->pPrev = New;*/

	Temp->pPrev = Temp->pPrev->pNext = new Element(Data, Temp, Temp->pPrev);

	size++;
}

//			Removing elements
template<typename T>void List<T>::pop_front()
{
	if (Head == nullptr && Tail == nullptr)return;
	if (Head == Tail)
	{
		delete Head;
		Head = Tail = nullptr;
	}
	else
	{
		// 1. смещаем Голову на след. элемент
		Head = Head->pNext;
		// 2. удаляем начальный элемент списка 'Head->pPrev' из памяти
		delete Head->pPrev;
		// 3. зануляем указатель 'Head->pPrev'
		Head->pPrev = nullptr;
	}
	size--;
}
template<typename T>void List<T>::pop_back()
{
	/*if (Head == nullptr && Tail == nullptr)return;
	if (Head == Tail)
	{
		delete Tail;
		Head = Tail = nullptr;
	}*/
	if (Head == Tail)return pop_front();
	else
	{
		Tail = Tail->pPrev;
		delete Tail->pNext;
		Tail->pNext = nullptr;
	}
	size--;
}

//			Methods
template<typename T>void List<T>::print()const
{
	for (Element* Temp = Head; Temp; Temp = Temp->pNext)
		cout << Temp->pPrev << tab << Temp << tab << Temp->Data << tab << Temp->pNext << endl;
	cout << "Кол-во эл-ов в списке: " << size << endl;
}
template<typename T>void List<T>::reverse_print()const
{
	for (Element* Temp = Tail; Temp; Temp = Temp->pPrev)
		cout << Temp->pPrev << tab << Temp << tab << Temp->Data << tab << Temp->pNext << endl;
	cout << "Кол-во эл-ов в списке: " << size << endl;
}

template<typename T>List<T> operator+(const List<T>& left, const List<T>& right)
{
	List<T> buffer = left;
	for (List::ConstIterator it = right.begin(); it != right.end(); ++it)
	{
		buffer.push_back(*it);
		//*it *= 10;
	}
	return buffer;
}

/// ////////////////					Class definition end:							//////////////// ///
/// //////////////////////////////////////////////////////////////////////////////////////////////////// ///

//#define BASE_CHECK
//#define ITERATORS_CHECK
//#define OPERATOR_PLUS_CHECK

void main()
{
	setlocale(LC_ALL, "");
#ifdef BASE_CHECK
	int n;
	cout << "Введите размер списка: "; cin >> n;
	
	List list;

	for (int i = 0; i < n; i++)
	{
		list.push_back(rand() % 100);
		//list.push_front(rand() % 100);
	}
	list.print();
	list.reverse_print();

	int index;
	int value;
	cout << "Введите индекс добавляемого элемента: "; cin >> index;
	cout << "Введите значение добавляемого элемента: "; cin >> value;
	list.insert(value, index);
	list.print();
	list.reverse_print();
#endif //BASE_CHECK

#ifdef ITERATORS_CHECK
	List list = { 3, 5, 8, 13, 21 };
	//list.print();
	for (int i : list)cout << i << tab; cout << endl; // работает быстрее

	for (List::Iterator it = list.begin(); it != list.end(); ++it)
	{
		cout << *it << tab;
	}
	cout << endl;

	for (List::ReverseIterator it = list.rbegin(); it != list.rend(); ++it)
	{
		cout << *it << tab;
	}
	cout << endl;
#endif // ITERATORS_CHECK

#ifdef OPERATOR_PLUS_CHECK
	List list1 = { 3, 5, 8, 13, 21 };
	List list2 = { 34, 55, 89 };
	List list3 = list1 + list2;
	for (int i : list1) cout << i << tab; cout << endl;
	for (int i : list2) cout << i << tab; cout << endl;
	for (int i : list3) cout << i << tab; cout << endl;

	// как обращаться к любому контейнеру
	for (List::Iterator it = list3.begin(); it != list3.end(); ++it)
	{
		*it *= 100;
	}
	for (int i : list3) cout << i << tab; cout << endl;
#endif // OPERATOR_PLUS_CHECK

	List<int> i_list = { 3, 5, 8, 13, 21 };
	for (int i : i_list)cout << i << tab; cout << endl;
	for(List<int>::Iterator it = i_list.begin(); it != i_list.end(); ++it)
		cout << *it << tab;	cout << endl;
	for (List<int>::ReverseIterator it = i_list.rbegin(); it != i_list.rend(); ++it)
		cout << *it << tab; cout << endl;
	cout << delimiter << endl;
	//////////////////////////////////////////////////////////////////////////////////////

	List<double> d_list = { 2.7, 3.14, 5.5, 8.3 };
	for (double i : d_list)cout << i << tab; cout << endl;
	for (List<double>::Iterator it = d_list.begin(); it != d_list.end(); ++it)
		cout << *it << tab;	cout << endl;
	for (List<double>::ReverseIterator it = d_list.rbegin(); it != d_list.rend(); ++it)
		cout << *it << tab; cout << endl;
	cout << delimiter << endl;
	/////////////////////////////////////////////////////////////////////////////////////

	List<std::string> s_list = { "Хорошо", "живёт", "на", "свете", "Винни", "Пух" };
	for (std::string i : s_list)cout << i << tab; cout << endl;
	for (List<std::string>::Iterator it = s_list.begin(); it != s_list.end(); ++it)
		cout << *it << tab; cout << endl;
	for (List<std::string>::ReverseIterator it = s_list.rbegin(); it != s_list.rend(); ++it)
		cout << *it << tab; cout << endl;
	cout << delimiter << endl;
}
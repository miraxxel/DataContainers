// обычно в заголовочный файл подключени€ Ќ≈ вынос€тс€
#pragma once
#include<iostream>
#include<string>
using namespace std;

#define tab "\t"
#define delimiter "\n----------------------------------------------------------\n"

/// //////////////////////////////////////////////////////////////////////////////////////////////////// ///
/// ////////////////				Class Declaration (ќбъ€вление класса):				//////////////// ///

template<typename T>class List
{
	// пр€мо внутри list мы можем описать структуру элемента, т.к. эти элементы, нигде кроме это листа не исп-с€
	class Element
	{
		T Data; // значение эл-та
		Element* pNext; // указатель на след. эл-т
		Element* pPrev; // указатель на предыдущ. эл-т
	public:
		// конструктор и деструктор, нужны чтобы мы видели, когда создаютс€ и удал€ютс€ элементы
		Element(T Data, Element* pNext = nullptr, Element* pPrev = nullptr);
		~Element();
		friend class List; // дл€ того, чтобы мы из List, имели пр€мой доступ к внутренност€м Element, 
		// не смотр€ на то, что второй находитс€ внутри первого.
	}*Head, * Tail; // объекты класса (или указатели на них) можно объ€вл€ть непосредственно после описани€ класса.
	// Element* Head;
	// Element* Tail;
	size_t size; // размер списка;  size_t - это typedef на 'unsigned int'
	// ƒиректива 'typedef' создает псевдоним дл€ существующего типа данных.
	// typedef имеет след. синтаксис:
	//			typedef существующий_тип_данных псевдоним
	class ConstBaseIterator
	{
	protected:
		Element* Temp;
	public:
		ConstBaseIterator(Element* Temp = nullptr) :Temp(Temp) {}
		~ConstBaseIterator() {}

		//			Comparison operators:
		bool operator==(const ConstBaseIterator& other)const;
		bool operator!=(const ConstBaseIterator& other)const;

		const T& operator*()const; // вызываетс€ дл€ константного объекта
	};
public:
	// указатель на элемент
	class ConstIterator :public ConstBaseIterator
	{
	public:
		ConstIterator(Element* Temp = nullptr) :ConstBaseIterator(Temp) {}
		~ConstIterator() {}

		ConstIterator& operator++();
		ConstIterator operator++(int);
		ConstIterator& operator--();
		ConstIterator operator--(int);

	};

	class ConstReverseIterator :public ConstBaseIterator
	{
	public:
		ConstReverseIterator(Element* Temp = nullptr) :ConstBaseIterator(Temp) {}
		~ConstReverseIterator() {}

		//		Increment/Decrement
		ConstReverseIterator& operator++();
		ConstReverseIterator operator++(int);

		ConstReverseIterator& operator--();
		ConstReverseIterator operator--(int);
	};

	// классы ниже не константные потому что внутри них есть не константный оператор разыменовани€
	// классы на базе которых они основаны, €вл€ютс€ константными и не позвол€ют измен€ть содержимое контейнера
	class Iterator :public ConstIterator
	{
	public:
		Iterator(Element* Temp = nullptr) :ConstIterator(Temp) {}
		~Iterator() {}
		T& operator*();
	};
	class ReverseIterator :public ConstReverseIterator
	{
	public:
		ReverseIterator(Element* Temp = nullptr) :ConstReverseIterator(Temp) {}
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
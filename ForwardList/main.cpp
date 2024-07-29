#include<iostream>
using namespace std;
using std::cout;
using std::cin;
using std::endl;

#define tab "\t"
#define delimiter "\n--------------------------------------\n"

class Element
{
	int Data;			// �������� ��������
	Element* pNext;		// Pointer to Next - ��������� �� ��������� �������
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
	Element* Head;	// ������ ������, ��������� �� ��������� ������� ������
	int size;
public:
	int get_size()const
	{
		return size;
	}
	ForwardList()
	{
		// ����������� �� ���������, ������� ������� ������ ������
		Head = nullptr;
		size = 0;
		cout << "LConstructor:\t" << this << endl;
	}
	// ����� ��������� ������� �������������� ��� �������:
	// ForwardList list = 5; � main
	explicit ForwardList(int size):ForwardList()
	{
		while (size--)push_front(0);
	}
	ForwardList(const ForwardList& other) :ForwardList()
	{
		*this = other; // �������� ���������� ��� CopyAssignment
		cout << "CopyConstructor:\t" << this << endl; // ���������� ��� �������� � ���������� � ����� ������
	}
	ForwardList(ForwardList&& other) :ForwardList()
	{
		*this = std::move(other); // ����. std::move() ������������� �������� MoveAssignment ��� ������
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

	// [] ���������� �������� �� �������
	// ��� �������� ����� �� ������ �� ������ ��������, �� � ��������
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
		// 1) ������� ����� �������:
		Element* New = new Element(Data); // �������� new �������� ����������� ������

		// 2) �������������� ����� ������� � ������ ������:
		New->pNext = Head;

		// 3) ������ �������������� �� ����� �������:
		Head = New;

		size++;
	}
	void push_back(int Data)
	{
		// �.�. push_back() �� ����� �������� � ������ �������, �� ���������,
		// ���� ������ ����, �������� ����� push_front(), ������� ����� �������� � ������ �������.
		if (Head == nullptr)return push_front(Data);

		// 1) ������� ����� �������:
		Element* New = new Element(Data);

		// 2) ������� �� ����� ������:
		Element* Temp = Head;
		
		// ���� � temp ����� nullptr, �� ���� ������������, �� ����� �� ������������ � ��������� ��������, 
		// (Temp ��������� �� ��������� �������, � ��� ����� ������ �����, �������)
		// �� ����� (Temp->pNext):
		while (Temp->pNext)
			Temp = Temp->pNext; // -> �� ������ ������, �������������� ���������, ��� � ��� �� ����� ������

		// 3) ����� ���� ��� �� ��������� � ����� ������, ����� ��������� ����� ������� � �����:
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

		// 1. ����. ������ ��-�
		Element* New = new Element(Data);

		// 2. ������� �� ������� ��-��
		Element* Temp = Head;
		for (int i = 0; i < Index - 1; i++)
		{
			//if (Temp->pNext == nullptr)break;
				Temp = Temp->pNext;
		}

		// 3. ��������� ������� �� �������
		New->pNext = Temp->pNext;
		Temp->pNext = New;

		size++;
	}

	//			Removing elements:
	void pop_front()
	{
		if (Head == nullptr)return;

		// 1. ���������� ����� ���������� ��-��
		Element* erased = Head;
		// 2. ��������� ��������� ��-� �� ������
		Head = Head->pNext;
		// 3. ������� ������ ������� �� ������
		delete erased;

		/*
		--------------------------------------------------------------------
		new - ������� ������ � ���. ������
		new[] - ������� ������ �������� � ���.������

		delete - ������� ���� ������ �� ���. ������
		delete[] - ������� ������ �������� �� ���. ������

		���� ���� �� ��� ��� � [], �� � ������ �� �� ����. ������ ���� � []
		--------------------------------------------------------------------
		*/

		size--;
	}
	void pop_back()
	{
		if (Head->pNext == nullptr)return pop_front();
		// 1. ������� �� �������������� �������� ������
		Element* Temp = Head;
		while (Temp->pNext->pNext)Temp = Temp->pNext;

		// 2. ������� ��������� ������� �� ������
		delete Temp->pNext;

		// 3. �������� ��������� �� ��������� �������
		Temp->pNext = nullptr;

		size--;
	}



	//			Methods:
	void print()const
	{
		Element* Temp = Head;	// Temp - ��� ��������
								// �������� - ��� ���������, 
								// ��� ������ �������� �� ����� �������� ������ � ��������� ����� ��������� ������)
								// ��, ��� �� ����� ���������� ������
		// ������ ����������� ������� ��������� � ����� (����)
		while (Temp)
		{
			cout << Temp << tab << Temp->Data << tab << Temp->pNext << endl;
			Temp = Temp->pNext;	// ������� �� ��������� ������� 
			// � Temp-> - ��� �� ������������ ��������� temp � ����� �� ��� �������
			// 
			// Temp - ��������� Temp
			// *(Temp). ��� Temp-> - ������� Temp
			// Temp->pNext - ��������� Temp->pNext
			// *(Temp->pNext). ��� Temp->pNext-> - ������� Temp->pNext
		}
		cout << "���-�� ��������� � ������: " << size << endl;
		cout << "����� ���-�� ��������� : " << Element::count << endl;
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
	cout << "������� ���������� ��������� ������: "; cin >> n;

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
	cout << "������: "; cin >> index;
	cout << "��������: "; cin >> value;
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
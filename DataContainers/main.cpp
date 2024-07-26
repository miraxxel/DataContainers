#include<iostream>
using namespace std;

void main()
{
	setlocale(LC_ALL, "");

	const int n = 100;
	int arr[n]{};

	arr[32] = 123;

	for (int i = 0; i < n; i++)cout << arr[i] << "\t";

	nullptr;
	NULL;

	int* pa = nullptr;
	int* pb = NULL;
	int* pc = 0;
}
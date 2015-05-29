#include "ReverseOrder.h"

template <typename Iterator>
void Reverse_Order(Iterator begin, Iterator end)
{
	Iterator i = begin, j = --end;
	while (true)
	{
		std::swap(*i++, *j);
		if (i == j) break;
		if (i == --j) break;
	}
}


int Reverse_Test()
{
	std::fstream out;
	out.open("ReverseOrder/output.txt");


	std::vector <int> a;
	for (int i = 0; i < 11; ++i)
	{
		a.push_back(10 - i);
		out << a[i] << " ";
	}

	out << std::endl;
	std::vector <int> ::iterator beg = a.begin();
	std::vector <int> ::iterator end = a.end();
	Reverse_Order(a.begin(), a.end());
	for (int i = 0; i < 11; ++i)
	{
		out << a[i] << " ";
	}

	std::cout << "ReverseOrder finished OK." << std::endl;
	return 0;
}
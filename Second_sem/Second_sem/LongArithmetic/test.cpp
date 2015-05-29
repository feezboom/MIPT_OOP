#ifndef IOSTREAM
#include <iostream>
#define IOSTREAM
#endif
#include "longint.h"


int longartest()
{
	bigint a, b, c;
	ifstream in;
	ofstream out;
	in.open("input.txt");
	out.open("output.txt");
	cin >> a >> b;
	c = a - b;
	cout << c;
	system("pause");
	return 0;
}

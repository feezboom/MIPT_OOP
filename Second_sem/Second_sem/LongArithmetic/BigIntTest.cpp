#include "bigint.h"

int biginttest()
{
	bigint a, b, c;
	fstream in, out;
	in.open("LongArithmetic/input.txt");
	out.open("LongArithmetic/output.txt", ios::trunc | ios::out);
	if (!in.is_open()) 
		return 1;
	in >> a >> b;
	out << (a.div(b) == 1 ? a : a) << endl;

	cout << "Long Arithmetic finished. OK.\n";
	return 8;
}
#include "strings.h"
#include <fstream>

#pragma warning (disable:4996)

using namespace std;

void StringTest()
{
	line a, b; vector <char> c;

	FILE *input, *output;
		input = fopen("Strings/input.txt", "r");
		output = fopen("Strings/output.txt", "w");
	fclose(input); fclose(output);

	fstream in, out;
	in.open("Strings/input.txt");
	out.open("Strings/output.txt");
	
	in >> a >> b;
	out << a;
	out << endl << b;

	line d;
	d = a + b;
	out << d << endl;
	d.erase(1, 16);
	d.insert_back(a);
	out << d << endl;

	in.close(); out.close();
	cout << "Everything in strings OK." << endl;
}
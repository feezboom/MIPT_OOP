#include "topresult.h"

#include <fstream>
#include <iostream>
#include "../Strings/strings.h"
#include "../Pharmacy/pharmacy.h"

template <typename T>
int InvertedCompare(T a, T b)	//Функция обратного сравнения чисел double
{
	if (a > b) return -1;
	if (a < b) return 1;
	if (a == b) return 0;
	else
	{ 
		assert(0);
		return 333;
	}
}
template <typename T>
int Compare(T a, T b)
{
	if (a > b) return 1;
	if (a < b) return -1;
	if (a == b) return 0;
	else
	{
		assert(0);
		return 333;
	}
}


void Top_Result_Test()
{
	fstream in, out;
	in.open("Top_Result/input.txt");
	out.open("Top_Result/output.txt", ios::trunc | ios::out);

	drug a("myname1", "mymaker1", "myactivesubstance1", "tablet",	101, 1001);
	drug b("myname2", "mymaker2", "myactivesubstance2", "syrup",	102, 1002);
	drug c("myname3", "mymaker3", "myactivesubstance3", "solution", 103, 1003);
	drug d("myname4", "mymaker4", "myactivesubstance4", "ointment", 104, 1004);
	drug e("myname5", "mymaker5", "myactivesubstance5", "gel",		105, 1005);
	drug f("myname6", "mymaker6", "myactivesubstance6", "pills",	106, 1006);
	drug g("myname7", "mymaker7", "myactivesubstance7", "powder",	107, 1007);
	drug h("myname8", "mymaker8", "myactivesubstance8", "capsule",	108, 1008);

	top_result <double, drug, Compare> mr(5);

	mr.push(a, a.get_price());
	mr.push(b, b.get_price());
	mr.push(c, c.get_price());
	mr.push(d, d.get_price());
	mr.push(e, e.get_price());
	mr.push(f, f.get_price());
	mr.push(g, g.get_price());
	mr.push(h, h.get_price());

	out << mr;


	in.close();
	out.close();
	cout << "Hello. Top Result finished its work. NOT FINISHED. NO ITERATORS." << endl;
}
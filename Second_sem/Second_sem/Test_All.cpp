#include "HashTable\HT.h"
#include "Matrix\matrix.h"
#include "Quaternion\quaternion.h"
#include "LongArithmetic\bigint.h"
#include "Strings\strings.h"
#include "Pharmacy\pharmacy.h"
#include "Top_Result\topresult.h"
#include "ReverseOrder\ReverseOrder.h"
#include "Geometry\geometry.h"

int main()
{
	setlocale(LC_ALL, "Russian");

	matrixtest();
	quattest();
	hashtest();
	biginttest();
	StringTest();
	Pharmacy_Test();
	Top_Result_Test();
	Reverse_Test();
	Geometry_test();

	cout << "ÂÑ¨ ÃÎÒÎÂÎ." << endl;

	system("pause");
	return 0;
}
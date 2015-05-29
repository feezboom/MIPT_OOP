#include <fstream>
#include "matrix.h"


int matrixtest()
{

	fstream in, out;
	int n, m;
	in.open("Matrix/in.txt");
	if (!in)
	{
		cout << "File in.txt not found." << endl;
		return 1;
	}
	out.open("Matrix/out.txt", ios::trunc | ios::out);
	
	in >> n >> m;

	Matrix <double> a(n, m), b(n, m), c(n, m), d(n, m);
	
	in >> a >> b;
	out << a << b;

	c = a;

	out << a;
	a.Invert();
	out << a;

	out << a.geth() << endl;
	out << a.getl() << endl;

	out << d << endl;
	in >> n >> m;
	d.ReSize(n, m);
	out << d;

	d.Read(in);
	d.Print(out);

	d.AddStrs(d[0], 2, d[1], 1);
	out << d;

	d.ChangeStrs(d[0], d[1]);
	out << d;

	d.MulStr(d[1], 1.0 / 2);
	out << d;

	out << a;
	out << a.Det() << endl;
	out << a.Trace();
	
	a = c;

	out << a;
	a.Invert();
	out << a;

	a = c;
	out << a;

	c = a + b;
	out << c;
	c = a - b;
	out << c;
	c = a * b;
	out << c;
	c = a / b;
	out << c << b;
	out << c[0][0] << endl;

	out << b.Det() << endl;

	in.close();
	out.close();

	cout << "Everything in matrices is OK\n";
	return 0;
}
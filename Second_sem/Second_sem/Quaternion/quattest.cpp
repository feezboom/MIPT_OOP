#include "quaternion.h"

int quattest()
{
	fstream in, out;
	in.open("Quaternion/in.txt");
	if (!in)
	{
		cout << "File in.txt not found." << endl;
		return 1;
	}
	out.open("Quaternion/out.txt", ios::trunc | ios::out);

	quat <double> a, b, c, d, e, f;
	double h, i, j, k;
	in >> a >> b >> c >> d >> e >> f;
	out << a << endl << b << endl << c << endl << d << endl << e << endl << f << endl;

	out << a + 5 << endl;
	out << a - 5 << endl;
	out << a * 5 << endl;
	out << a / 5 << endl;

	out << endl;

	out << a + b << endl;
	out << a - b << endl;
	out << a * b << endl;
	out << a / b << endl;

	a += b;
	a -= b;
	a *= b;
	a /= b;

	out << a << endl;
	out << b << endl;

	out << endl;

	a.update(1, 2, 3, 4);
	a.getcrd(h, i, j, k);
	out << h << i << j << k << endl;

	out << endl;

	Matrix <double> matrix(QUATDIMENSION, QUATDIMENSION);
	matrix = a.matrixinit();
	b.initbymatrix(matrix);
	out << matrix << endl;
	out << a << endl;
	out << b << endl;

	out << endl;

	out << a.norm() << endl;
	out << a.abs() << endl;
	out << a.scalar(b) << endl;
	
	out << endl;

	out << a.conjugate() << endl;
	out << a.invert() << endl;
	a.update(1, 2, 3, 4);
	b.update(6, 7, 8, 9);
	out << a.vectmul(b) << endl;

	a.update(0, 0, 0, 0);
	out << a << endl;
	a.print(out);

	cout << "Everything in quaternioins is OK.\n";
	in.close();
	out.close();
	return 0;
}
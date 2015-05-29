#include "geometry.h"
#include "figures.h"

int Geometry_test()
{
	Point	
		a0(0, 0), a1(1, 0), a2(0, 1), a3(1, 1),
		a23(5, 5), a14(1, 6), a13(1, 5), a4(12, 8),
		a22(4, 4), a15(6, 3), a12(2, 6), a5(13, 9),
		a21(3, 3), a16(1, 2), a11(4, 7), a6(14, 10),
		a20(2, 2), a17(4, 0), a10(5, 8), a7(15, 11),
		a19(1, 0.5), a18(7, 8), a9(6, 9), a8(16, 12);
	Segment
		b0(a0, a1),  b1(a1, a0),  b2(a1, a0),  b3(a1, a0),
		b23(a1, a0), b14(a1, a0), b13(a1, a0), b4(a1, a0),
		b22(a1, a0), b15(a1, a0), b12(a1, a0), b5(a1, a0),
		b21(a1, a0), b16(a1, a0), b11(a1, a0), b6(a1, a0),
		b20(a1, a0), b17(a1, a0), b10(a1, a0), b7(a1, a0),
		b19(a1, a0), b18(a1, a0), b9(a1, a0),  b8(a1, a0);

	Triangle tr(a0, a1, a2);
	Rectangle(a3, a4, a5, a6);
	Circle(a7, 10);
	Square(a8, a9, a10, a11);

	std::vector <Point> points;
	points.push_back(a12);
	points.push_back(a13);
	points.push_back(a14);
	points.push_back(a15);
	points.push_back(a16);

	Polygon pol(points);


	std::cout << "Geometry is DONE OK.\n";
	return 0;
}
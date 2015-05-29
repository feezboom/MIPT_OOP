#include "figures.h"

double Rectangle::get_square()
{
	return pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2);
}
bool Rectangle::belong_divide(const Point& p)
{
	Segment s1(p1, p2);
	Segment s2(p2, p3);
	Segment s3(p3, p4);
	Segment s4(p4, p1);

	return s1.belong(p) || s2.belong(p) || s3.belong(p) || s4.belong(p);
}
bool Rectangle::belong_figure(const Point& p)
{
	if (belong_divide(p))
		return false;

	Segment s1(p1, p2);
	Segment s2(p2, p3);
	Segment s3(p3, p4);
	Segment s4(p4, p1);

	int s = s1.what_side(p) + s2.what_side(p) + s3.what_side(p) + s4.what_side(p);
	return s == 0;
}
Point Rectangle::intersect(const Segment& S)
{
	Point O;
	Segment s1(p1, p2);
	Segment s2(p2, p3);
	Segment s3(p3, p4);
	Segment s4(p4, p1);

	try
	{
		Point O;
		s1.intersect(S, O);
		return O;
	}
	catch (std::exception& ex) { std::cerr << ex.what(); }

	try
	{
		Point O;
		s2.intersect(S, O);
		return O;
	}
	catch (std::exception& ex) { std::cerr << ex.what(); }

	try
	{
		Point O;
		s3.intersect(S, O);
		return O;
	}
	catch (std::exception& ex) { std::cerr << ex.what(); }

	try
	{
		Point O;
		s4.intersect(S, O);
		return O;
	}
	catch (std::exception& ex) { std::cerr << ex.what(); }
	return O;
}

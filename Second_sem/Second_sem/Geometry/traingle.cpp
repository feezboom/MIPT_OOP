#include "figures.h"

double Triangle::get_square()
{
    Vector v1(p2.x - p1.x, p2.y - p1.y);
    Vector v2(p3.x - p1.x, p3.y - p1.y);

    return std::abs(v1.a * v2.b - v1.b * v2.a);
}
bool Triangle::belong_divide(const Point& p)
{
    Segment s1(p1, p2);
    Segment s2(p2, p3);
    Segment s3(p1, p3);

    return s1.belong(p) || s2.belong(p) || s3.belong(p);
}
bool Triangle::belong_figure(const Point& p)
{
	Point O;

	return true;
}
Point Triangle::intersect(const Segment& S)
{
    Segment s1(p1, p2);
    Segment s2(p2, p3);
    Segment s3(p1, p3);

    try
    {
		Point O;
		s1.intersect(S, O);
        return O;
    }
    catch (std::exception& ex) 
	{
		std::cerr << ex.what();
	}

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

    throw ("Segment don't intersect triangle");
}

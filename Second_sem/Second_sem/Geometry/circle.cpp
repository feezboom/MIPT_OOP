#include "figures.h"

double Circle::get_square()
{
    return M_PI * radius * radius;
}
bool Circle::belong_divide(const Point& p)
{
    return std::abs(sqrt(pow(p.x - center.x, 2) + pow(p.y - center.y, 2)) - radius) < eps;
}
bool Circle::belong_figure(const Point& p)
{
    return sqrt(pow(p.x - center.x, 2) + pow(p.y - center.y, 2)) < radius;
}
Point Circle::intersect(const Segment& S)
{
	Point O;
    double a = S.B.y - S.A.y;
    double b = S.A.x - S.B.x;
    double c = S.A.y * S.B.x - S.A.x * S.B.y;

    Vector normal(a, b);



	return O;
}
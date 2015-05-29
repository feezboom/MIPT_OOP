#include "figures.h"

double Polygon::get_square()
{
    double s = 0;

    for (unsigned int i = 1; i < verticies.size(); i++)
        s += (verticies[i - 1].y + verticies[i].y) * (verticies[i].x - verticies[i - 1].x);
    return s / 2;
}
bool Polygon::belong_divide(const Point& p)
{
    for (unsigned int i = 1; i < verticies.size(); i++)
    {
        Segment edge(verticies[i - 1], verticies[i]);
        if (edge.belong(p))
            return true;
    }
    return false;
}
bool Polygon::belong_figure(const Point& p)
{
    unsigned int intersect = 0;

    if (belong_divide(p))
        return false;
    for (unsigned int i = 1; i < verticies.size(); i++)
    {
        Segment edge(verticies[i - 1], verticies[i]);
        double a = edge.B.y - edge.A.y;
        double b = edge.A.x - edge.B.x;
        double c = edge.A.y * edge.B.x - edge.B.y * edge.A.x;
        Point q(-(c + b * p.y) / a, p.y);

        if (edge.belong(q))
            intersect++;
    }

    return intersect % 2;
}
Point Polygon::intersect(const Segment& S)
{
	Point O;
        for (unsigned int i = 1; i < verticies.size(); i++)
        {
            try
            {
                Segment edge(verticies[i - 1], verticies[i]);
				edge.intersect(S, O);
            }
			catch (std::exception& ex) { std::cerr << ex.what(); }
        }
		return O;
}
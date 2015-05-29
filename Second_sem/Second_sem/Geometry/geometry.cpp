#include "geometry.h"

std::istream& operator >>(std::istream& in, Point& p)
{
    in >> p.x;
    in >> p.y;
    return in;
}
std::istream& operator >>(std::istream& in, Vector& v)
{
    in >> v.a;
    in >> v.b;
    return in;
}
std::ostream& operator <<(std::ostream& out, const Point& p)
{
    out << "(" << p.x << ", " << p.y << ")" << std::endl;
    return out;
}
std::ostream& operator <<(std::ostream& out, const Vector& v)
{
    out << "{" << v.a << ", " << v.b << "}" << std::endl;
    return out;
}
void Vector::reverse_()
{
    a = -a;
    b = -b;
}
bool Segment::intersect(const Segment& S, Point& O) const
{
    double a1 = B.y - A.y;
    double b1 = A.x - B.x;
    double c1 = A.y * B.x - A.x * B.y;

    double a2 = S.B.y - S.A.y;
    double b2 = S.A.x - S.B.x;
    double c2 = S.A.y * S.B.x - S.A.x * S.B.y;

    if (std::abs(a1 * b2 - a2 * b1) < eps)
    {
        if (std::abs(a1 * c2 - a2 * c1) < eps)
        {
            if (belong(S.A))
            {
                O = S.A;
                return true;
            }
            if (belong(S.B))
            {
                O = S.B;
                return true;
            }
            if (S.belong(A))
            {
                O = A;
                return true;
            }
            if (S.belong(B))
            {
                O = B;
                return true;
            }
        }
        else
            return false;
    }

    Point O1((b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1), (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1));
    if ((belong(O1)) && (S.belong(O1)))
    {
        O = O1;
        return true;
    }
    return false;
}
Vector Segment::get_vector() const
{
    return Vector(B.x - A.x, B.y - A.y);
}
bool Segment::belong(const Point& p) const
{
    double a = B.y - A.y;
    double b = A.x - B.x;
    double c = A.y * B.x - A.x * B.y;

    return ((std::abs(a * p.x + b * p.y + c) < eps) && (p.x < max(A.x, B.x) + eps) && (p.x > min(A.x, B.x) - eps)
             && (p.y < max(A.y, B.y) + eps) && (p.y > min(A.y, B.y)) - eps);
}
SideSegment Segment::what_side(const Point& p) const
{
    if (belong(p))
        return SSbelong;

    double a = B.y - A.y;
    double b = A.x - B.x;
    double c = A.y * B.x - A.x * B.y;

    if (a * p.x + b * p.y + c >= eps)
        return SSright;
    else if (a * p.x + b * p.y + c <= -eps)
            return SSleft;
    return ((p.x > max(A.x, B.x))? SSahead : SSbehind);
}

#pragma once

#include <iostream>
#include "../functions.h"
#include <exception>
#include <cmath>
#include <math.h>

class Vector;
class Segment;

static const double eps = 0.00000001;

/*
Напишите набор классов для планиметрии :

++	Point(точка)
++	Segment(отрезок)
++	Vector(вектор)

Предусмотрите методы для :
++	нахождения пересечения отрезков
++	определения вектора, соответствующего отрезку
++	проверки принадлежности точки отрезку
++	обращения вектора
++	определения, с какой стороны от отрезку лежит точка
*/

enum SideSegment
{
    SSleft,
    SSright,
    SSbelong,
    SSahead,
    SSbehind
};
class Point
{
private:
    double x;
    double y;
public:

	Point() : x(0), y(0) {};
	Point(double x_, double y_) : x(x_), y(y_) {};
	Point(const Point& p) : x(p.x), y(p.y) {};

    friend std::istream& operator >>(std::istream&, Point&);
    friend std::ostream& operator <<(std::ostream&, const Point&);

    friend class Vector;
    friend class Segment;

	friend class Shape;
	friend class Triangle;
	friend class Rectangle;
	friend class Square;
	friend class Polygon;
	friend class Circle;
	friend class ConvexPolygon;
};
class Vector
{
private:
    double a;
    double b;
public:
    Vector() {}
    Vector(double a_, double b_): a(a_), b(b_) {}
    Vector(const Vector& v): a(v.a), b(v.b) {}

    void reverse_();

    friend std::istream& operator >>(std::istream&, Vector&);
    friend std::ostream& operator <<(std::ostream&, const Vector&);

	friend class Shape;
	friend class Triangle;
	friend class Rectangle;
	friend class Square;
	friend class Polygon;
	friend class Circle;
	friend class ConvexPolygon;
};
class Segment
{
private:
    Point A;
    Point B;
public:
    Segment() {}
    Segment(Point a, Point b): A(a), B(b) {}
    Segment(const Segment& S): A(S.A), B(S.B) {}

	bool intersect(const Segment&, Point&) const;
	Vector get_vector() const;
	bool belong(const Point&) const;
	SideSegment what_side(const Point&) const;

	friend class Shape;
	friend class Triangle;
	friend class Rectangle;
	friend class Square;
	friend class Polygon;
	friend class Circle;
	friend class ConvexPolygon;
};
int Geometry_test();
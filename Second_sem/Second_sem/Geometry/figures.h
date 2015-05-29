#pragma once

#include "geometry.h"
#include <vector>

static const long double M_PI = 3.1415926535897932384626433832795;

/*
Описание

Напишите набор классов для представления геометрических фигур на плоскости:

	Окружность
	Многоугольник
	Выпуклый многоугольник
	Прямоугольник
	Квадрат
	Треугольник
Эти классы необходимо отнаследовать от базового класса фигуры (Shape), в котором определить виртуальные методы для:
	нахождения площади фигуры
	проверки попадания точки на границу фигуры
	проверки принадлежности точки фигуре
	нахождения точки пересечения фигуры и отрезка
*/

class Shape
{
public:
    Shape() {}

	virtual double get_square()=0;
	virtual bool belong_divide(const Point&)=0;
    virtual bool belong_figure(const Point&)=0;
    virtual Point intersect(const Segment&)=0;
};
class Circle: public Shape
{
private:
    Point center;
    double radius;
public:
    Circle(): center(0, 0), radius(0) {}
	Circle(const Point& O, const double R) : center(O), radius(R){};
	Circle(const double x, const double y, const double R) : center(x,y), radius(R){}

    virtual double get_square() override;
	virtual bool belong_divide(const Point&) override;
	virtual bool belong_figure(const Point&) override;
	virtual Point intersect(const Segment&) override;
};
class Polygon: public Shape
{
private:
    std::vector <Point> verticies;
public:
	Polygon(){};
	Polygon(const std::vector <Point>& q) : verticies(q){};

	virtual double get_square() override;
	virtual bool belong_divide(const Point&) override;
	virtual bool belong_figure(const Point&) override;
	virtual Point intersect(const Segment&) override;
};
class Rectangle: public Shape
{
protected:
    Point p1;
    Point p2;
    Point p3;
    Point p4;
public:
	Rectangle(){};
    Rectangle(const Point& a, const Point& b, const Point& c, const Point& d): p1(a), p2(b), p3(c), p4(d) {}

	virtual double get_square() override;
	virtual bool belong_divide(const Point&) override;
	virtual bool belong_figure(const Point&) override;
	virtual Point intersect(const Segment&) override;
	friend class Square;
};
class Square: public Shape
{
private:
	Point p1;
	Point p2;
	Point p3;
	Point p4;
public:
	Square(const Point& a, const Point& b, const Point& c, const Point& d) : p1(a), p2(b), p3(c), p4(d) {}

	virtual double get_square() override;
	virtual bool belong_divide(const Point&) override;
	virtual bool belong_figure(const Point&) override;
	virtual Point intersect(const Segment&) override;
};
class Triangle: public Shape
{
private:
    Point p1;
    Point p2;
    Point p3;
public:
	Triangle() {}
	Triangle(const Point& a, const Point& b, const Point& c) : p1(a), p2(b), p3(c) {};

	virtual double get_square() override;
	virtual bool belong_divide(const Point&) override;
	virtual bool belong_figure(const Point&) override;
	virtual Point intersect(const Segment&) override;
};
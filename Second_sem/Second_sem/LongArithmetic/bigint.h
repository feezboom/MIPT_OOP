#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

#define MYBASE 10000
#define MYBLOCKSIZE 4

typedef _int64 myint;

class bigint
{
private:
	vector <myint> number;							//Хранится с конца блоками размером BLOCKSIZE
	//По сути хранение числа в основании BASE
	//	vector <myint> fraction;					//Дробная часть длинного числа
	int sign;										//Длина длинного числа
	int length;										//Знак длинного числа

	myint BASE;										//Что нибудь вроде десяти в какой-нибудь степени в пределах разумного
	int BLOCKSIZE;									//Как раз степень десятки

public:

	bigint();
	bigint(myint l);
	void push_back(myint t);
	void clear();
	size_t size() const;
	bigint Abs() const;
	bigint convert(myint a) const;						//Конвертирует число int a в bigint а
	myint getint();

	bigint sum(bigint n) const;
	bigint subtraction(const bigint n) const;

	bool operator==(bigint n) const;
	bool operator==(myint n) const;
	bool operator>(bigint n) const;
	bool operator>(myint n) const;
	bool operator>=(bigint n) const;
	bool operator>=(myint n) const;
	bool operator<(bigint n) const;
	bool operator<(myint n) const;
	bool operator<=(bigint n) const;
	bool operator<=(myint n) const;

	int initlength();
	friend istream& operator>>(istream& in, bigint &a);
	friend ostream& operator<<(ostream& out, bigint &a);
	void correct();

	bigint operator+(myint c) const;
	bigint operator+(bigint n) const;

	bigint operator-(myint c) const;
	bigint operator-(bigint n) const;

	bigint operator=(bigint n);
	bigint operator=(myint a);

	bigint operator*(myint c) const;
	bigint operator*(bigint n) const;

	myint& operator[](size_t i);

	bigint operator/(myint c) const;
	bigint operator/(bigint n) const;						//Same as div, division by integers, works faster?
	bigint div(bigint n);									//*this = (*this / n) целочисленно, возвращает остаток от деления

};

int biginttest();
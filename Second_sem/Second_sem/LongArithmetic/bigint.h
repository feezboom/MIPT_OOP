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
	vector <myint> number;							//�������� � ����� ������� �������� BLOCKSIZE
	//�� ���� �������� ����� � ��������� BASE
	//	vector <myint> fraction;					//������� ����� �������� �����
	int sign;										//����� �������� �����
	int length;										//���� �������� �����

	myint BASE;										//��� ������ ����� ������ � �����-������ ������� � �������� ���������
	int BLOCKSIZE;									//��� ��� ������� �������

public:

	bigint();
	bigint(myint l);
	void push_back(myint t);
	void clear();
	size_t size() const;
	bigint Abs() const;
	bigint convert(myint a) const;						//������������ ����� int a � bigint �
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
	bigint div(bigint n);									//*this = (*this / n) ������������, ���������� ������� �� �������

};

int biginttest();
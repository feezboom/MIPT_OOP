#pragma once

#include <iostream>
#include <iomanip>
#include "../Matrix/matrix.h"

#define QUATDIMENSION 4

using namespace std;

// al:
// ��������� ������
template <class T>
class quat																			//��������� ����� �����������
{
private:
	T a, b, c, d;																	//���������� �����������
public:
	quat(T a0 = NULL, T b0 = NULL, T c0 = NULL, T d0 = NULL);						//����������� �����������
	quat <T> update(T a0, T b0, T c0, T d0);										//������������������� ����������
	void initbymatrix(Matrix <T> &rm);												//������������������� ���������� �������� rm
	void getcrd(T &r, T &i, T &j, T &k);											//�������� ���������� �����������
	
	Matrix <T> matrixinit();														//������� ������� �����������
	
	double norm();																	//������� ����� �����������
	double abs();																	//������� ������ �����������
	double scalar(quat <T> q);														//������� ��������� ������������ �� ���������� q
	
	quat <T> conjugate();															//������� ����������� ����������
	quat <T> invert();																//������� �������� ����������
	quat <T> vectmul(quat <T> q);													//������� ��������� ������������ �� ���������� q

	template <typename T> friend ostream& operator<< (ostream &out, quat <T> &q);	//����� ����������� � ����� out
	template <typename T> friend istream& operator>> (istream &in, quat <T> &q);	//���� ����������� � ����� in

	quat <T> operator+(quat <T> &q);												/*���������� ����������*/
	quat <T> operator+(int q);
	quat <T> operator-(quat <T> &q);												/*���������� ����������*/
	quat <T> operator-(int q);
	quat <T> operator*(quat <T> &q);												/*���������� ����������*/
	quat <T> operator*(int q);
	quat <T> operator/(quat <T> &q);												/*���������� ����������*/
	quat <T> operator/(int q);
	void operator+=(quat <T> &q);													/*���������� ����������*/
	void operator-=(quat <T> &q);													/*���������� ����������*/
	void operator*=(quat <T> &q);													/*���������� ����������*/
	void operator/=(quat <T> &q);													/*���������� ����������*/

	void print(ostream &out);														//������� ���������� � ����������� �����
	void printmatrix(ostream &out);													//������� ������� ����������� � ����������� �����
};

int quattest();
#include "quaternion.hpp"
#pragma once

#include <iostream>
#include <fstream>
#include <exception>

using namespace std;
template <class T>
class Matrix																				//��������� ����� �������			
{   
private:
	int length, high;																		//�������� ���������� ����� � �������� �������
	T **elements;																			//�������� ��������� �������
public:
	Matrix(int, int);																		//����������� �������
	Matrix(const Matrix&);
	int geth() const;																		//������� ���������� ����� �������
	int getl() const;																		//������� ���������� �������� �������

	Matrix <T> ReSize(int NewSizeH, int NewSizeL);												//�������������� ������� �������
	Matrix <T> Read(istream& f);																	//������� ������� �� ������ fstream
	void Print(ostream &out) const;															//������� ������� � ����������� �����

	void AddStrs(T* str1, T multiplier1, T* str2, T multiplier2);							//��������� � ������ str1 ���������� �� m1 ������ str2 ���������� �� m2 
	void ChangeStrs(T* str1, T* str2);														//�������� ������ str1 � str2 �������
	void MulStr(T* str, double multiplier);													//�������� ������ str �� ����� multiplier

	double Det();																			//������� ����������� �������
	T Trace() const;																		//������� ���� �������

	Matrix <T> Invert();																	//�������� �������
	Matrix <T> Transpose();																	//��������������� �������
	double Diagonilize();																	//����������������� ������� � ������� �����������

	Matrix <T>	operator+ (const Matrix <T> &p) const;										/*���������� ����������*/
	Matrix <T>	operator- (const Matrix <T> &p) const;										/*���������� ����������*/
	Matrix <T>	operator* (const Matrix <T> &p) const;										/*���������� ����������*/
	Matrix <T>  operator/ (const Matrix <T> &p) const;										/*���������� ����������*/
	Matrix <T>	operator= (const Matrix <T> &p);											/*���������� ����������*/
	T* operator[](int i) const;																/*���������� ����������*/
	

	template <typename T> friend ostream& operator<< (ostream& out,const Matrix <T> &matrix);	//����� ������� � ����� out
	template <typename T> friend istream& operator>> (istream& in, Matrix <T> &matrix);			//����� ������� � ����� in

	~Matrix();
};

int matrixtest();

#include "matrix.hpp"
#pragma once

#include <iostream>
#include <fstream>
#include <exception>

using namespace std;
template <class T>
class Matrix																				//Шаблонный класс матрицы			
{   
private:
	int length, high;																		//Хранение количества строк и столбцов матрицы
	T **elements;																			//Хранение элементов матрицы
public:
	Matrix(int, int);																		//Конструктор матрицы
	Matrix(const Matrix&);
	int geth() const;																		//Вернуть количество строк матрицы
	int getl() const;																		//Вернуть количество столбцов матрицы

	Matrix <T> ReSize(int NewSizeH, int NewSizeL);												//Переопределить размеры матрицы
	Matrix <T> Read(istream& f);																	//Считать матрицу из потока fstream
	void Print(ostream &out) const;															//Вывести матрицу в стандартный поток

	void AddStrs(T* str1, T multiplier1, T* str2, T multiplier2);							//Прибавить к строке str1 умноженной на m1 строку str2 умноженную на m2 
	void ChangeStrs(T* str1, T* str2);														//Поменять строки str1 и str2 местами
	void MulStr(T* str, double multiplier);													//Умножить строку str на число multiplier

	double Det();																			//Вернуть детерминант матрицы
	T Trace() const;																		//Вернуть след матрицы

	Matrix <T> Invert();																	//Обратить матрицу
	Matrix <T> Transpose();																	//Транспонировать матрицу
	double Diagonilize();																	//Диагонализировать матрицу и вернуть детерминант

	Matrix <T>	operator+ (const Matrix <T> &p) const;										/*Перегрузка операторов*/
	Matrix <T>	operator- (const Matrix <T> &p) const;										/*Перегрузка операторов*/
	Matrix <T>	operator* (const Matrix <T> &p) const;										/*Перегрузка операторов*/
	Matrix <T>  operator/ (const Matrix <T> &p) const;										/*Перегрузка операторов*/
	Matrix <T>	operator= (const Matrix <T> &p);											/*Перегрузка операторов*/
	T* operator[](int i) const;																/*Перегрузка операторов*/
	

	template <typename T> friend ostream& operator<< (ostream& out,const Matrix <T> &matrix);	//Вывод матрицы в поток out
	template <typename T> friend istream& operator>> (istream& in, Matrix <T> &matrix);			//Вывод матрицы в поток in

	~Matrix();
};

int matrixtest();

#include "matrix.hpp"
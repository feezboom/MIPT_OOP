#pragma once

#include <iostream>
#include <iomanip>
#include "../Matrix/matrix.h"

#define QUATDIMENSION 4

using namespace std;

// al:
// копипаста убрать
template <class T>
class quat																			//Шаблонный класс кватерниона
{
private:
	T a, b, c, d;																	//Координаты кватерниона
public:
	quat(T a0 = NULL, T b0 = NULL, T c0 = NULL, T d0 = NULL);						//Конструктор кватерниона
	quat <T> update(T a0, T b0, T c0, T d0);										//Проинициализировать кватернион
	void initbymatrix(Matrix <T> &rm);												//Проинициализировать кватернион матрицей rm
	void getcrd(T &r, T &i, T &j, T &k);											//Получить координаты кватерниона
	
	Matrix <T> matrixinit();														//Вернуть матрицу кватерниона
	
	double norm();																	//Вернуть норму кватерниона
	double abs();																	//Вернуть модуль кватерниона
	double scalar(quat <T> q);														//Вернуть скалярное произведение на кватернион q
	
	quat <T> conjugate();															//Вернуть сопряженный кватернион
	quat <T> invert();																//Вернуть обратный кватернион
	quat <T> vectmul(quat <T> q);													//Вернуть векторное произведение на кватернион q

	template <typename T> friend ostream& operator<< (ostream &out, quat <T> &q);	//Вывод кватерниона в поток out
	template <typename T> friend istream& operator>> (istream &in, quat <T> &q);	//Ввод кватерниона в поток in

	quat <T> operator+(quat <T> &q);												/*Перегрузки операторов*/
	quat <T> operator+(int q);
	quat <T> operator-(quat <T> &q);												/*Перегрузки операторов*/
	quat <T> operator-(int q);
	quat <T> operator*(quat <T> &q);												/*Перегрузки операторов*/
	quat <T> operator*(int q);
	quat <T> operator/(quat <T> &q);												/*Перегрузки операторов*/
	quat <T> operator/(int q);
	void operator+=(quat <T> &q);													/*Перегрузки операторов*/
	void operator-=(quat <T> &q);													/*Перегрузки операторов*/
	void operator*=(quat <T> &q);													/*Перегрузки операторов*/
	void operator/=(quat <T> &q);													/*Перегрузки операторов*/

	void print(ostream &out);														//Вывести кватернион в стандартный поток
	void printmatrix(ostream &out);													//Вывести матрицу кватерниона в стандартный поток
};

int quattest();
#include "quaternion.hpp"
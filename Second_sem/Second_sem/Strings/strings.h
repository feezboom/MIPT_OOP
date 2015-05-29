#pragma once

#include "../functions.h"
#include <vector>
#include <iostream>
#include <assert.h>
#include <string>

using namespace std;

#define MAX_STRING_SIZE 255

/*	
Напишите класс строки.
Предусмотрите возможность 

++	ввода-вывода из потока,
++	обращения по индексу,
++	конструктора из char*, std::string, 
++	операторов присваивания, 
++	сравнения, 
++	сложения, 
??	обращения к внутреннему содержимому, 
++	вставки в середину строки, 
++	удаления из середины, 
++	вставки в конец, 
++	удаления последней буквы, 
??	умному выделению и освобождению памяти.
*/

class line
{
private:
	vector <char> str;
public:

	line(){}
	line(char* input)
	{
		*this = input;
	}
	line(string input)
	{
		*this = input;
	}

	void clear()
	{
		str.clear();
	}
	void push_back(char in)
	{
		str.push_back(in);
	}
	void pop_back()
	{
		str.pop_back();
	}

	size_t size()const
	{
		return str.size();
	}

	char& operator[](size_t i)
	{
		if (i < size() && i >= 0) 
			return str[i];
		cout << "Wrong index : operator[]" << endl;
		system("pause");
		exit(2);
	}
	char operator[](size_t i) const
	{
		return str[i];
	}
	line operator=(line input)
	{
		clear();
		for (size_t i = 0; i < input.size(); ++i)
			push_back(input[i]);
		return *this;
	}
	line operator=(char* input)
	{
		clear(); int i = 0;
		while (input[i++] != '\0')
		{
			assert(i <= MAX_STRING_SIZE);
			push_back(input[i - 1]);
		}
		return *this;
	}
	line operator=(string input)
	{
		clear();
		for (size_t i = 0; i < input.size(); ++i)
			push_back(input[i]);
		return *this;
	}
	bool operator>(line input)const
	{
		int i, length = min(size(), input.size());
		for (i = 0; i < length && str[i] == input[i]; ++i);

		assert(i <= length);

		if (i == length)
			return size() > input.size() ? true : false;
		else
			return str[i] > input[i] ? true : false;
	}
	bool operator<(line input)const
	{
		int i, length = min(size(), input.size());
		for (i = 0; i < length && str[i] == input[i]; ++i);

		assert(i <= length);

		if (i == length)
			return size() < input.size() ? true : false;
		else
			return str[i] < input[i] ? true : false;
	}
	bool operator>=(line input)const
	{
		return (*this<input) ? false : true;
	}
	bool operator<=(line input)const
	{
		return (*this>input) ? false : true;
	}
	bool operator==(line input)const
	{
		if (size() != input.size()) return false; 
		
		size_t i;
		for (i = 0; i < size() && str[i] == input[i]; ++i);

		assert(i <= size());

		if (i == size())
			return true;
		else
			return false;
	}
	bool operator==(char* input)const
	{
		line temp = input;
		return (temp == *this) ? true : false;
	}
	bool operator!=(line input)const
	{
		return *this == input ? false : true;
	}
	bool operator!=(char* input)const
	{
		return *this == input ? false : true;
	}
	
	line operator+(line input)const
	{
		line ans(*this);
		for (size_t i = 0; i < input.size(); ++i)
			ans.push_back(input[i]);
		return ans;
	}
	line insert(line input, size_t position)
	{
		if (position > MAX_STRING_SIZE || position >= size()) return *this;
		vector <char> ::iterator it, inputit;
	
		it = str.begin() + position;
		inputit = input.str.end() - 1;

		while (inputit - 1 != input.str.begin())
		{
			str.insert(it = str.begin() + position, *inputit);
			inputit--;
		}
		return *this;
	}
	line erase(size_t position, size_t number)
	{
		if (position > MAX_STRING_SIZE || position >= size())
			return *this;
		vector<char> :: iterator it = str.begin() + position;
		if (position + number >= size()) number = size() - position;
		str.erase(it, it + number);
		return *this;
	}
	line insert_back(line input)
	{
		for (size_t i = 0; i < input.size(); ++i)
			push_back(input[i]);
		return *this;
	}
	bool is_subs(line input)const
	{
		if (input.size() > size()) return 0;
		size_t i;
		for (i = 0; i < input.size() && input[i] == str[i]; ++i);
		if (i == input.size()) 
			return 1;
		else 
			return 0;
	}

	friend istream& operator>> (istream& in, line &q);
	friend ostream& operator<< (ostream& out, line &q);

	~line(){};
};

void StringTest();

#pragma once

#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

/*
	Реализуйте шаблонный класс TopResult, 
который представляет собой контейнер для хранения k самых больших значений из добавляемых в него.
Шаблонными параметрами являются ключ K, по которому происходит упорядочивание элементов, 
ассоциированное с ним значение V, и функция сравнения элементов K.
		-Размер топа передается в конструкторе класса.
Должны быть реализованы операции:
		-добавления пары <K,V>, 
		-получения первого и последнего элементов. 
Также необходимо реализовать 
		-итераторы для обхода элементов топа в порядке убывания (прямой порядок) и возрастания (обратный порядок) элементов топа.*/

template <typename K, typename V, int (Compare)(K a, K b)>
class top_result;


template <typename K, typename V, int (Compare)(K a, K b)>
class top_result
{
private:
	vector <V> unitsV;
	vector <K> keys;

	size_t size;
	size_t FindMin()
	{
		K min = keys[0];
		size_t Min_Index = 0;
		for (size_t i = 0; i < count(); ++i)
		{
			if (Compare(min, keys[i]) >= 0)
			{
				min = keys[i];
				Min_Index = i;
			}
		}
		return Min_Index;
	}
	void sort()
	{
		assert(unitsV.size() == keys.size());
		for (size_t i = 0; i < unitsV.size() - 1; ++i)
		{
			for (size_t j = i + 1; j < unitsV.size(); ++j)
			if (Compare(keys[i], keys[j]) == 1)
			{
				swap(keys[i], keys[j]);
				swap(unitsV[i], unitsV[j]);
			}
		}
	}
public:
	class iterator
	{
		struct node
		{
			node* previous;
			node* next;
			V* value;
		};
		node* current;
	private:
		V* operator++()
		{
			current = current->next;
			return current->value;
		}
		V* operator--()
		{
			current = current->previous;
			return current->value;
		}
	};
	top_result()
	{
		size = 1;
	}
	top_result(size_t _size)
	{
		size = _size;
	}


	void set_size(size_t new_size)
	{
		if (new_size >= 0 && new_size <= INT_MAX);
		size = new_size;
		while (unitsV.size() > size)
		{
			unitsV.pop_back();
			keys.pop_back();
		}
		assert(units.size() == keys.size());
	}
	size_t count() const
	{
		assert(unitsV.size() == keys.size());
		return unitsV.size();
	}
	void push(V input, K key)
	{
		size_t count = unitsV.size();
		if (count < size)
		{
			unitsV.push_back(input);
			keys.push_back(key);
			count++;
		}
		else
		{
			size_t i = FindMin();
			if (Compare(key, keys[i]) == 1)
			{
				unitsV[i] = input;
				keys[i] = key;
			}
		}
		sort();
		assert(unitsV.size() == keys.size());

	}

	template <typename K1, typename V1, int (Compare)(K1 a, K1 b)>
	friend ostream& operator<<(ostream& out, top_result <K1, V1, Compare> my_top_result);
};

void Top_Result_Test();

#include "topresult.hpp"

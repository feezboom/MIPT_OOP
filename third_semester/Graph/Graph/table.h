#pragma once

template <class type>
class table{
private:
	type** matrix;
	size_t l, h;
public:
	table(size_t l, size_t h)
	{
		this->l = l; this->h = h;

		matrix = new type*[l];
		for (size_t i = 0; i < l; i++)
			matrix[i] = new type[h];
	}
	~table() {
		for (size_t i = 0; i < l; i++)
		{
			delete[] matrix[i];
		}
		delete[] matrix;
	}
};

template <class T>				Matrix <T> :: Matrix(int n = 3, int m = 3) : high(n), length(m)
{
	elements = new T*[high];
	for (int i = 0; i < high; ++i) elements[i] = new T[length];

	for (int i = 0; i < high; ++i)
	{ //initialization by zero's
		for (int j = 0; j < length; ++j)
			elements[i][j] = (i == j) ? 1 : NULL;
	}
}
template <class T>				Matrix <T> :: Matrix(const Matrix& matrix) : high(matrix.high), length(matrix.length)
{
	elements = new T*[high];
	for (int i = 0; i < high; ++i)
	{
		elements[i] = new T[length];
		for (int j = 0; j < length; ++j)
		{
			elements[i][j] = matrix[i][j];
		}
	}
}
template <class T> void			Matrix <T> :: Print(ostream &out) const
{
	out << endl;
	for (int i = 0; i < high; ++i)
	{
		for (int j = 0; j < length; ++j)
		{
			out.width(5); out.precision(3);
			out << elements[i][j] << " ";
		}
		out << endl;
	}
}
template <class T> Matrix <T>	Matrix <T> :: Read(istream& f)
{
	for (int i = 0; i < high; ++i){
		for (int j = 0; j < length; ++j)
			f >> elements[i][j];
	}
	return (*this);
}
template <class T> Matrix <T>	Matrix <T> :: operator+ (const Matrix <T> &p) const
{
	Matrix <T> result(high, length);
	if (length != p.length || high != p.high)
		cout << "Matrixes have wrong size. ";
	else
	{
		for (int i = 0; i < high; ++i){
			for (int j = 0; j < length; ++j)
				result.elements[i][j] = elements[i][j] + p.elements[i][j];
		}
	}
	return result;
}
template <class T> Matrix <T>	Matrix <T> :: operator- (const Matrix <T> &p) const
{
	Matrix <T> result(high, length);
	if (length != p.length || high != p.high)
		cout << "Matrixes have wrong size. ";
	else
	{
		for (int i = 0; i < high; ++i){
			for (int j = 0; j < length; ++j)
				result.elements[i][j] = elements[i][j] - p.elements[i][j];
		}
	}
	return result;
}
template <class T> Matrix <T>	Matrix <T> :: operator* (const Matrix <T> &p) const
{
	Matrix <T> result(high, p.length);
	if (length == p.high)
	{
		for (int i = 0; i < p.length; ++i){
			for (int j = 0; j < high; ++j){
				for (int k = 0; k < length; k++)
					result[i][j] = result[i][j] + elements[i][k] * p[k][j];
			}
		}
	}
	else cout << "Matrices have wrong size. ";
	return result;
}
template <class T> Matrix <T>	Matrix <T> :: operator/ (const Matrix <T> &p) const
{
	Matrix <T> res = p;
	res.Invert();
	res = (*this) * res;
	return res;
}
template <class T> Matrix <T>	Matrix <T> :: operator= (const Matrix <T> &p)
{
	ReSize(p.geth(), p.getl());
	for (int i = 0; i < high; ++i)
	{
		for (int j = 0; j < length; ++j)
			elements[i][j] = p[i][j];
	}
	return (*this);
}
template <class T> T*			Matrix <T> :: operator[](int i) const
{
	return elements[i];
}
template <class T> Matrix <T>	Matrix <T> :: ReSize(int NewSizeH, int NewSizeL){
	for (int i = 0; i < high; ++i) delete(elements[i]);//Deleting old matrix
	delete(elements);

	high = NewSizeH;
	length = NewSizeL;
	elements = new T*[high];
	for (int i = 0; i < high; ++i) elements[i] = new T[length];

	for (int i = 0; i < high; ++i){ //initialization by zero's
		for (int j = 0; j < length; ++j)
			elements[i][j] = NULL;
	}
	return (*this);
}
template <class T> Matrix <T>	Matrix <T> :: Transpose()
{
	if (high == length)
	{
		T** temp = new T*[high];	//allocating memory for the copy
		for (int i = 0; i < high; ++i) temp[i] = new T[length];

		for (int i = 0; i < high; ++i){	//Initializing by elements
			for (int j = 0; j < length; ++j)
				temp[i][j] = elements[i][j];
		}

		for (int i = 0; i < high; ++i){	//Transposing
			for (int j = 0; j < length; ++j) elements[i][j] = temp[j][i];
		}

		for (int i = 0; i < high; ++i) delete(temp[i]);	//freeing temp memory
		delete(temp);
	}
	else cout << "Matrix has wrong size. " << endl;
	return (*this);
}
template <class T> void			Matrix <T> :: AddStrs(T* str1, T multiplier1, T* str2, T multiplier2)
{
	for (int i = 0; i < length; ++i){
		str1[i] = multiplier1*str1[i] + multiplier2*str2[i];
	}
}
template <class T> void			Matrix <T> :: ChangeStrs(T* str1, T* str2)
{
	for (int i = 0; i < length; ++i)
	{
		T temp = str1[i];
		str1[i] = str2[i];
		str2[i] = temp;
	}
}
template <class T> void			Matrix <T> :: MulStr(T* str, double multiplier)
{
	for (int i = 0; i < length; ++i)
	{
		str[i] = ((double)str[i]) * multiplier;
	}
}
template <class T> Matrix <T>	Matrix <T> :: Invert()
{
	if (high == length)								//Делаем единичную
	{
		Matrix <T> result(high, length);
		for (int i = 0; i < high; ++i){
			for (int j = 0; j < length; ++j){
				result[i][j] = (i == j) ? 1 : 0;
			}
		}

		for (int j = 0; j < length; ++j) //Calculating inverted matrix
		{
			for (int i = 0; i < high; ++i)
			if (i != j && (*this)[i][j] != 0)
			{
				if ((*this)[j][j] == 0)
				{
					bool Ok = false;
					for (int k = j + 1; k < high && Ok == false; ++k){
						if ((*this)[k][j] != 0)
						{
							Ok = true;
							ChangeStrs((*this)[k], (*this)[j]);
							ChangeStrs(result[k], result[j]);
						}
					}
					if (Ok == false)
					{
						cout << "det == 0. No inverted matrix.";
						Matrix <T> err(high, length);
						return err;
					}
				}
				double m = ((double)(*this)[i][j] / (double)(*this)[j][j]);
				AddStrs((*this)[i], 1, (*this)[j], -1.0 * m);
				AddStrs(result[i], 1, result[j], -1.0 * m);
			}
		}

		for (int i = 0; i < high; ++i) MulStr(result[i], 1.0 / ((*this)[i][i]));
		*this = result;
	}
	else 
		cout << "This matrix hasn't got any inverted matrix. " << endl;
	return *this;
}
template <class T> double		Matrix <T> :: Det()
{
	if (high == length)
	{
		Matrix <T> temp = *this;
		return temp.Diagonilize();
	}
	else
	{
		cout << "Matrix has wrong size. " << endl;
		return NAN;
	}
}
template <class T> double		Matrix <T> :: Diagonilize()
{
	double det = 1;
	if (high == length)
	{
		Matrix <T> temp(high, length); temp = *this;
		for (int j = 0; j < length; ++j){
			for (int i = 0; i < high; ++i)
			if (i != j && temp[i][j] != 0)
			{
				if (temp[j][j] == 0)
				{
					bool Ok = false;
					for (int k = j + 1; k < high && Ok == false; ++k){
						if (temp[k][j] != 0)
						{
							Ok = true;
							ChangeStrs(temp[k], temp[j]);
							det *= -1;
						}
					}
					if (Ok == false)  
						det = 0;
				}
				AddStrs(temp[i], 1, temp[j], -1 * (temp[i][j] / temp[j][j]));
			}
		}
		for (int i = 0; i < length; ++i) 
			det *= temp[i][i];
	}
	else
	{
		cout << "This matrix isn't square." << endl;
		det = NAN;
	}
	return det;
}
template <class T> T			Matrix <T> :: Trace() const
{
	T trace = 0;
	if (length == high) for (int i = 0; i < high; ++i) trace += elements[i][i];
	else cout << "Matrix has wrong size. " << endl;
	return trace;
}
template <class T> int			Matrix <T> :: geth() const
{
	return high;
}
template <class T> int			Matrix <T> :: getl() const
{
	return length;
}
template <typename T>			ostream& operator<< (ostream& out, const Matrix <T> &matrix)
{
	out << endl;
	for (int i = 0; i < matrix.high; ++i)
	{
		for (int j = 0; j < matrix.length; ++j)
		{
			out.width(5); out.precision(3);
			out << matrix[i][j] << " ";
		}
		out << endl;
	}
	return out;
}
template <typename T>			istream& operator>> (istream& in, Matrix <T> &matrix)
{
	for (int i = 0; i < matrix.high; ++i){
		for (int j = 0; j < matrix.length; ++j)
			in >> matrix[i][j];
	}
	return in;
}
template <class T>				Matrix <T> :: ~Matrix()
{
	for (int i = 0; i < high; ++i)
		delete[] elements[i];
	delete[] elements;
}
#include "../functions.h"
#include "bigint.h"

bigint min(bigint a, bigint b)
{
	return (a < b ? a : b);
}
bigint max(bigint a, bigint b)
{
	return (a>b ? a : b);
}
myint intlen(myint a)
{
	if (a == 0) return 1;
	int i = 1;
	while (a != 0 && ++i)
		a /= 10;
	return i - 1;
}
bigint::bigint()
{
	sign = 1;
	BASE = MYBASE;
	BLOCKSIZE = MYBLOCKSIZE;
}
bigint::bigint(myint l)
{
	BASE = MYBASE;
	BLOCKSIZE = MYBLOCKSIZE;
	if (l == 0) push_back(0);
	if (l != 0) convert(max(l, -l));
	if (l < 0)
		sign = -1;
	else
		sign = 1;
	length = number.size();
}
void bigint::push_back(myint t)
{
	number.push_back(t);
}
void bigint::clear()
{
	number.clear();
}
size_t bigint::size() const
{
	return number.size();
}
bigint bigint::convert(myint a) const
{
	bigint ans(0);
	if (a == 0)
		return ans;
	else
		ans.clear();

	ans.sign = (a<0 ? -1 : 1);
	if (a < 0) a = -a;
	while (a > 0)
	{
		ans.push_back(a % BASE);
		a /= BASE;
	}
	return ans;
}
myint bigint::getint()
{
	myint ans = 0, multiplier = 1;
	size_t i = 0;
	while (i < size())
	{
		ans += number[i] * multiplier;
		multiplier *= BASE;
		i++;
	}
	return ans;
}
myint& bigint :: operator[](size_t i)
{
	return number[i];
}
//�����, ��������� copy/paste
void bigint::correct()					//������������ ������, ����� � ������ ����� ���� ������ �����, ������� �� ������ BLOCKSIZE ������
{
	/*
	* ������ pop_back ��� ���� �� ������ �������
	* ���������������� ������ ����� �������� � ������ ������
	*/


	size_t i;
	//���� � ������ ����� ����
	i = number.size() - 1;
	while (i != 0)
	{
		if (number[i] != 0)
			break;
		number.pop_back();
		i--;
	}
	//�����

	//����� ��������
	myint next = 0;
	for (i = 0; i < number.size(); ++i)
	{
		number[i] += next;
		next = number[i] / BASE;
		number[i] = number[i] % BASE;
	}
	if (next != 0)
	{
		number.push_back(next);
	}

	//���� � ������ ����� ����
	i = number.size();
	while (i != 0)
	{
		i--;
		if (number[i] == 0)
			number.pop_back();
		else
			break;
	}
	//�����
}
bigint bigint::Abs() const
{
	bigint ans = *this;
	ans.sign = 1;
	return ans;
}
bool bigint :: operator>(bigint n) const
{
	if (sign > n.sign) return true;
	if (sign < n.sign) return false;
	if (size() > n.size()) return true;
	if (size() < n.size()) return false;

	bigint a, b;
	a = *this; b = n;

	int i = size() - 1;
	while (i >= 0)
	{
		if (a[i] < b[i])
			return false;
		if (a[i] > b[i])
			return true;
		i--;
	}
	return false;								//���� ����� �����
}
bool bigint :: operator>(myint n) const
{
	return (*this > convert(n) ? true : false);
}
bool bigint :: operator>=(bigint n) const
{
	return (*this > n || *this == n ? true : false);
}
bool bigint :: operator>=(myint n) const
{
	return (*this > n || *this == n ? true : false);
}
bool bigint :: operator<(bigint n) const
{
	if (sign > n.sign) return false;
	if (sign < n.sign) return true;
	if (size() > n.size()) return false;
	if (size() < n.size()) return true;

	bigint a, b;
	a = *this; b = n;

	int i = size() - 1;
	while (i >= 0)
	{
		if (a[i] < b[i])
			return true;
		if (a[i] > b[i])
			return false;
		i--;
	}
	return false;								//���� ����� �����
}
bool bigint :: operator<(myint n) const
{
	return (*this < convert(n) ? true : false);
}
bool bigint :: operator<=(bigint n) const
{
	return (*this < n || *this == n ? true : false);
}
bool bigint :: operator<=(myint n) const
{
	return (*this < n || *this == n ? true : false);
}
bool bigint :: operator==(bigint n) const
{
	if (!(*this > n) && !(*this < n))
		return true;
	else
		return false;
}
bool bigint :: operator==(myint n) const
{
	return (*this == convert(n));
}
bigint bigint :: operator=(bigint n)
{
	sign = n.sign;
	number = n.number;
	return (*this);
}
bigint bigint :: operator=(myint a)
{
	return (*this = convert(a));
}
bigint bigint :: sum(bigint n) const					//������������ ����� ������� �� ������ ����
{
	bigint ans, tempmin, tempmax;
	tempmin = min(*this, n);								//������ ��� [] �� �������� � ������������ ���������
	tempmax = max(*this, n);
	for (size_t i = 0; i < tempmax.size(); ++i)
	{
		if (i < tempmin.size())
			ans.push_back(tempmax[i] + tempmin[i]);
		else
			ans.push_back(tempmax[i]);
	}
	ans.sign = 1;
	ans.correct();											//��������, ���������, �� ��� ������ ���������
	return ans;
}
bigint bigint :: subtraction(bigint n) const	//�������������, ��� *this ������ ��� n
{
	bigint ans, minuend, subtrahend;
	minuend = *this;
	subtrahend = n;
	for (size_t i = 0; i < size(); ++i)
	{
		if (i < n.size())
			ans.push_back(minuend[i] - subtrahend[i]);
		else
			ans.push_back(minuend[i]);
		if (ans[i] < 0)
		{
			ans[i] += BASE;
			minuend[i + 1] -= 1;
		}
	}
	ans.correct();
	return ans;
}
bigint bigint :: operator+(myint c) const
{
	return *this + convert(c);
}
bigint bigint :: operator+(bigint n) const
{
	bigint ans;
	/*
	����� ���������� ������ �������, � ������:
	- ��������������� ����� �����. ���� ��� ������, �� ������ �������� �������, ���� ���������� - �����
	- ��������������� ������ �����. ���� ����� ����� ����� ����������� �� ������ �����.
	� ����� ��������� ����������� ������ ������.
	������, ������� ����, �� ���� ����������� ��������� �������.
	*/

	if (Abs() >= n.Abs())					//��������� �������
	{
		if (sign == n.sign)					//���� ����� �������, �� �������
		{
			ans = sum(n);					//����� �������, ������������ ������������� �����
		}
		else								//���� ����� ������, ��� ����� ��������
		{
			ans = subtraction(n);			//�������� �������, �������� ������ ������ �������
		}
		if (ans == 0)						//���������� ���������������� ����� ������
			ans.sign = 1;
		else
			ans.sign = sign;
	}
	if (Abs() < n.Abs())
	{
		if (sign == n.sign)					//���� ����� �������, �� �������
		{
			ans = n.sum(*this);
		}
		else								//���� ����� ������, ��� ����� ��������
		{
			ans = n.subtraction(*this);
		}
		ans.sign = n.sign;
	}
	return ans;
}
bigint bigint :: operator-(myint n) const
{
	return *this - convert(n);
}
bigint bigint :: operator-(bigint n) const
{
	bigint ans, temp;
	temp = n;
	temp.sign = -temp.sign;
	ans = *this + temp;
	return ans;
}
istream& operator>>(istream& in, bigint &a)
{
	string t; int i, v = 0, mul = 1;
	getline(in, t, '\n');
	if (t[0] == '-')						//��������� ������������� �����
	{
		a.sign = -1;
		for (size_t i = 0; i < t.size() - 1; ++i)
			t[i] = t[i + 1];
		t.erase(t.begin());
	}
	else
		a.sign = 1;

	a.length = t.size();
	a.number.clear();
	a.push_back(0);

	i = a.length - 1;

	while (i >= 0)
	{
		if (mul == a.BASE)
		{
			mul = 1;
			a.push_back(0);
			v++;
		}
		a[v] += (t[i--] - '0')* mul;
		mul *= 10;
	}
	return in;
}
ostream& operator<<(ostream& out, bigint &a)
{
	if (a.sign == -1)
		out << "-";
	int i = a.number.size() - 1; myint temp;		//����� �� ������
	bool start = 1;
	while (i >= 0)
	{
		if (!start)
		{
			temp = intlen(a[i]);
			for (int j = 0; j < a.BLOCKSIZE - temp; ++j)
				out << 0;
		}
		start = 0;
		out << a[i];
		i--;
	}
	return out;
}
bigint bigint :: operator*(myint c) const	//��������� �� ����� ��� �� �������������� �����
{
	bigint ans;
	for (size_t i = 0; i < number.size(); ++i)
	{
		ans.push_back(number[i] * c);
	}
	ans.correct();
	return ans;
}
bigint bigint :: operator*(bigint n) const
{
	bigint ans; int l = n.number.size();
	bigint temp;

	for (int i = 0; i < l; ++i)
	{
		temp = (*this) * n[i];					//������ ��������� ��������
		for (int j = 0; j < i; ++j)				//���������� �� ������� ���������
		{
			temp = temp * BASE;
		}
		ans = ans + temp;
	}
	return ans;
}
bigint bigint :: operator/(myint divider) const//������� �� ����� ������� BASE, ����� �������� �����
{
	bigint ans;
	if (divider == 0 || size() == 0)
		return ans;

	int i = size() - 1;

	myint mod, temp;
	mod = 0;

	while (i >= 0)
	{
		temp = mod*BASE + number[i];
		ans.number.insert(ans.number.begin(), temp / divider);
		mod = temp % divider;
		i--;
	}
	ans.correct();
	return ans;
}
bigint bigint :: operator/(bigint divider) const
{
	if (*this < divider || *this == 0)
		return convert(0);
	if (*this == divider)
		return convert(1);
	//	if (divider == 1)
	//		return (*this);

	bigint ans, left, right, middle, tempres, diff;
	ans.sign = sign * divider.sign;

	int i = size() - 2;
	bool finded = false;

	left = 1;
	right = (*this).Abs();
	while (!finded)
	{
		middle = (left + right) / 2;
		tempres = middle * divider;
		diff = *this - tempres;

		if (diff <= divider && diff >= 0)
		{
			//������������� ����� ������
			finded = 1;
			if (diff < divider)
				ans = middle;
			else
				ans = middle + diff.sign;
		}
		else
		{
			if (diff < 0)
			{
				right = middle;
			}
			if (diff > 0)
			{
				left = middle;
			}
		}

	}
	return ans;
}
bigint bigint :: div(bigint divider)
{
	sign = sign * divider.sign;									//���������� ����� ���������� �������������� �������

	if (divider == 0)											//�������� ����� ����
	{
		cout << "Error: division by zero." << endl;
		return convert(0);
	}
	if (divider.Abs() == 1)										//�������� ����� �������
		return convert(0);

	if (*this == 0)												//������� ����� ����
		return (*this = convert(0));

	if (divider > *this)										//�������� ������ ��������
		return (*this = convert(0)) == 0 ? divider : divider;

	if (divider.Abs() == this->Abs())							//�������� � ������� ����� �� ������
		return convert(0);

	bigint ans, temp, tempmul;									//����������, ������ ��� ������� � �������
	bigint left, right, middle, difference;						//���������� ��� ��������� ������.
	int i = number.size() - 1;
	while (i >= 0)
	{
		temp.number.insert(temp.number.begin(), 1, number[i]);
		if (temp >= divider)
		{
			//�������� ����� ��������, ���������� ��������.
			//� ��� ���� ����� temp, ������� ������ ��������, ���������� ������.
			//����� ��������� - �������� ������� ����� �������.
			left = 1;
			right = temp / 2;
			while (1)
			{
				middle = left + right;
				middle = middle / 2;
				tempmul = middle * divider;
				difference = temp - tempmul;

				if (difference > 0)
				{
					if (difference < divider.Abs())
						break;
					left = middle;
				}
				else
					right = middle;
			}
			temp.clear();
			temp.push_back(difference[0]);
			ans.number.insert(ans.number.begin(), 1, middle[0]);
		}
		i--;
	}
	*this = ans;
	return difference;
}
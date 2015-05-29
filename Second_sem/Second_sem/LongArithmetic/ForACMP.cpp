#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
#define mybase 10000
#define myblocksize 4
typedef _int32 myint;

class bigint
{
private:
	vector <myint> number;							//�������� � ����� ������� �������� BLOCKSIZE
	//�� ���� �������� ����� � ��������� BASE
	vector <myint> fraction;						//������� ����� �������� �����
	int sign;										//����� �������� �����
	int length;										//���� �������� �����
	myint BASE;								//��� ������ ����� ������ � �����-������ ������� � �������� ���������
	int BLOCKSIZE;								//��� ��� ������� �������
public:


	bigint();
	bigint(myint l);
	void push_back(myint t);
	void clear();
	size_t size() const;
	bigint Abs() const;
	bigint convert(myint a);						//������������ ����� int a � bigint �

	bigint sum(const bigint n) const;
	bigint subtraction(const bigint n) const;

	bool operator>(const bigint n) const;
	bool operator<(const bigint n1) const;

	friend istream& operator>>(istream& in, bigint &a);
	friend ostream& operator<<(ostream& out, bigint &a);
	void correct();
	bigint operator+(const bigint n) const;
	bigint operator-(const bigint n) const;

	bigint operator=(bigint n);

	myint& operator[](const size_t i);

};



template <typename T> T min(T a, T b)
{
	return (a < b ? a : b);
}
template <typename T> T max(T a, T b)
{
	return (a>b ? a : b);
}
template <typename T> T absolute(T a)
{
	return (a < 0 ? -a : a);
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
	BASE = mybase;
	BLOCKSIZE = myblocksize;
}
bigint::bigint(myint l)
{
	BASE = mybase;
	BLOCKSIZE = myblocksize;
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
bigint bigint::convert(myint a)
{
	bigint ans;
	ans.sign = (a<0 ? -1 : 1);
	if (a < 0) a = -a;
	while (a > 0)
	{
		ans.push_back(a % BASE);
		a /= BASE;
	}
	return ans;
}
myint& bigint :: operator[](const size_t i)
{
	return number[i];
}


//�����, ��������� copy/paste
void bigint::correct()					//������������ ������, ����� � ������ ����� ���� ������ �����, ������� BLOCKSIZE ������
{
	bool finish; size_t i;
	//���� � ������ ����� ����
	finish = 0;
	i = number.size() - 1;
	while (!finish && i != 0)
	{
		if (number[i] != 0)
			break;
		number.pop_back();
		i--;
	}
	//�����

	//����� ��������
	int next = 0; finish = 0;
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
	finish = 0;
	i = number.size() - 1;
	while (!finish && i != 0)
	{
		if (number[i] != 0)
			break;
		number.pop_back();
		i--;
	}
	//�����
}

bigint bigint::Abs() const
{
	bigint ans = *this;
	ans.sign = 1;
	return ans;
}

bool bigint :: operator>(const bigint n) const
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
bool bigint :: operator<(const bigint n) const
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

bigint bigint::sum(const bigint n) const					//������������ ����� ������� �� ������ ����
{
	bigint ans, tempmin, tempmax;
	tempmin = min(*this, n);								//������ ��� [] �� �������� � ������������ ���������
	tempmax = max(*this, n);
	for (int i = 0; i < tempmax.size(); ++i)
	{
		if (i < tempmin.size())
			ans.push_back(tempmax[i] + tempmin[i]);
		else
			ans.push_back(tempmax[i]);
	}
	ans.correct();											//��������, ���������, �� ��� ������ ���������
	return ans;
}
bigint bigint::subtraction(const bigint n) const	//�������������, ��� *this ������ ��� n
{
	bigint ans, minuend, subtrahend;
	minuend = *this;
	subtrahend = n;
	for (int i = 0; i < size(); ++i)
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
bigint bigint :: operator+(const bigint n) const
{
	bigint ans;
	/*
	����� ���������� ������ �������, � ������:
	- ��������������� ����� �����. ���� ��� ������, �� ������ �������� �������, ���� ���������� - �����
	- ��������������� ������ �����. ���� ����� ����� ����� ����������� �� ������ �����.
	� ����� ��������� ����������� ������ ������.
	*/

	if (Abs() > n.Abs())					//��������� �������
	{
		if (sign == n.sign)					//���� ����� �������, �� �������
		{
			ans = sum(n);					//����� �������, ������������ ������������� �����
		}
		else								//���� ����� ������, ��� ����� ��������
		{
			ans = subtraction(n);			//�������� �������, �������� ������ ������ �������
		}
		ans.sign = sign;					//���������� ���������������� ����� ������
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
bigint bigint :: operator-(const bigint n) const
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
		for (i = 0; i < t.size() - 1; ++i)
			t[i] = t[i + 1];
		t.pop_back();
	}
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
	if (a.sign == -1) cout << "-";
	int i = a.number.size() - 1, temp;		//����� �� ������
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
















bigint bigint :: operator=(bigint n)
{
	sign = n.sign;
	number = n.number;
	return (*this);
}





int main()
{
	bigint a, b, c;
	fstream in, out;
	in.open("input.txt");
	out.open("output.txt");
	if (!in.is_open())
		return 1;
	in >> a >> b;
	out << (a + b);

	return 8;
}
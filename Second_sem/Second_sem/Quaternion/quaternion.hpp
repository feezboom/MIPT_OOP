template <typename T> quat <T> ::quat(T a0 = NULL, T b0 = NULL, T c0 = NULL, T d0 = NULL) : a(a0), b(b0), c(c0), d(d0)
{}
template <typename T> quat <T> quat <T> ::update(T a0, T b0, T c0, T d0)
{
	a = a0; b = b0; c = c0; d = d0;
	return (*this);
}
template <typename T> void quat <T> ::getcrd(T &r, T &i, T &j, T &k)
{
	r = a; i = b; j = c; k = d;
}
template <typename T> Matrix <T> quat <T> ::matrixinit()
{
	Matrix <T> q(QUATDIMENSION, QUATDIMENSION);
	q[0][0] = a; q[0][1] = -b;	q[0][2] = -c;	q[0][3] = -d;
	q[1][0] = b; q[1][1] = a;	q[1][2] = -d;	q[1][3] = c;
	q[2][0] = c; q[2][1] = d;	q[2][2] = a;	q[2][3] = -b;
	q[3][0] = d; q[3][1] = -c;	q[3][2] = b;	q[3][3] = a;
	q.Transpose();
	return q;
}
template <typename T> quat <T> quat <T> :: operator+(quat <T> &q)
{
	T r, i, j, k;
	q.getcrd(r, i, j, k);
	quat <T> res;
	res.update(r + a, i + b, j + c, k + d);
	return res;
}
template <typename T> quat <T> quat <T> :: operator+(int q)
{
	quat <T> res;
	res.update(a + q, b, c, d);
	return res;
}
template <typename T> quat <T> quat <T> :: operator-(quat <T> &q)
{
	T r, i, j, k;
	q.getcrd(r, i, j, k);
	quat <T> res;
	res.update(r - a, i - b, j - c, k - d);
	return res;
}
template <typename T> quat <T> quat <T> :: operator-(int q)
{
	quat <T> res;
	res.update(a - q, b, c, d);
	return res;
}
template <typename T> quat <T> quat <T> :: operator*(quat <T> &q)
{
	quat <T> res;
	T r, i, j, k;
	q.getcrd(r, i, j, k);
	res.update(
		a*r - b*i - c*j - d*k,
		a*i + b*r + c*k - d*j,
		a*j - b*k + c*r + d*i,
		a*k + b*j - c*i + d*r);
	return res;
}
template <typename T> quat <T> quat <T> :: operator*(int q)
{
	quat <T> res;
	res.update(a * q, b * q, c * q, d * q);
	return res;
}
template <typename T> quat <T> quat <T> :: operator/(quat <T> &q)
{
	quat <T> res;
	res = (*this) * q.invert();
	return res;
}
template <typename T> quat <T> quat <T> :: operator/(int q)
{
	quat <T> res;
	res.update(a / q, b / q, c / q, d / q);
	return res;
}
template <typename T> void quat <T> :: operator+=(quat <T> &q)
{
	T r, i, j, k;
	q.getcrd(r, i, j, k);
	update(r + a, i + b, j + c, k + d);
}
template <typename T> void quat <T> :: operator-=(quat <T> &q)
{
	T r, i, j, k;
	q.getcrd(r, i, j, k);
	update(-r + a, -i + b, -j + c, -k + d);
}
template <typename T> void quat <T> :: operator*=(quat <T> &q)
{
	T r, i, j, k;
	q.getcrd(r, i, j, k);
	update(
		a*r - b*i - c*j - d*k,
		a*i + b*r + c*k - d*j,
		a*j - b*k + c*r + d*i,
		a*k + b*j - c*i + d*r);
}
template <typename T> void quat <T> :: operator/=(quat <T> &q)
{
	(*this) = (*this) * q.invert();
}
template <typename T> quat <T> quat <T> ::invert()
{
	quat <T> res;
	res = conjugate() / (double)norm();
	return res;
}
template <typename T> void quat <T> ::initbymatrix(Matrix <T> &rm)
{
	if (rm.geth() == rm.getl() && rm.geth() == QUATDIMENSION)
		update(rm[0][0], rm[0][1], rm[0][2], rm[0][3]);
	else
		cout << "Update failed.\n";
}
template <typename T> double quat <T> ::norm()
{
	return a*a + b*b + c*c + d*d;
}
template <typename T> double quat <T> ::abs()
{
	return sqrt(norm());
}
template <typename T> quat <T> quat <T> ::conjugate()
{
	quat <T> res;
	res.update(a, -b, -c, -d);
	return res;
}
template <typename T> double quat <T> ::scalar(quat <T> q)
{
	T r, i, j, k;
	q.getcrd(r, i, j, k);
	return sqrt(a*r + b*i + c*j + d*k);
}
template <typename T> quat <T> quat <T> ::vectmul(quat <T> q)
{
	quat <T> res;
	res = ((*this)*q - q*(*this));
	res = res / 2;
	return res;
}
template <typename T> void quat <T> ::printmatrix(ostream &out)
{
	Matrix <T> q(4, 4);
	q = matrixinit();
	q.Print(out);
}
template <typename T> void quat <T> ::print(ostream &out)
{
	out << (*this);
}
template <typename T> ostream& operator<< (ostream &out, quat <T> &q)
{
	bool sign = false;
	if (q.a != 0)
	{
		out << q.a;
		sign = true;
	}
	if (sign) 
		out.setf(ios::showpos);
	if (q.b != 0)
	{
		out << q.b << "i";
		sign = true;
	}
	if (sign)
		out.setf(ios::showpos);
	if (q.c != 0)
	{
		out << q.c << "j";
		sign = true;
	}
	if (sign) 
		out.setf(ios::showpos);
	if (q.d != 0)
	{
		out << q.c << "j";
		sign = true;
	}
	out.unsetf(ios::showpos);
	if (!sign) out << NULL;
	return out;
}
template <typename T> istream& operator>> (istream &in, quat <T> &q)
{
	in >> q.a >> q.b >> q.c >> q.d;
	return in;
}
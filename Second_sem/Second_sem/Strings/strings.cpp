#include "strings.h"

istream& operator>> (istream& in, line &q)
{
	q.clear();
	char t = in.get();
	while (t != '\0' && t != '\n' && t != ' ' && !in.eof())
	{
		q.push_back(t);
		t = in.get();
	}
	return in;
}
ostream& operator<< (ostream& out, line &q)
{
	size_t i = 0;
	while (i < q.size())
	{
		out << q[i];
		i++;
	}
	return out;
}
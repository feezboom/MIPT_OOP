template <typename K1, typename V1, int(Compare)(K1 a, K1 b)>
ostream& operator<<(ostream& out, top_result <K1, V1, Compare> my_top_res)
{
	for (size_t i = 0; i < my_top_res.unitsV.size(); ++i)
		out << my_top_res.unitsV[i] << endl;
	return out;
}
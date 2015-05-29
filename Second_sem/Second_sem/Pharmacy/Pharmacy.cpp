#include "pharmacy.h"

ostream& operator<<(ostream &out, drug output)
{
	out << "Name:			" << output.name << endl;
	out << "Maker:			" << output.maker << endl;
	out << "Active_substance:	" << output.active_substance << endl;
	out << "Release_Form:		" << output.release_form << endl;
	out << "Price:			" << output.price<< endl;
	out << "Dosage:			" << output.dosage<< endl;
	return out;
}
ostream& operator<<(ostream &output, const order &myorder)
{
	if (myorder.size() == 0)
	{
		output << "Your order is empty... :(" << endl;
		return output;
	}

	output << "Your order consists of:" << endl;
	for (size_t i = 0; i < myorder.size(); ++i)
		output << myorder[i].get_name() << endl;
	return output;
}
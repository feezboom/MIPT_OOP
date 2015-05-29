#include "pharmacy.h"
#include "../Strings/strings.h"

#include <fstream>

int Pharmacy_Test()
{
	fstream out;
	out.open("Pharmacy/output.txt");

	line t;
	drug a("myname1", "mymaker1", "myactivesubstance3", "myreleaseform1", 121, 1001);
	drug b("myname2", "mymaker2", "myactivesubstance1", "myreleaseform2", 102, 1002);
	drug c("myname3", "mymaker3", "myactivesubstance3", "myreleaseform3", 103, 1003);
	drug d("myname4", "mymaker4", "myactivesubstance3", "myreleaseform4", 104, 1004);
	
	a.update_name("Coldrex");
	a.update_maker("Pharm");
	a.update_active_substance("Paracetamol");
	a.update_release_form("powder");
	a.update_price(20);
	a.update_dosage(15);

	out << a.get_name() << a.get_maker() << a.get_active_substance() << a.get_release_form() << a.get_price() << a.get_dosage() << endl;
	out << a;
	b = a;
	out << b;

	vector <drug> q, w, e, r, y;
	q.push_back(a);
	w.push_back(a); w.push_back(b); e.push_back(d);
	e.push_back(a); e.push_back(b); 
	r.push_back(a); y.push_back(c);

	out << (a == b) << (a == c);


	pharmacy MyPharmacy(4, a, b, c, d);
	pharmacy qwer(e);
	
	qwer.add_drug("Tisin", "famar orlean", "ksilometasoline", "spray", 100, 2);
	qwer.update_price("Tisin", 132);

	drug p = qwer.find_by_name("Tisin");
	out << qwer.get_price("Tisin");

	vector <drug> u;
	u = MyPharmacy.list_by_subname("my");
	for (auto it = u.begin(), end = u.end(); it != end; ++it)
		out << *it;
	vector <line> names;
	names.push_back("myname3");
	names.push_back("myname4");
	u = MyPharmacy.list_by_names(names);
	for (auto it = u.begin(), end = u.end(); it != end; ++it)
		out << *it;
	vector <line> makers;
	makers.push_back("mymaker1");
	makers.push_back("Pharm");
	makers.push_back("mymaker3");
	makers.push_back("mymaker4");
	u = MyPharmacy.list_by_makers(makers);
	for (auto it = u.begin(), end = u.end(); it != end; ++it)
		out << *it;
	vector <line> active;
	active.push_back("myactivesubstance3");
	active.push_back("Paracetamol");
	u = MyPharmacy.list_by_active_substances(active);
	for (auto it = u.begin(), end = u.end(); it != end; ++it)
		out << *it;
	u = MyPharmacy.list_by_maker_substance("Pharm", "Paracetamol");
	for (auto it = u.begin(), end = u.end(); it != end; ++it)
		out << *it;
	u = MyPharmacy.list_by_release_substance("Paracetamol", "powder");
	for (auto it = u.begin(), end = u.end(); it != end; ++it)
		out << *it;

	order MyOrder;

	vector <line> active_sub;
	active_sub.push_back("myactivesubstance3");
	active_sub.push_back("myactivesubstance1");
	MyOrder = MyPharmacy.min_order_by_active_substances(active_sub);

	out << MyOrder << endl;

	names.push_back("Coldrex");
	order test = MyPharmacy.order_by_names(names);
	out << test;
	out.close();

	cout << "Pharmacy testing finished...\n";
	return 0;
}
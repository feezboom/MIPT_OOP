#pragma once

#include "../Strings/strings.h"
#include <vector>
#include <map>

using namespace std;
class pharmacy;
class drug;
class order;

/*Ќапишите класс дл€ описани€ лекарства.
ѕредусмотрите как минимум следующую информацию, которую можно

прочитать или установить:

++	название (строка)
++	производитель (строка)
++	действующее вещество (строка)
++	форма выпуска (таблетки, сироп, раствор, мазь, гель, драже, порошок, капсула, гранула, настойка, экстракт)
++	цена (double)
++	дозировка (double)
*/
class drug
{
private:
	bool NOTNULL;
	line name;
	line maker;
	line active_substance;
	line release_form;
	double price;
	double dosage;

	vector <line> available_forms;
	void fill_available_forms()
	{
		available_forms.clear();
		available_forms.push_back("tablet");
		available_forms.push_back("syrup");
		available_forms.push_back("solution");
		available_forms.push_back("ointment");
		available_forms.push_back("gel");
		available_forms.push_back("pills");
		available_forms.push_back("powder");
		available_forms.push_back("capsule");
		available_forms.push_back("granule");
		available_forms.push_back("tincture");
		available_forms.push_back("extract");
	}
	bool release_form_ok(line input_release_form)
	{
		bool check = false;
		for (size_t i = 0; i < available_forms.size(); ++i)
		{
			if (input_release_form == available_forms[i])
			{
				check = true;
				break;
			}
		}
		return check;
	}
	bool release_form_ok(char* input_release_form)
	{
		bool check = false;
		for (size_t i = 0; i < available_forms.size(); ++i)
		{
			if (available_forms[i] == input_release_form)
			{
				check = true;
				break;
			}
		}
		return check;
	}

public:
	friend class order;
	friend class pharmacy;
	drug()
	{
		fill_available_forms();
		NOTNULL = true;
	};
	drug(bool notnull)
	{
		NOTNULL = notnull;
	}
	drug(line input_name,
		line input_maker,
		line input_active_substance,
		line input_release_form,
		double input_price,
		double input_dosage)
	{
		name = input_name;
		maker = input_maker;
		active_substance = input_active_substance;
		dosage = input_dosage;
		price = input_price;
		NOTNULL = true;

		fill_available_forms();

		if (release_form_ok(input_release_form))
			release_form = input_release_form;
	}
	
	drug(char* input_name,
		char* input_maker,
		char* input_active_substance,
		char* input_release_form,
		double input_price,
		double input_dosage)
	{
		name = input_name;
		maker = input_maker;
		active_substance = input_active_substance;
		dosage = input_dosage;
		price = input_price;

		fill_available_forms();

		if (release_form_ok(input_release_form))
			release_form = input_release_form;
		NOTNULL = true;
	}

	bool update_name(line new_name)
	{
		name = new_name;
		return true;
	}
	bool update_maker(line new_maker)
	{
		maker = new_maker;
		return true;
	}
	bool update_active_substance(line new_active_substance)
	{
		active_substance = new_active_substance;
		return true;
	}
	bool update_release_form(line new_release_form)
	{
		if(release_form_ok(new_release_form))
			release_form = new_release_form;
		else release_form = "unknown";
		return true;
	}
	bool update_price(double new_price)
	{
		price = new_price;
		return true;
	}
	bool update_dosage(double new_dosage)
	{
		dosage = new_dosage;
		return true;
	}
	bool is_null()
	{
		return NOTNULL == false ? true : false;
	}

	line get_name()
	{
		return name;
	}
	line get_maker()
	{
		return maker;
	}
	line get_active_substance()
	{
		return active_substance;
	}
	line get_release_form()
	{
		return release_form;
	}
	double get_price()
	{
		return price;
	}
	double get_dosage()
	{
		return dosage;
	}

	drug operator=(drug input)
	{
		name = input.name;
		maker = input.maker;
		active_substance = input.active_substance;
		release_form = input.release_form;
		price = input.price;
		dosage = input.dosage;
		return *this;
	}
	bool operator==(drug input)
	{
		if (name != input.name) return false;
		if (maker != input.maker) return false;
		if (active_substance != input.active_substance) return false;
		if (release_form != input.release_form) return false;
		if (price != input.price) return false;
		if (dosage != input.dosage) return false;
		return true;
	}

	friend ostream& operator<<(ostream& out, drug output);
};
/*
	Ќабор классов должен позвол€ть :

++		устанавливать(добавл€ть, измен€ть) стоимость лекарства
??		добавл€ть лекарство без стоимости
++		находить стоимость лекарства
++		находить список лекарств по первым буквам названи€
++		находить все лекарства заданного производител€
++		находить все лекарства с заданным действующим веществом
++		находить лекарства заданного производител€ с заданным действующим веществом
++		фильтровать все лекарства с заданным действующим веществом по заданной форме выпуска
++		формировать и выводить заказ по названи€м препаратов
++		формировать и выводить заказ по названи€м действующих веществ, минимизиру€ его стоимость
++		просматривать и модифицировать содержание сформированного заказа

	ѕримечание.¬ этой задаче не требуетс€ применение продвинутых строковых алгоритмов.ƒостаточно средств STL.
*/	
class order
{
private:
	vector <drug> list;
	double price;
	void update_price()
	{
		price = 0;
		for (size_t i = 0; i < list.size(); ++i)
			price += list[i].price;
	}
public:
	friend class pharmacy;
	void add(drug input)
	{
		list.push_back(input);
	}
	void remove(drug input)
	{
		vector <drug> ::iterator it;
		for (it = list.begin(); it != list.end(); ++it)
			if (input == *it)
			{
				list.erase(it);
				it = list.begin();
			}
	}
	void clear()
	{
		list.clear();
	}
	size_t size()const
	{
		return list.size();
	}
	drug& operator[](int i)
	{
		return list[i];
	}
	drug operator[](int i) const
	{
		return list[i];
	}
	void show(ostream& out) const
	{
		out << *this;
	}
	friend ostream& operator<<(ostream &output, const order &myorder);
};
class pharmacy
{
private:
	map <line, drug> units;
	order CurrentOrder;
public:
	pharmacy(){};
	pharmacy(int number, drug first, ...)
	{
		drug *pFirst = &first;
		for (int i = 0; i < number; ++i)
			units[pFirst[i].get_name()] = pFirst[i];
	}
	pharmacy(vector <drug> input_drugs)
	{
		for (size_t i = 0; i < input_drugs.size(); ++i)
			units[input_drugs[i].get_name()] = input_drugs[i];
	}

	bool add_drug(line name, line maker, line active_substance, line release_form, double price, double dosage)
	{
		drug new_drug;
		new_drug.update_name(name);
		new_drug.update_maker(maker);
		new_drug.update_active_substance(active_substance);
		new_drug.update_price(price);
		new_drug.update_dosage(dosage);
		new_drug.update_release_form(release_form);
		units[name] = new_drug;
		return true;
	}
	bool update_price(line name, double new_price)
	{
		if (units.find(name) != units.end())
			units[name].update_price(new_price);
		return true;
	}
	double get_price(line name)
	{
		if (units.find(name) != units.end())
			return units[name].get_price();
		return NULL;
	}

	drug find_by_name(line name)
	{
		map <line, drug> ::iterator it;
		drug answer(false);
		for (it = units.begin(); it != units.end(); ++it)
		{
			if ((*it).first == name)
				return (it->second);
		}
		return answer;
	}

	vector <drug> list_by_subname(line tname)
	{
		map <line, drug> ::iterator it;
		vector <drug> answer;
		for (it = units.begin(); it != units.end(); ++it)
		{
			if ((*it).first.is_subs(tname))
				answer.push_back(it->second);
		}
		return answer;
	}
	vector <drug> list_by_names(vector <line> names)
	{
		vector <drug> answer;
		for (size_t i = 0; i < names.size(); ++i)
		{
			drug temp = find_by_name(names[i]);
			if (!temp.is_null())
				answer.push_back(temp);
		}
		return answer;
	}
	vector <drug> list_by_makers(vector <line> makers)
	{
		map <line, drug> ::iterator it;
		vector <drug> answer;
		for (size_t j = 0; j < makers.size(); ++j)
		{
			for (it = units.begin(); it != units.end(); ++it)
			{
				if ((*it).second.get_maker() == makers[j])
					answer.push_back(it->second);
			}
		}
		return answer;
	}
	vector <drug> list_by_active_substances(vector <line> active_substances)
	{
		map <line, drug> ::iterator it;
		vector <drug> answer;
		for (size_t j = 0; j < active_substances.size(); ++j)
		{
			for (it = units.begin(); it != units.end(); ++it)
			{
				if ((*it).second.get_active_substance() == active_substances[j])
					answer.push_back(it->second);
			}
		}
		return answer;
	}
	vector <drug> list_by_maker_substance(line maker, line active_substance)
	{
		map <line, drug> ::iterator it;
		vector <drug> answer;
		for (it = units.begin(); it != units.end(); ++it)
		{
			if ((*it).second.get_maker() == maker && (*it).second.get_active_substance() == active_substance)
				answer.push_back(it->second);
		}
		return answer;
	}
	vector <drug> list_by_release_substance(line active_substance, line release_form)
	{
		map <line, drug> ::iterator it;
		vector <drug> answer;
		for (it = units.begin(); it != units.end(); ++it)
		{
			if ((*it).second.get_release_form() == release_form && (*it).second.get_active_substance() == active_substance)
				answer.push_back(it->second);
		}
		return answer;
	}

	order order_by_names(vector <line> names)
	{
		CurrentOrder.list = list_by_names(names);
		return CurrentOrder;
	}
	order min_order_by_active_substances(vector <line> active_substances)//Ќ≈ “≈—“»–ќ¬јЌј
	{
		vector <drug> temp;
		order answer;
		temp = list_by_active_substances(active_substances);
		for (size_t i = 0; i < active_substances.size(); ++i)
		{
			double min_price = INT_MAX;
			size_t temp_num = INT_MAX;
			for (size_t j = 0; j < temp.size(); ++j)
			{
				if (active_substances[i] == temp[j].get_active_substance() && temp[j].get_price() < min_price)
				{
					temp_num = j;
					min_price = temp[j].get_price();
				}
			}
			assert(temp_num == INT_MAX || (temp_num >= 0 && temp_num < temp.size()));
			if (temp_num < temp.size() && temp_num >= 0)
				answer.add(temp[temp_num]);
		}
		return answer;
	}
};

int Pharmacy_Test();
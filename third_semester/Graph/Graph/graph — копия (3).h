#pragma once
#include <vector>
#include <unordered_map>
#include <map>
#include <list>
#include <memory>
#include "table.h"
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <iterator>
#include <fstream>
#include <exception>

//Наверное нужно реализовать алгоритмы Косарайю - поиска компонент сильной связности в графе
//http://e-maxx.ru/algo/strong_connected_components
//И, ещё, алгоритм Тарьяна, пока хз, что он делает
//Бинарная куча
//Алгоритм Форда-Беллмана
//Алгоритм Дейкстры

using namespace std;

enum class color{ white, black, gray };
template <class type> class vertex;
template <class type> class edge;
template <class type> class graph;
template <class type> class graph_list;
template <class type> class graph_table;



//**********Класс ребра***********//
template <class type>
class edge{
private:
	std::pair<vertex<type>, vertex<type>> between;
	double weigh;							//Вес ребра
};



//**********Класс вершины***********//
template <class type>
class vertex{
	template <class type>
	friend class graph_list;
	template <class type>
	friend class time_out_comparator;
	
private:
	unordered_map <string, shared_ptr <vertex <type>>> verteces_to;		//Следующие вершины	
	unordered_map <string, shared_ptr <vertex <type>>> verteces_from;	//Предыдущие вершины

	void add_way_to(string name, shared_ptr <vertex<type>> pointer) {
		verteces_to[name] = shared_ptr <vertex<type>>{ pointer };
		return;
		// then update edges.
	};
	void add_way_from(string name, shared_ptr <vertex<type>> pointer) {
		verteces_from[name] = shared_ptr <vertex<type>>{ pointer };
		return;
		// then I nust update edges. I'll do it later;
	}

	string name;
	type data;
	color current_color;					//Деталь DFS
	
	int time_in;
	int time_out;
public:
	vertex() {
		name = "noname";
		data = 0;
		current_color = color::white;
		time_in = -1;
		time_out = -1;
	}
	vertex(const string& name) {
		this->name = name;
		data = 0;
		current_color = color::white;
		time_in = -1;
		time_out = -1;
	}
	
	void set_name(const string& name){
		this->name = name;
	}
	void set_color(const color& our_color) {
		this->current_color = our_color;
	}
	
	auto get_name() {
		return name;
	}
	auto get_data() {
		return data;
	}
	auto get_color() {
		return current_color;
	}

	auto get_map_to() {
		return verteces_to;
	}
	auto get_map_from() {
		return verteces_from;
	}

	void print_vertex() {
		cout << "Vertex " << name << "."/* << endl << "Data : " << data*/ << "; Time_in: " << time_in << "; Time_out: " << time_out;
		if (current_color == color::white) cout << "; Color : white";
		if (current_color == color::gray)  cout << "; Color : gray";
		if (current_color == color::black) cout << "; Color : black";
		cout << endl;
	}
	void print_vertex_ways() {
		cout << "Ways to : ";
		for (const auto &kvp : verteces_to) {
			cout << kvp.first << " ";
		}
		cout << endl << "Ways from : ";
		for (const auto &kvp : verteces_from) {
			cout << kvp.first << " ";
		}
		cout << endl;
	}
};

template <class type>
class time_out_comparator {
public:
	bool operator()(const vertex <type>& v1, const vertex <type>& v2) {
		return v1.time_out < v2.time_out;
	}
	bool operator()(const std::pair<string, vertex <type>>& pair1, const std::pair <string, vertex <type>>& pair2) {
		return pair1.second.time_out < pair2.second.time_out;
	}
	bool operator()(const std::pair<string, shared_ptr<vertex<type>>>& pair1, const std::pair<string, shared_ptr<vertex<type>>>& pair2) {
		return (pair1.second->time_out) < pair2.second->time_out;
	}
	bool operator()(const shared_ptr <vertex<type>>& ptr1, const shared_ptr <vertex<type>>& ptr2) {
		return ptr1->time_out < ptr2->time_out;
	}
};



template <class type>
class graph_list
{
private:
	unordered_map <string, shared_ptr <vertex <type>>> verteces;
	vector <shared_ptr <vertex<type>>> pverteces;

	list <vector <edge <type>>> edges; 
	int verteces_number;

	int timer;
	bool tree;			//Если это было дерево обхода, то true, иначе - false

	void whitewash_all() {
		for (auto& it : verteces) {
			it.second->current_color = color::white;
		}
	}
	void zero_times() {
		for (auto& it : verteces) {
			it.second->time_in = 0;
			it.second->time_out = 0;
		}
	}
	bool vertex_in_graph(string name)
	{
		for (const auto& it : verteces) {
			if (it.first == name) return true;
		}
		return false;
	}
public:
	graph_list() {
		verteces_number = 0;
		timer = 0;
	};
	auto insert_vertex(const string& name) {
		if (!vertex_in_graph(name)) {
			auto ptr = make_shared <vertex<type>>(name);
			verteces[name] = ptr;
			pverteces.push_back(ptr);
		}
		return;
	}
	//Не работает : начало
	auto insert_vertex(const vertex <type>& v) {
		string name = v.get_name();
		if (vertex_in_graph(name)) return;
		
		auto ptr = make_shared <vertex<type>> (v);
		verteces[name] = ptr;
		pverteces.push_back(ptr);
		cout << pverteces.empty() << "here is vertor empty" << endl;
		return;
	}
	//Не работает : конец

	auto print_graph() {
		cout << "You called function that prints all graph as lists. Now we're starting:" << endl;
		for (const auto &it : verteces) {
			cout << "Vertex : " << it.first << "; Ways to : ";
			for (const auto &kvp : it.second->verteces_to) {
				cout << kvp.first << " ";
			}
			cout << "; Ways from : ";
			for (const auto &kvp : it.second->verteces_from) {
				cout << kvp.first << " ";
			}
			cout << endl;
		}
		cout << "printed_successfully\n";
	}
	auto insert_verteces(const vector <vertex <type>>& verteces_to_insert) {
		for (auto& it : verteces_to_insert) {
			if (!vertex_in_graph(it.name)) {
				insert_vertex(it.name);
			}
		}
		return;
	};

	auto add_edge(string from, string to) {//Здесь происходит добавление направленного ребра из вершины from в вершину to
		if (!vertex_in_graph(from) || !vertex_in_graph(to)) //Если в графе нет хотя бы одной из указанных вершин, вставка не удастся
			return;

		auto v_from = verteces[from];		//Here are the pointers to verteces with names from and to
		auto v_to = verteces[to];

		v_from->add_way_to(to, v_to);//What did I wrote here?
		v_to->add_way_from(from, v_from);		//Here I'm adding to and from verteces to theirs lists "to" and "from"
	}


	const vertex <type>& operator[](int i) {
		if (i >= 0 && i < verteces_number) {
			for (const auto& it : verteces) {
				if (it.second.Num == i) return it.second;
			}
		}
	}
	auto transpose() {
		for (auto& it : verteces) {
			auto v_pointer = (it.second);
			v_pointer->verteces_to.swap(v_pointer->verteces_from);
		}
	}
	auto Kosaraju() {
//1)С помощью алгоритма поиска в глубину найти время окончания обработки f[u] для каждой вершины u графа G
//2)Транспонировать граф G
//3)Вызвать поиск в глубину для транспонированного графа, при этом в его внешнем цикле перебирать вершины в порядке убывания величины f[u](вычисленной в строке 1)
//4)Сильно связными компонентами будут деревья поиска, построенные на шаге 3.

		

		//Первая часть:
		top_sort();
		//Вторая часть
		transpose();
		//Третья часть
		//Отсортируем в порядке убывания времени выхода

		//vector <std::pair<string, shared_ptr <vertex <type>>>> our_verteces;
		//std::copy(verteces.begin(), verteces.end(), std::back_inserter(our_verteces));
		//std::sort(verteces.begin(), verteces.end(), *(new time_out_comparator <type>{}));

		//for (const auto& it : verteces) {
		//	pverteces.insert(pverteces.begin(), it.second);
		//}
		std::sort(pverteces.begin(), pverteces.end(), *(new time_out_comparator<type>{}));
		top_sort();


	}

	auto dfs_for_top(shared_ptr <vertex<type>>& pvertex, fstream& log_file) {
		if (pvertex->current_color == color::gray || pvertex->current_color == color::black) {
			log_file << "tried to join vertex " << pvertex->name << ", but failed : color == " << ((pvertex->current_color == color::gray) ? "gray" : "black") << endl;
			return;
		}
		 	
		log_file << "dfs timer == " << (pvertex->time_in = timer++) << " joined " << pvertex->name << endl;
		pvertex->current_color = color::gray;

		for (auto& ptr : pvertex->verteces_to) {//Проходимся по всем соседям вершины графа vertex, ptrs - итератор map<string, shared_ptr> соседей вершины
			dfs_for_top(ptr.second, log_file);
		}

		log_file << "dfs timer == " << (pvertex->time_out = timer++)<< " left " << pvertex->name << endl;
		pvertex->current_color = color::black;
		return;
	}
	auto top_sort() {
		
		whitewash_all();	//Красим все вершины в белый цвет
		zero_times();		//Zero all verteces time_out and time_in

		auto& it = pverteces.begin();
		fstream log_file;
		log_file.open("dfs_for_top.log", /*ios::trunc |*/ios :: app | ios_base::out);

		timer = 0;
		log_file << "dfs_for_top started from vertex " << (*it)->name << endl;
		for (auto& it : pverteces) { //Проходимся по всем вершинам графа, it - итератор map<string, shared_ptr<vertex>> вершин графа
			dfs_for_top(it, log_file);
		}
		log_file << "Top_sort finished...\n\n\n\n\n\n\n\n\n\n";
		log_file.close();
	}
};


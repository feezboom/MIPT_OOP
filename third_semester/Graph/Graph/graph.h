#pragma once
#include <vector>
#include <unordered_map>
#include <map>
#include <list>
#include <memory>
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <iterator>
#include <fstream>
#include <exception>
#include <stack>
#include <queue>

//Наверное нужно реализовать алгоритмы Косарайю - поиска компонент сильной связности в графе
//http://e-maxx.ru/algo/strong_connected_components
//И, ещё, алгоритм Тарьяна
//Бинарная куча
//Алгоритм Форда-Беллмана
//Алгоритм Дейкстры

using namespace std;

enum class color{ white, black, gray };
template <class type> class vertex;
template <class type, class E> class edge;
template <class type, class E> class graph_list;

//**********Класс ребра***********//
template <class type, class E>
class edge{
private:
	template <class type, class E>
	friend class graph_list;
	template <class type>
	friend class vertex;

	shared_ptr <vertex<type>> vertex_to;
	shared_ptr <vertex<type>> vertex_from;
	E weigh;			//Вес ребра
public:
	edge() {
		vertex_to = nullptr;
		vertex_from = nullptr;
		weigh = 0;
	}
	edge(shared_ptr<vertex<type>> from, shared_ptr <vertex<type>> to, E weigh) {
		vertex_to = to;
		vertex_from = from;
		this->weigh = weigh;
	}
public:
	auto print_edge() {
		cout << "Edge : ";
		cout << "From : " << vertex_from->name << "; To : " << vertex_to->name << "; Weigh == " << weigh << endl;
	}

};


//**********Класс вершины***********//
template <class type>
class vertex{
	template <class type, class E>
	friend class graph_list;
	template <class type>
	friend class time_out_comparator;
	template <class type, class E>
	friend class edge;
private:
	unordered_map <string, shared_ptr <vertex <type>>> vertices_to;		//Следующие вершины	
	unordered_map <string, shared_ptr <vertex <type>>> vertices_from;	//Предыдущие вершины

	void add_way_to(string name, shared_ptr <vertex<type>> pointer) {
		vertices_to[name] = shared_ptr <vertex<type>>{ pointer };
		return;
		// then update edges.
	};
	void add_way_from(string name, shared_ptr <vertex<type>> pointer) {
		vertices_from[name] = shared_ptr <vertex<type>>{ pointer };
		return;
		// then I nust update edges. I'll do it later;
	}

	string name;
	type data;
	
//DFS	
	color current_color;					//Детали DFS
	size_t thelowestlink;
	size_t time_in;
	size_t time_out;
//DFS

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
		return vertices_to;
	}
	auto get_map_from() {
		return vertices_from;
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
		for (const auto &kvp : vertices_to) {
			cout << kvp.first << " ";
		}
		cout << endl << "Ways from : ";
		for (const auto &kvp : vertices_from) {
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

//Класс графа списками смежностей
template <class type, class E>
class graph_list
{
private:
	unordered_map <string, shared_ptr <vertex <type>>> vertices;
	vector <shared_ptr <vertex<type>>> pvertices;
	
	unordered_map<string, unordered_map<string, shared_ptr <edge <type, E>>>> edges;//[]

	int vertices_number;

	int timer;
	bool tree;			//Если это было дерево обхода, то true, иначе - false

	void whitewash_all() {
		for (auto& it : vertices) {
			it.second->current_color = color::white;
		}
	}
	void zero_numbers() {
		for (auto& it : vertices) {
			it.second->time_in = 0;
			it.second->time_out = 0;
			it.second->thelowestlink = 0;
		}
	}
	bool vertex_in_graph(const string& name)
	{
		for (const auto& it : vertices) {
			if (it.first == name) return true;
		}
		return false;
	}
public:
	graph_list() {
		vertices_number = 0;
		timer = 0;
	};
	auto insert_vertex(const string& name) {
		if (!vertex_in_graph(name)) {
			auto ptr = make_shared <vertex<type>>(name);
			vertices[name] = ptr;
			pvertices.push_back(ptr);
		}
		return;
	}
	auto get_weigh(const string& from, const string& to) {
		if (vertex_in_graph(from) && vertex_in_graph(to))
			return edges[from][to].weigh;
		else throw new exception("Edge: no such vertex\n");
	}
	auto set_weigh(const E& new_weigh) {
		weigh = new_weigh;
	}
	auto print_graph() {
		cout << "You called function that prints all graph as lists. Now we're starting:" << endl;
		for (const auto &it : vertices) {
			cout << "Vertex : " << it.first << "; Ways to : ";
			for (const auto &kvp : it.second->vertices_to) {
				cout << kvp.first << " ";
			}
			cout << "; Ways from : ";
			for (const auto &kvp : it.second->vertices_from) {
				cout << kvp.first << " ";
			}
			cout << endl;
		}
		cout << "printed_successfully\n";
	}
	auto insert_vertices(const vector <vertex <type>>& vertices_to_insert) {
		for (auto& it : vertices_to_insert) {
			if (!vertex_in_graph(it.name)) {
				insert_vertex(it.name);
			}
		}
		return;
	};

	auto add_edge(const string& from, const string& to, E weigh = 0) {
		if (!vertex_in_graph(from) || !vertex_in_graph(to)) //Если в графе нет хотя бы одной из указанных вершин, вставка не удастся
			return;

		auto v_from = vertices[from];		//Here are the pointers to vertices with names from and to
		auto v_to = vertices[to];
		edge <type, E> Edge{ v_from, v_to, weigh };
		edges[from][to] = make_shared <edge <type, E>>(Edge);
		v_from->add_way_to(to, v_to);//What did I wrote here?
		v_to->add_way_from(from, v_from);		//Here I'm adding to and from vertices to theirs lists "to" and "from"
	}

	auto operator[](const string& name) {
		if (!vertex_in_graph(name)) {
			for (const auto& it : vertices) {
				if (it.first == name) return it.second;
			}
		}
	}
	auto transpose() {
		for (auto& it : vertices) {
			auto v_pointer = (it.second);
			v_pointer->vertices_to.swap(v_pointer->vertices_from);
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

		std::sort(pvertices.rbegin(), pvertices.rend(), *(new time_out_comparator<type>{}));  //Отсортируем в порядке убывания времени выхода
		top_sort();
		
		//А здесь из лога вытащим сильно связные компоненты
		//Как из лог-файла вытащить эти компоненты? 
		//Мы должны посмотреть на подряд идущие блоки говорящие о выходе алгоритма из вершины
		//То есть строки, содержащие слово left
		// Именно такие подряд идущие строки содержат названия вершин, содержащихся в одной и той же сильно-связной компоненте

		//Working with log file, we are catching strong-connected components from it
		fstream log("dfs_for_top.log", ios :: in);
		string line;
		bool prev_left = false, first = true;
		vector <vector <string>> answer;

		size_t strong_connected_index = -1;
		while (!log.eof()) {
			getline(log, line);
			if (line[0] == 't') 
				continue;//If current line begins with "tried" - we must skip it
			//
			size_t left_position = line.find("left");
			
			if (left_position < 1000) {
				if (!prev_left) {//Если предыдущая строка не содержала слова left, то началась новая сильно-связная компонента
					answer.push_back(*(new vector<string>));//Добавим для неё отдельный вектор
					strong_connected_index++;
				}
				string name = line.substr(left_position + string("line ").size(), line.size() - 1);
				answer[strong_connected_index].push_back(name);
				prev_left = true;
			}
			else prev_left = false;
		}

		return answer;
	};

	auto dfs_for_Tarjan(shared_ptr <vertex <type>>& pvertex, fstream& log_file, vector<vector<string>>& answer) {
		static stack <shared_ptr<vertex<type>>> Stack;
		if (pvertex->current_color == color::white) {	
			Stack.push(pvertex);//pushing vertex on stack
			pvertex->thelowestlink = timer;//the lowest time from vertices from which this vertex can be reached OR making the lowestlink
			log_file << "dfs timer == " << (pvertex->time_in = timer++) << " joined " << pvertex->name << endl; //writing log
			pvertex->current_color = color::gray;//mark vertex as already visited
		}
		else {
			log_file << "tried to join vertex " << pvertex->name << ", but failed : color == " << ((pvertex->current_color == color::gray) ? "gray" : "black") << endl;
			return;
		}

		for (auto& ptr : pvertex->vertices_to) {
			if (ptr.second->current_color == color::white) {
				dfs_for_Tarjan(ptr.second, log_file, answer);	//ptr.second is a vertex in the list of vertices_to of vertex *pvertex
				pvertex->thelowestlink = min(pvertex->thelowestlink, ptr.second->thelowestlink);
			}
			else {
				if (ptr.second->current_color == color::gray) {
					pvertex->thelowestlink = min(pvertex->thelowestlink, ptr.second->time_in);
				}
				log_file << "tried to join vertex " << pvertex->name << ", but failed : color == " << ((pvertex->current_color == color::gray) ? "gray" : "black") << endl;
			}
		}


		if (pvertex->thelowestlink == pvertex->time_in) {//That is mean that it is a root of s strongly connected component
			shared_ptr <vertex<type>> vrtx;
			vector <string> component;
			do
			{
				vrtx = Stack.top();
				Stack.pop();
				vrtx->current_color = color::black;
				component.push_back(vrtx->name);
			} while (vrtx != pvertex);
			answer.push_back(component);
		}
		return;
	};
	auto Tarjan() {
		whitewash_all();
		zero_numbers();
		fstream log_file;
		vector <vector<string>> answer;
		log_file.open("dfs_for_Tarjan.log", ios::trunc | ios_base::out);
		timer = 0;

		auto& it = pvertices.begin();
		log_file << "dfs_for_Tarjan started from vertex " << (*it)->name << endl;
		for (auto& it : vertices) {
			dfs_for_Tarjan(it.second, log_file, answer);
		}
		return answer;
	}

	auto dfs_for_top(shared_ptr <vertex<type>>& pvertex, fstream& log_file) {
		if (pvertex->current_color == color::gray || pvertex->current_color == color::black) {
			log_file << "tried to join vertex " << pvertex->name << ", but failed : color == " << ((pvertex->current_color == color::gray) ? "gray" : "black") << endl;
			return;
		}

		log_file << "dfs timer == " << (pvertex->time_in = timer++) << " joined " << pvertex->name << endl;
		pvertex->current_color = color::gray;

		for (auto& ptr : pvertex->vertices_to) {//Проходимся по всем соседям вершины графа vertex, ptrs - итератор map<string, shared_ptr> соседей вершины
			dfs_for_top(ptr.second, log_file);
		}

		log_file << "dfs timer == " << (pvertex->time_out = timer++) << " left " << pvertex->name << endl;
		pvertex->current_color = color::black;
		return;
	}
	auto top_sort() {

		whitewash_all();	//Красим все вершины в белый цвет
		zero_numbers();		//Zero all vertices time_out and time_in

		auto& it = pvertices.begin();
		fstream log_file;
		log_file.open("dfs_for_top.log", ios::trunc | ios_base::out);

		timer = 0;
		log_file << "dfs_for_top started from vertex " << (*it)->name << endl;
		for (auto& it : pvertices) { //Проходимся по всем вершинам графа, it - итератор map<string, shared_ptr<vertex>> вершин графа
			dfs_for_top(it, log_file);
		}
		log_file << "Top_sort finished...\n\n\n\n\n\n\n\n\n\n";
		log_file.close();
	}

	void Ford_Bellman(const string& name) {
		for (const auto& it : edges) {
			for (const auto& jt : it.second) {
				jt.second->print_edge();
			}
		}

/*		fstream log_file{ "Ford_Bellman.log", ios::trunc | ios_base::out };
		log_file << "Ford-Bellman started its work." << endl;
		unordered_map <string, E> answer{ { name, 0 } };
		if (vertex_in_graph(name)) {
			queue <shared_ptr<vertex<type>>> Queue;
			shared_ptr <vertex<type>> start = vertices[name];
			whitewash_all();

			Queue.push(start);
			while (!Queue.empty()) {
				auto v = Queue.front();
				Queue.pop();
				v->current_color = color::gray;
				for (auto& it : v->vertices_to) {
					if (it.second->current_color == color::white) {
						Queue.push(it.second);
						answer[it.second->name] = answer[v->name] + edges[v->name][it.second->name]->weigh;
						log_file << "Visited vertex " << it.second->name << ", the lowest way was set to " << answer[it.second->name] << endl;
					}
					else {
						log_file << "Tried to visit vertex " << it
							.second->name << ", but failed : color isn't white." << endl;
					}
				}
			}
		}
		else
		{
			log_file << "requested vertex was not found." << endl;
		}
		log_file << "Ford-Bellman finished." << endl;
		log_file.close();
		return answer;*/
	}
};

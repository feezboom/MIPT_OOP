#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include "graph.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	cout << "Hello world! I'm started my work." << endl;

	fstream in;													//Считывание из файла
	vertex <int> v;												//Временная вершинка графа
	string name, current_name;									//переменные для хранения имён вершин
	graph_list <int, double> our_graph;							//экземпляр класса графа. Для проверки
	shared_ptr <vertex <int>> ptr;								//Экземпляр умного указателя
	size_t vertices_number, vertex_count;						//Переменные для хранения количества вершин для вставки в граф
	vector <vertex <int>> vertices;
	

	in.open("test_graph.txt");
	in >> vertices_number;										//Количество вершин, которое будет добавлено

																//Вводим имена и создаем вершины по именам
	for (size_t i = 0; i < vertices_number; i++){
		in >> name;
		vertices.push_back(vertex <int> (name));
	}

	our_graph.insert_vertices(vertices);						//Вставляем вершины в граф
																//Далее заполняем эти вершины информацией
	for (size_t i = 0; i < vertices_number; i++)
	{
		in >> name;												//Вершина для заполнения её соседей.
		in >> vertex_count;										//Количество вершин, в которые ведет вершина name
																//Смотрим куда ведет данная, текущая вершина, и заполняем список
		for (size_t i = 0; i < vertex_count; i++)
		{
			double weigh;
			in >> current_name;
			in >> weigh; 
//Здесь устанавливаем связи между вершинами в графе. Две строчки ниже.
			our_graph.add_edge(name, current_name, weigh);
		}
	}
	
//	our_graph.insert_vertex("Fuck");

	//int* q = new int(24);
	//shared_ptr <int> p(new int(10));
	//shared_ptr <int> p1(p);
	//shared_ptr <int> p2(q);
	//cout << "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ --->>> " << *p2 << endl;

	//vertex <int> a{ "shit" };
	//shared_ptr <vertex<int>> s = make_shared <vertex<int>>(a);
	//a.set_name("motherfucker");
	//s->set_name("unit");

//	our_graph.insert_vertex(a);
	
	our_graph.add_edge("Fuck", "motherfucker");

//	our_graph.top_sort();
//	our_graph.print_graph();									//Выводим граф на экран our_graph.print_graph();

	vector<vector<string>> ans = our_graph.Kosaraju();
//	vector <vector<string>> ans = our_graph.Tarjan();
	
	our_graph.transpose();
	our_graph.print_graph();

	cout << "Strong connected components are:" << endl;
	for (const auto& i : ans) {
		for (const auto& j : i) {
			cout << j << " ";
		}
		cout << endl;
	}
//	try {
////		unordered_map <string, double> ford_bellban = our_graph.Ford_Bellman("v1");
//	}
//	catch (const exception& ex) {
//		cerr << ex.what();
//	}

//	our_graph.Ford_Bellman("v1");
	
	cout << "Hey, program finished its work;)\n";
	return 0;
}
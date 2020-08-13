#pragma once
#include <iostream>
#include <map>

template <typename Key>
class AdjacencyList {
public:
	using key_type = Key;
	using size_type = size_t;

private:
	struct Node{
		key_type edge;
		size_type weight{ 0 };
		Node* next_edge{ nullptr };
	};

	Node* list;
	size_type max_sz{ 4 };
	size_type sz{ 0 };

	void insert_(const key_type& edge, Node* next_);
	void dfs_(const key_type& start, std::map<key_type, bool>&  edges);
	void bfs_(const key_type& start, std::map<key_type, bool>& edges);
	bool connects_component(const key_type& from, const key_type& to);

public:
	AdjacencyList();
	~AdjacencyList();
	AdjacencyList(const AdjacencyList& other);
	void add_edge(const key_type& edge, std::map<key_type, size_type> connections = {});
	void add_connection(const key_type& from, const key_type& to, size_type weight);
	bool del_edge(const key_type& edge);
	bool del_connection(const key_type& from, const key_type& to);
	size_type size() const;
	size_type count(const key_type& edge) const;
	std::map<key_type, size_t> get_connnections(const key_type& edge);
	
	//Algorithms
	void traverse_bfs();
	void traverse_dfs();
	AdjacencyList dijkstra_algo(const key_type& start);
	AdjacencyList kruskal_algo() const;

	friend std::ostream& operator<<(std::ostream& o, const AdjacencyList& l) {
		for (size_type i{ 0 }; i < l.sz; ++i) {
			o << l.list[i].edge;
			Node *help{ l.list[i].next_edge };
			while (help != nullptr) {
				o << " -> (" << help->edge << ", " << help->weight << ")";
				help = help->next_edge;
			}
			o << "\n";
		}
		return o;
	}
};
#include "AdjacencyList.h"
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

template <typename Key>
AdjacencyList<Key>::AdjacencyList() {
	list = new Node[max_sz];
}

template <typename Key>
AdjacencyList<Key>::~AdjacencyList() {
	for (size_type i{ 0 }; i < sz; ++i) {
		Node* head{ list[i].next_edge };
		Node* help;
		while (head != nullptr) {
			help = head;
			head = head->next_edge;
			delete help;
		}
	}
	delete[] list;
}

template <typename Key>
AdjacencyList<Key>::AdjacencyList(const AdjacencyList& other) {
	list = new Node[other.max_sz];
	for (size_type i{ 0 }; i < other.sz; ++i) {
		insert_(other.list[i].edge, other.list[i].next_edge);
	}
}

template <typename Key>
void AdjacencyList<Key>::insert_(const key_type& edge, Node* next_) {
	list[sz++].edge = edge;
	Node* to_insert{ next_ };
	Node* help{ list + (sz - 1) };
	while (to_insert) {
		help->next_edge = new Node;
		help->next_edge->edge = to_insert->edge;
		help->next_edge->weight = to_insert->weight;
		help = help->next_edge;
		to_insert = to_insert->next_edge;
	}
}

template <typename Key>
void AdjacencyList<Key>::add_edge(const key_type& edge, std::map<key_type, size_type> connections) {	
	//Create a bigger List and initialize the List with the "old values" if the List is to small
	if (sz == max_sz) {
		Node* old_list{ list };
		list = new Node[++max_sz];
		for (size_type i{ 0 }; i < sz; ++i) {
			list[i] = old_list[i];
		}
		delete[] old_list;
	}

	//Insert the new edge and their connections
	list[sz++].edge = edge;
	Node* help{ list + (sz - 1) };
	for (const auto& e : connections) {
		help->next_edge = new Node;
		help->next_edge->edge = e.first;
		help->next_edge->weight = e.second;
		help = help->next_edge;
	}
}

template <typename Key>
bool AdjacencyList<Key>::del_edge(const key_type& edge) {
	for (size_type i{ 0 }; i < sz; ++i) {
		if (std::equal_to<key_type>{}(list[i].edge, edge)) {
			Node* head{ list[i].next_edge };
			Node* help;
			while (head) {
				help = head;
				head = head->next_edge;
				delete help;
			}
			std::swap(list[i], list[--sz]);
		}
		else {
			Node* head{ list + i };
			Node* help{ head->next_edge };
			while (help != nullptr) {
				if (std::equal_to<key_type>{}(help->edge, edge)) {
					head->next_edge = help->next_edge;
					delete help;
					head = head->next_edge;
					if (!head) break;
					help = head->next_edge;
				}
				else {
					head = help;
					help = help->next_edge;
				}
			}
		}
	}
	return false;
}

template<typename Key>
typename AdjacencyList<Key>::size_type AdjacencyList<Key>::size() const { return sz; }

template<typename Key>
void AdjacencyList<Key>::traverse_bfs() {
	std::map<key_type, bool> edges;
	for (size_type i{ 0 }; i < sz; ++i) {
		edges.insert_or_assign(list[i].edge, false);
	}
	for (const auto& a : edges) {
		if (!a.second) {
			bfs_(a.first, edges);
		}
	}
	std::cout << std::endl;
}

template<typename Key>
void AdjacencyList<Key>::traverse_dfs() {
	std::map<key_type, bool> edges;
	for (size_type i{ 0 }; i < sz; ++i) {
		edges.insert_or_assign(list[i].edge, false);
	}
	for (const auto& a : edges) {
		if (!a.second) {
			dfs_(a.first, edges);
		}
	}
	std::cout << std::endl;
}

/*
//Recursive implementation of dfs_
template<typename Key>
void AdjacencyList<Key>::dfs_(const key_type& start, std::map<key_type, bool>& edges) {
	std::cout << start << " ";
	edges.find(start)->second = true;
	std::vector<key_type> neighbours;
	for (size_type i{ 0 }; i < sz; ++i) {
		if (std::equal_to<key_type>{}(list[i].edge, start)) {
			Node* help{ list[i].next_edge };
			while (help) {
				if (!edges.find(help->edge)->second) dfs_(help->edge, edges);
				help = help->next_edge;
			}
		}
	}
}
*/

template<typename Key>
void AdjacencyList<Key>::dfs_(const key_type& start, std::map<key_type, bool>& edges) {
	std::cout << start << " ";
	edges.find(start)->second = true;
	std::stack<key_type> stack;
	stack.push(start);
	key_type edge;
	while (!stack.empty()) {
		edge = stack.top();
		stack.pop();
		for (size_type i{ 0 }; i < sz; ++i) {
			if (std::equal_to<key_type>{}(list[i].edge, edge)) {
				Node* help{ list[i].next_edge };
				while (help) {
					auto it{ edges.find(help->edge) };
					if (!it->second) {
						it->second = true;
						stack.push(help->edge);
						std::cout << help->edge << " ";
					}
					help = help->next_edge;
				}
				return;
			}
		}
	}
}



template<typename Key>
void AdjacencyList<Key>::bfs_(const key_type& start, std::map<key_type, bool>& edges) {
	std::cout << start << " ";
	edges.find(start)->second = true;
	std::queue<key_type> queue;
	queue.push(start);
	key_type edge;
	while (!queue.empty()) {
		edge = queue.front();
		queue.pop();
		for (size_type i{ 0 }; i < sz; ++i) {
			if (std::equal_to<key_type>{}(list[i].edge, edge)) {
				Node* help{ list[i].next_edge };
				while (help) {
					auto it{ edges.find(help->edge) };
					if (!it->second) {
						it->second = true;
						queue.push(help->edge);
						std::cout << help->edge << " ";
					}
					help = help->next_edge;
				}
				return;
			}
		}
	}
}

template<typename Key>
typename AdjacencyList<Key>::size_type AdjacencyList<Key>::count(const key_type& edge) const {
	size_type out{ 0 };
	for (size_type i{ 0 }; i < sz; ++i) {
		if (std::equal_to<key_type>{}(list[i].edge, edge)) ++out;
		Node* help{ list[i].next_edge };
		while (help) {
			if (std::equal_to<key_type>{}(help->edge, edge)) ++out;
			help = help->next_edge;
		}
	}
	return out;
}

template<typename Key>
std::map<typename AdjacencyList<Key>::key_type, size_t> AdjacencyList<Key>::get_connnections(const key_type& edge) {
	std::map<key_type, size_t> map;
	for (int i{ 0 }; i < sz; ++i) {
		if (std::equal_to<key_type>{}(list[i].edge, edge)) {
			Node* help{ list[i].next_edge };
			while (help) {
				map.insert_or_assign(help->edge, help->weight);
				help = help->next_edge;
			}
		}
	}
	return map;
}

template <typename Key>
bool AdjacencyList<Key>::del_connection(const key_type& from, const key_type& to) {
	for (size_type i{ 0 }; i < sz; ++i) {
		if (std::equal_to<key_type>{}(list[i].edge, from)) {
			Node* head{ list+i };
			Node* help{ head->next_edge };
			while (help != nullptr) {
				if (std::equal_to<key_type>{}(help->edge, to)) {
					head->next_edge = help->next_edge;
					delete help;
					head = head->next_edge;
					if (!head) break;
					help = head->next_edge;
				}
				else {
					head = help;
					help = help->next_edge;
				}
			}
			return true;
		}
	}
	return false;
}

template <typename Key>
void AdjacencyList<Key>::add_connection(const key_type& from, const key_type& to, size_type weight) {
	for (size_type i{ 0 }; i < sz; ++i) {
		if (std::equal_to<key_type>{}(list[i].edge, from)) {
			Node* help{ list+i };
			while (help != nullptr) {
				if (!help->next_edge) {
					help->next_edge = new Node;
					help->next_edge->edge = to;
					help->next_edge->weight = weight;
					return;
				}
				help = help->next_edge;
			}
		}
	}
}

template <typename Key>
AdjacencyList<Key> AdjacencyList<Key>::dijkstra_algo(const key_type& start) {
	AdjacencyList<Key> out;
	std::map<key_type, size_type> connections;

	//Add all connections from the start edge to "connections"
	Node* help;
	for (size_type i{ 0 }; i < sz; ++i) {
		if (std::equal_to<key_type>{}(list[i].edge, start)) {
			help = list[i].next_edge;
			while (help != nullptr) {
				if (!std::equal_to<key_type>{}(help->edge, start)) connections.insert_or_assign(help->edge, help->weight);
				help = help->next_edge;
			}
			break;
		}
	}
	 
	//Update the path if there is a shorter one or add a path if there is a new edge
	for (size_type i{ 0 }; i < sz; ++i) {
		if (connections.count(list[i].edge) > 0) {
			help = list[i].next_edge;
			while (help != nullptr) {
				if (std::equal_to<key_type>{}(help->edge, start)) help = help->next_edge; 
				else {

					//If the connection is already in the "connections" than we look if we found a shorter path and update it
					if (connections.find(help->edge) != connections.end()) {
						if (connections.find(list[i].edge)->second + help->weight < connections.find(help->edge)->second) {
							connections[help->edge] = connections.find(list[i].edge)->second + help->weight;
						}
					}

					//Means that we found a new edge and therefore we add it to the "connections"
					else {
						connections.insert_or_assign(help->edge, connections.find(list[i].edge)->second + help->weight);
					}
					help = help->next_edge;
				}
			}
		}
	}

	//Add the connections to the AdjacencyList
	out.add_edge(start, connections);

	return out;
}

template<typename Key>
AdjacencyList<Key> AdjacencyList<Key>::kruskal_algo() const {
	AdjacencyList minmal_spanning_tree;
	for (size_type i{ 0 }; i < sz; ++i) minmal_spanning_tree.add_edge(list[i].edge);
	
	//		weight		    from	to
	std::multimap<size_type, std::pair<key_type, key_type>> map;

	for (size_type i{ 0 }; i < sz; ++i) {
		Node* help{ list[i].next_edge };
		while (help) {
			map.insert(std::make_pair(help->weight,std::make_pair(list[i].edge, help->edge)));
			help = help->next_edge;
		}
	}

	//if minmal_spanning_tree.count(edge) returns 1: edge has no connections
	for (const auto& a : map) {
		if (minmal_spanning_tree.count(a.second.second) == 1 || minmal_spanning_tree.count(a.second.first) == 1) {
			minmal_spanning_tree.add_connection(a.second.first, a.second.second, a.first);
			minmal_spanning_tree.add_connection(a.second.second, a.second.first, a.first);
		}
		else if (minmal_spanning_tree.connects_component(a.second.second, a.second.first) || minmal_spanning_tree.connects_component(a.second.first, a.second.second)) {
			minmal_spanning_tree.add_connection(a.second.first, a.second.second, a.first);
			minmal_spanning_tree.add_connection(a.second.second, a.second.first, a.first);
		}
	}

	return minmal_spanning_tree;
}

template <typename Key>
bool AdjacencyList<Key>::connects_component(const key_type& from, const key_type& to) {
	std::stack<key_type> stack;
	std::vector<key_type> vec;
	std::map<key_type, bool> edges;
	for (size_type i{ 0 }; i < sz; ++i) edges.insert_or_assign(list[i].edge, false);
	stack.push(from);
	key_type edge;

	//DFS to find out if components exist
	while (!stack.empty()) {
		edge = stack.top();
		stack.pop();
		for (size_type i{ 0 }; i < sz; ++i) {
			if (std::equal_to<key_type>{}(list[i].edge, edge)) {
				Node* help{ list[i].next_edge };
				while (help) {
					auto it{ edges.find(help->edge) };
					if (!it->second) {
						it->second = true;
						stack.push(help->edge);
						vec.push_back(help->edge);
					}
					help = help->next_edge;
				}
			}
		}
	}
	
	//Means that a component which contains "from" exists 
	if (vec.size() != sz) {

		//If "to" is not in "vec" than this connections connects 2 components
		if (std::find(vec.begin(), vec.end(), to) == vec.end()) return true;
		return false;
	}
	return false;
}

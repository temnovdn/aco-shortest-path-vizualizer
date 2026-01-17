#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <stdexcept>
#include <map>
#include <vector>
#include <iostream>
#include <limits>

using namespace std;

class GraphExcept: public runtime_error
{
        public:
                GraphExcept(const string& msg): runtime_error(msg) {};
};

class EdgeNotExist: public runtime_error
{
        public:
                EdgeNotExist(const string& msg): runtime_error(msg) {};
};

/*
 * graph is represented as an adjacency matrix
 * the matrix is represented as the map 
 * 
 */


class Graph
{
	public:
		Graph();
		~Graph();

	typedef map<string, map<string, int>> AdjMatrix;
	typedef AdjMatrix::iterator r_iter; // iterator for adjacency matrix rows
	typedef map<string, int>::iterator c_iter; // iterator for adjacency matrix columns

		void addVertex(const string name);
		void removeVertex(const string name);

	void addEdge(const string v1, const string v2, const int weight);
	void removeEdge(const string v1, const string v2);
	bool edgeExist(const string v1, const string v2);
	int getWeight(const string v1, const string v2);
	void setWeight(const string v1, const string v2, const int weight);

		int countVertices() const;
		int countEdges() const;
		vector<string> getVertices() const;

		void print() const;
	private:
		int edges;
		AdjMatrix adjMatrix;
};

#endif // GRAPH_H

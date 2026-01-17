#ifndef ACOSERVICE_H
#define ACOSERVICE_H

#include "graph.h"
#include <vector>
#include <map>
#include <set>
#include <string>

using namespace std;

class ACOService
{
	public:
		ACOService(const map<string, map<string, int>>& adjacencyMatrix);
		~ACOService();

		typedef map<string, map<string, int>> PheromoneMatrix;
		typedef PheromoneMatrix::iterator p_r_iter; // iterator for pheromone matrix rows
		typedef map<string, int>::iterator p_c_iter; // iterator for pheromone matrix columns

		// Getters
		vector<string> getVertices() const;
		int getWeight(const string v1, const string v2);
		int getPheromone(const string v1, const string v2);
		void setPheromone(const string v1, const string v2, const int pheromone);
		
		// ACO methods
		void iterate();
		
		// Get iterations made
		int getIterationsMade() const;
		int getAntsNumber() const;
		
	private:
		Graph graph;
		PheromoneMatrix pheromoneMatrix;
		int iterationsMade;
		int antsNumber;
};

ACOService::ACOService(const map<string, map<string, int>>& adjacencyMatrix)
	: iterationsMade(0)
{
	// Extract all unique vertices from the adjacency matrix
	set<string> uniqueVertices;
	for (const auto& row : adjacencyMatrix)
	{
		uniqueVertices.insert(row.first);
		for (const auto& col : row.second)
		{
			uniqueVertices.insert(col.first);
		}
	}

	antsNumber = uniqueVertices.size();
	
	// Add all vertices to the graph
	for (const string& vertex : uniqueVertices)
	{
		graph.addVertex(vertex);
	}
	
	// Add all edges from the adjacency matrix
	for (const auto& row : adjacencyMatrix)
	{
		const string& v1 = row.first;
		for (const auto& col : row.second)
		{
			const string& v2 = col.first;
			int weight = col.second;
			
			// Skip self-loops (diagonal entries with value 0)
			if (v1 == v2 && weight == 0)
			{
				continue;
			}
			
			// Add edge if it doesn't already exist
			if (!graph.edgeExist(v1, v2) && !graph.edgeExist(v2, v1))
			{
				graph.addEdge(v1, v2, weight);
				// Initialize pheromone for this edge
				pheromoneMatrix[v1][v2] = 0;
			}
		}
	}
}

ACOService::~ACOService()
{
	for (p_r_iter row = pheromoneMatrix.begin(); row != pheromoneMatrix.end(); ++row)
	{
		row->second.clear();
	}
	pheromoneMatrix.clear();
}

vector<string> ACOService::getVertices() const
{
	return graph.getVertices();
}

int ACOService::getWeight(const string v1, const string v2)
{
	return graph.getWeight(v1, v2);
}

int ACOService::getPheromone(const string v1, const string v2)
{
	if (!graph.edgeExist(v1, v2))
	{
		throw EdgeNotExist("The edge does not exist");
	}
	
	return pheromoneMatrix[v1][v2];
}

void ACOService::setPheromone(const string v1, const string v2, const int pheromone)
{
	if (v1 == v2) throw GraphExcept("The vertex can't have pheromone with itself");
	
	if (!graph.edgeExist(v1, v2))
	{
		throw EdgeNotExist("The edge does not exist");
	}
	
	pheromoneMatrix[v1][v2] = pheromone;
}

void ACOService::iterate()
{
	// TODO: Implement ACO iteration
	// This will contain the ant colony optimization algorithm logic
	iterationsMade++;
}

int ACOService::getIterationsMade() const
{
	return iterationsMade;
}

#endif // ACOSERVICE_H

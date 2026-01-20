#include "ACOService.h"
#include <set>
#include <string>
#include <random>
#include <algorithm>
#include <cmath>
 
using namespace std;

ACOService::ACOService(const map<string, map<string, int>>& adjacencyMatrix)
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

	antsNumber = uniqueVertices.size(); // Amount of ants is equal to the amount of vertices
	maxIterations = 100;  // Default max iterations
	
	// Initialize ACO parameters
	alpha = 1.0;  // Pheromone importance
	beta = 2.0;   // Heuristic importance
    evaporationRate = 0.4;    // Evaporation rate (40%)  zhao change
    depositAmount = 100.0;    // Pheromone deposit constant
	
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
			if (!graph.edgeExist(v1, v2))//zhao
			{
				graph.addEdge(v1, v2, weight);
				// Initialize pheromone for this edge with small initial value
				pheromoneMatrix[v1][v2] = 1.0;
				

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

double ACOService::getPheromone(const string v1, const string v2)
{
	if (!graph.edgeExist(v1, v2))
	{
		throw EdgeNotExist("The edge does not exist");
	}
	
	return pheromoneMatrix[v1][v2];
}

void ACOService::setPheromone(const string v1, const string v2, const double pheromone)
{
	if (v1 == v2) throw GraphExcept("The vertex can't have pheromone with itself");
	
	if (!graph.edgeExist(v1, v2))
	{
		throw EdgeNotExist("The edge does not exist");
	}
	
	pheromoneMatrix[v1][v2] = pheromone;
}

vector<string> ACOService::getAvailableNeighbors(const string& current, const vector<string>& visited)
{
	vector<string> neighbors;
	vector<string> vertices = graph.getVertices();
	
	for (const string& vertex : vertices)
	{
		// Check if edge exists and vertex is not visited
		if (graph.edgeExist(current, vertex) && 
		    find(visited.begin(), visited.end(), vertex) == visited.end())
		{
			neighbors.push_back(vertex);
		}
	}
	
	return neighbors;
}

double ACOService::calculateTransitionProbability(const string& from, const string& to, const vector<string>& visited)
{
	// Get available neighbors
	vector<string> available = getAvailableNeighbors(from, visited);
	
	if (available.empty())
	{
		return 0.0;
	}
	
	// Calculate denominator: sum of (tau^alpha * eta^beta) for all available neighbors
	// tau (pheromone): amount of pheromone on the edge - higher values indicate better paths
	// alpha: pheromone importance parameter - controls how much ants rely on pheromone trails
	// eta (heuristic): 1/weight - inverse of edge weight, shorter edges are more attractive
	// beta: heuristic importance parameter - controls how much ants prefer shorter edges
	double denominator = 0.0;
	for (const string& neighbor : available)
	{
		double pheromone = 0.0;
		if (graph.edgeExist(from, neighbor))
		{
			pheromone = pheromoneMatrix.at(from).at(neighbor);
		}
		
		int weight = graph.getWeight(from, neighbor);
		double eta = (weight > 0) ? (1.0 / weight) : 0.0;  // Heuristic value
		
		denominator += pow(pheromone, alpha) * pow(eta, beta);
	}
	
	if (denominator == 0.0)
	{
		return 1.0 / available.size();  // Equal probability if no pheromone
	}
	
	// Calculate numerator: (tau^alpha * eta^beta) for the target edge
	double pheromone = 0.0;
	if (graph.edgeExist(from, to))
	{
		pheromone = pheromoneMatrix.at(from).at(to);
	}
	
	int weight = graph.getWeight(from, to);
	double eta = (weight > 0) ? (1.0 / weight) : 0.0;
	
	double numerator = pow(pheromone, alpha) * pow(eta, beta);
	
	return numerator / denominator;
}

string ACOService::selectNextVertex(const string& current, const vector<string>& visited, mt19937& rng)
{
	vector<string> available = getAvailableNeighbors(current, visited);
	
	if (available.empty())
	{
		return "";  // No available neighbors
	}
	
	// Calculate probabilities for each available neighbor
	vector<double> probabilities;
	double sum = 0.0;
	
		for (const string& neighbor : available)
		{
			double prob = calculateTransitionProbability(current, neighbor, visited);
			probabilities.push_back(prob);
			sum += prob;
		}
	
	// Normalize probabilities
	if (sum > 0.0)
	{
		for (double& prob : probabilities)
		{
			prob /= sum;
		}
	}
	else
	{
		// Equal probability for all the edges if probabilities sum is zero
		double equalProb = 1.0 / available.size();
		for (double& prob : probabilities)
		{
			prob = equalProb;
		}
	}
	
	// Roulette wheel selection: select a random number between 0 and 1 and select the edge with the cumulative probability that is greater than the random number
	uniform_real_distribution<double> dist(0.0, 1.0);
	double randomValue = dist(rng);
	
	double cumulative = 0.0;
	for (size_t i = 0; i < available.size(); i++)
	{
		cumulative += probabilities[i];
		if (randomValue <= cumulative)
		{
			return available[i];
		}
	}
	
	// Fallback to last vertex
	return available.back();
}

void ACOService::depositPheromones(const vector<vector<string>>& antPaths, const string& destination)
{
	for (const vector<string>& path : antPaths)
	{
		// Check if ant reached destination
		if (!path.empty() && path.back() == destination)
		{
			// Calculate path length
			int pathLength = 0;
			for (size_t i = 0; i < path.size() - 1; i++)
			{
				pathLength += graph.getWeight(path[i], path[i+1]);
			}
			
			if (pathLength > 0)
			{
				// Deposit pheromone: depositAmount / pathLength
				double deposit = depositAmount / pathLength;
				
				// Deposit on each edge of the path
				for (size_t i = 0; i < path.size() - 1; i++)
				{
					const string& from = path[i];
					const string& to = path[i+1];
					
					if (graph.edgeExist(from, to))
					{
						double currentPheromone = 0.0;
						if (pheromoneMatrix.find(from) != pheromoneMatrix.end() &&
						    pheromoneMatrix[from].find(to) != pheromoneMatrix[from].end())
						{
							currentPheromone = pheromoneMatrix[from][to];
						}
						
						pheromoneMatrix[from][to] = currentPheromone + deposit;
					}
				}
			}
		}
	}
}

void ACOService::evaporatePheromones()
{
	// Evaporate pheromones on all edges: new = old * (1 - evaporationRate)
	for (auto& row : pheromoneMatrix)
	{
		for (auto& col : row.second)
		{
			col.second = col.second * (1.0 - evaporationRate);
		}
	}
}

vector<string> ACOService::findPath(const string source, const string destination)
{
	// Initialize pheromone matrix for all edges
	pheromoneMatrix.clear();
	vector<string> vertices = graph.getVertices();
	for (const string& from : vertices)
	{
		for (const string& to : vertices)
		{
			if (graph.edgeExist(from, to))
			{
				// Initialize pheromone for this edge with small initial value
				pheromoneMatrix[from][to] = 1.0;
			}
		}
	}
	
	// Validate input vertices
	if (find(vertices.begin(), vertices.end(), source) == vertices.end())
	{
		throw GraphExcept("Source vertex '" + source + "' not found in the graph");
	}
	if (find(vertices.begin(), vertices.end(), destination) == vertices.end())
	{
		throw GraphExcept("Destination vertex '" + destination + "' not found in the graph");
	}
	
	if (source == destination)
	{
		return vector<string>{source};  // Same vertex, return path with single vertex
	}
	
	// Run multiple iterations to find the best path
	vector<string> bestPath;
	// Maximum path length: 16 vertices means max 15 edges, assuming reasonable edge weights
	int bestPathLength = 10000;  // Reasonable upper bound for graph with max 16 vertices
	
	const int pheromoneIterations = 30; //Number of outer iteration rounds
	for (int iteration = 0; iteration < pheromoneIterations; iteration++)
	{
		// Create vector of ants
		vector<vector<string>> ants(antsNumber);
		for (int i = 0; i < antsNumber; i++)
		{
			ants[i].push_back(source);
		}
		
		// Random number generator
		random_device rd;
		mt19937 rng(rd());
		
		// Ants move until destination reached or maxIterations
		int currentIteration = 0;
		while (currentIteration < maxIterations)
		{
			// Move each ant
			for (vector<string>& antPath : ants)
			{
				// Skip if ant already reached destination
				if (!antPath.empty() && antPath.back() == destination)
				{
					continue;
				}
				
				// Get current position
				string current = antPath.back();
				
				// Select next vertex
				string next = selectNextVertex(current, antPath, rng);
				
				if (!next.empty())
				{
					antPath.push_back(next);
				}// If no next vertex available, ant is stuck (keep current position)
			   else
			   {

				 antPath.clear();
				 antPath.push_back(source);
			   }
			}

			
			currentIteration++;
			
			// Check if all ants reached destination
			bool allReached = true;
			for (const vector<string>& antPath : ants)
			{
				if (antPath.empty() || antPath.back() != destination)
				{
					allReached = false;
					break;
				}
			}
			
			if (allReached)
			{
				break;
			}
		}
		
		// Find the best path from this iteration
		for (const vector<string>& antPath : ants)
		{
			if (!antPath.empty() && antPath.back() == destination)
			{
				// Calculate path length
				int pathLength = 0;
				for (size_t i = 0; i < antPath.size() - 1; i++)
				{
					pathLength += graph.getWeight(antPath[i], antPath[i+1]);
				}
				
				if (pathLength < bestPathLength)
				{
					bestPathLength = pathLength;
					bestPath = antPath;
				}
			}
		}
		
		// Deposit pheromones for ants that reached destination
		depositPheromones(ants, destination);
		
		// Evaporate pheromones on all edges
		evaporatePheromones();
	}
	
	return bestPath;
}

int ACOService::getAntsNumber() const
{
	return antsNumber;
}

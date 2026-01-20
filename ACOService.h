#ifndef ACOSERVICE_H
#define ACOSERVICE_H

#include "graph.h"
#include <vector>
#include <map>
#include <random>

using namespace std;

class ACOService
{
	public:
		ACOService(const map<string, map<string, int>>& adjacencyMatrix);
		~ACOService();

		typedef map<string, map<string, double>> PheromoneMatrix;
		typedef PheromoneMatrix::iterator p_r_iter; // iterator for pheromone matrix rows
		typedef map<string, double>::iterator p_c_iter; // iterator for pheromone matrix columns

		// Getters
		vector<string> getVertices() const;
		int getWeight(const string v1, const string v2);
		double getPheromone(const string v1, const string v2);
		void setPheromone(const string v1, const string v2, const double pheromone);
		
		// ACO methods
		vector<string> findPath(const string source, const string destination);
		
		int getAntsNumber() const;
		
	private:
		Graph graph;
		PheromoneMatrix pheromoneMatrix;
		
		// ACO parameters
		double alpha;  // Pheromone importance
		double beta;   // Heuristic importance (1/weight)
        double evaporationRate;    // Evaporation rate
        double depositAmount;      // Pheromone deposit constant
        int antsNumber; // Amount of ants
        int maxIterations; // Maximum steps to reach the destination for the ant
		
		// Helper methods
		vector<string> getAvailableNeighbors(const string& current, const vector<string>& visited);
		double calculateTransitionProbability(const string& from, const string& to, const vector<string>& visited);
        std::string selectNextVertex(const std::string& current,const std::vector<std::string>& visited,std::mt19937& rng);
        void depositPheromones(const vector<vector<string>>& antPaths, const string& destination);
		void evaporatePheromones();
};
#endif

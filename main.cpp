#include "graph.h"
#include "ACOService.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include<QApplication>
#include"MainWindow.h"

using namespace std;

void testGraphClass()
{
	// GRAPH CLASS TESTS
        Graph graph;

        // check that there is no vertices and edges in the graph
        assert(graph.countVertices() == 0);
        assert(graph.countEdges() == 0);

        // testing vertices manipulations
        graph.addVertex("A");
        assert(graph.countVertices() == 1);
        assert(graph.countEdges() == 0);

        try
        {
                graph.addVertex("A");
        }
        catch (GraphExcept)
        {
                cout << "TEST PASSED: can't add the same vertex twice." << endl;
        }

        graph.addVertex("B");

        assert(graph.countVertices() == 2);
        assert(graph.countEdges() == 0);

        // testing edges manipulations

        const int testWeightAB = 1;

        graph.addEdge("A", "B", testWeightAB);
	
	assert(graph.countVertices() == 2);
        assert(graph.countEdges() == 1);
        assert(graph.edgeExist("A", "B"));
        assert(graph.getWeight("A", "B") == testWeightAB);

        graph.removeEdge("A", "B");
        assert(graph.edgeExist("A", "B") == false);
        assert(graph.countEdges() == 0);

        graph.removeVertex("B");

        assert(graph.countVertices() == 1);

        try
        {
                graph.removeVertex("B");
        }
        catch (GraphExcept)
        {
                cout << "TEST PASSED: Can't remove the same vertex twice" << endl;
        }

        // test non-directed graph: can't add edge if it already exists in opposite direction
        graph.addVertex("B");
        graph.addVertex("C");
        graph.addEdge("B", "C", 5);
        
        try
        {
                graph.addEdge("C", "B", 10);
        }
        catch (GraphExcept)
        {
                cout << "TEST PASSED: Can't add edge if it already exists between the same vertices (non-directed graph)" << endl;
        }

}

void testACOServiceClass()
{
	// ACOSERVICE CLASS TESTS
	map<string, map<string, int>> adjacencyMatrix;
	
	// Create a simple graph: A-B-D, B-C
	adjacencyMatrix["A"]["A"] = 0;
	adjacencyMatrix["A"]["B"] = 1;
	adjacencyMatrix["B"]["A"] = 1;
	adjacencyMatrix["B"]["B"] = 0;
	adjacencyMatrix["B"]["C"] = 5;
	adjacencyMatrix["B"]["D"] = 3;
	adjacencyMatrix["C"]["B"] = 5;
	adjacencyMatrix["C"]["C"] = 0;
	adjacencyMatrix["D"]["B"] = 3;
	adjacencyMatrix["D"]["D"] = 0;
	
	ACOService acoService(adjacencyMatrix);
	
	// Test that pheromone is initialized to 0 when edge is added
	assert(acoService.getPheromone("B", "D") == 0);
	
	// Test that setter sets a value and it can be retrieved by getter
	const int testPheromone = 42;
	acoService.setPheromone("B", "D", testPheromone);
	assert(acoService.getPheromone("B", "D") == testPheromone);
	
	// Test that exception is thrown when trying to get pheromone on non-existent edge
	try
	{
		acoService.getPheromone("A", "D");
	}
	catch (EdgeNotExist)
	{
		cout << "TEST PASSED: Exception thrown when getting pheromone on non-existent edge" << endl;
	}
	
	// Test that exception is thrown when trying to set pheromone on non-existent edge
	try
	{
		acoService.setPheromone("A", "D", 10);
	}
	catch (EdgeNotExist)
	{
		cout << "TEST PASSED: Exception thrown when setting pheromone on non-existent edge" << endl;
	}
}

int main(int argc, char *argv[])
{
	testGraphClass();
	testACOServiceClass();

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}

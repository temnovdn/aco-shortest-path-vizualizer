#include"graph.h"

Graph::Graph()
{
        edges = 0;
}

Graph::~Graph()
{
        for (r_iter row = adjMatrix.begin(); row != adjMatrix.end(); ++row)
        {
                row->second.clear();
        }
        adjMatrix.clear();
}


void Graph::addVertex(const string name)
{
        if( adjMatrix.find(name) != adjMatrix.end())
        {
                throw GraphExcept("The vertex already exists!");
        }

        adjMatrix[name][name] = 0;
}

void Graph::removeVertex(const string name)
{
        if( adjMatrix.find(name) == adjMatrix.end())
        {
                throw GraphExcept("The vertex does not exist");
        }

        for (r_iter it = adjMatrix.begin(); it != adjMatrix.end(); ++it)
        {
                it->second.erase(name);
        }
        adjMatrix.erase(name);
}

int Graph::countVertices() const
{
        return adjMatrix.size();
}

vector<string> Graph::getVertices() const
{
        vector<string> vertices;
        for (const auto& row : adjMatrix)
        {
                vertices.push_back(row.first);
        }
        return vertices;
}

int Graph::countEdges() const
{
        return edges;
}

void Graph::addEdge(const string v1, const string v2, const int weight)
{
        if(v1 == v2)
        {
                throw GraphExcept("You can't add an edge between the same vertex");
        }

          if(edgeExist(v1, v2))
        {
                throw GraphExcept("The edge already exists");
        }

        adjMatrix[v1][v2] = weight;
        edges++;
}

void Graph::removeEdge(const string v1, const string v2)
{
        if(v1 == v2)
        {
                throw GraphExcept("You can't remove an edge between the same vertex");
        }

        if (!edgeExist(v1, v2))
        {
                throw EdgeNotExist("The edge does not exist");
        }

        adjMatrix[v1].erase(v2);
        edges--;
}

bool Graph::edgeExist(const string v1, const string v2)
{
        if (v1 == v2) return false;

        // Check if v1 exists in the adjacency matrix
        r_iter row = adjMatrix.find(v1);
        if (row == adjMatrix.end()) return false;

        // Check if v2 exists in v1's row
        c_iter col = row->second.find(v2);
        if (col == row->second.end()) return false;

        // Entry exists, so edge exists (self-loops are already filtered out by v1 == v2 check)
        return true;
}

int Graph::getWeight(const string v1, const string v2)
{
        if (!edgeExist(v1, v2))
        {
                throw EdgeNotExist("The edge does not exist");
        }

        return adjMatrix[v1][v2];
}

void Graph::setWeight(const string v1, const string v2, const int weight)
{
        if (v1 == v2) throw GraphExcept("The vertex can't have an edge with itself");

        if (!edgeExist(v1, v2))
        {
                throw EdgeNotExist("The edge does not exist");
        }

        adjMatrix[v1][v2] = weight;
}


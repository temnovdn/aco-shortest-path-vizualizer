#ifndef ACORUNNER_H
#define ACORUNNER_H
#include <vector>
#include <string>

class Graph;

struct AcoResult
{
    std::vector<std::string> path;
    int totalWeight = 0;
};

class AcoRunner
{
public:
    AcoRunner() = default;
    AcoResult run(Graph& graph,const std::string& source,const std::string& destination);
private:
    int calculateTotalWeight(Graph& graph,const std::vector<std::string>& path);
};
#endif
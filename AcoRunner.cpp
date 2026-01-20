#include "AcoRunner.h"
#include "graph.h"
#include "ACOService.h"
#include <map>

AcoResult AcoRunner::run(Graph& graph,const std::string& source,const std::string& destination)

{
 AcoResult result;
 std::map<std::string, std::map<std::string, int>> adjacency;
 
for (const auto& u : graph.getVertices())
 {
for (const auto& v : graph.getVertices()) 
    {
if (graph.edgeExist(u, v))
  {
    adjacency[u][v] = graph.getWeight(u, v);
    adjacency[v][u] = graph.getWeight(u, v);
}
 }
}
  const int maxAttempts = 30;
for (int attempt = 0; attempt < maxAttempts; ++attempt)
{
    ACOService aco(adjacency);                           
    result.path = aco.findPath(source, destination);     

  if (!result.path.empty())
      break;
}
  result.totalWeight = calculateTotalWeight(graph, result.path);
  return result;
}

int AcoRunner::calculateTotalWeight(Graph& graph,const std::vector<std::string>& path)
{
   int sum = 0;
   if (path.size() < 2)
   return 0;
  for (size_t i = 0; i + 1 < path.size(); ++i)
    { 
    const std::string& u = path[i];
    const std::string& v = path[i + 1];

   if (graph.edgeExist(u, v))
    sum += graph.getWeight(u, v);
    else if (graph.edgeExist(v, u))
    sum += graph.getWeight(v, u);
     
     }
return sum;
}
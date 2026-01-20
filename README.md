# ACO Shortest Path Visualizer

This project implements the **Ant Colony Optimization (ACO)** algorithm for finding the shortest path between two vertices in a weighted graph.

## Algorithm Overview

The Ant Colony Optimization algorithm is a metaheuristic shortest path search algorithm inspired by the foraging behavior of ants. Ants deposit pheromones on paths they traverse, and subsequent ants are more likely to follow paths with stronger pheromone trails. Over time, this leads to the discovery of optimal paths.

## Implementation Details

### Core Components

#### 1. **ACOService** (`ACOService.h`, `ACOService.cpp`)
The main class implementing the ACO algorithm. It manages:
- **Graph representation**: Uses the `Graph` class to represent the weighted graph
- **Pheromone matrix**: Maintains pheromone levels for each edge in the graph
- **ACO parameters**: Configurable parameters that control algorithm behavior

#### 2. **Graph Structure** (`graph.h`, `graph.cpp`)
Represents the graph as an adjacency matrix using nested maps:
```cpp
map<string, map<string, int>> adjMatrix
```
- Vertices are identified by string names
- Edge weights are stored as integers
- Supports bidirectional edges

#### 3. **AcoRunner** (`AcoRunner.h`, `AcoRunner.cpp`)
Wrapper class that:
- Converts the `Graph` structure to an adjacency matrix format
- Executes the ACO algorithm
- Handles multiple attempts if no path is found initially
- Calculates the total weight of the resulting path

### ACO Parameters

The algorithm uses the following default parameters:

| Parameter | Value | Description |
|-----------|-------|-------------|
| `alpha` | 1.0 | Pheromone importance factor - controls how much ants rely on existing pheromone trails |
| `beta` | 2.0 | Heuristic importance factor - controls how much ants prefer shorter edges |
| `evaporationRate` | 0.4 | Rate at which pheromones evaporate (40% per iteration) |
| `depositAmount` | 100.0 | Base amount of pheromone deposited by ants |
| `antsNumber` | N | Number of ants equals the number of vertices in the graph |
| `maxIterations` | 100 | Maximum steps an ant can take before stopping |
| `pheromoneIterations` | 30 | Number of outer iteration rounds |

### Algorithm Flow

The `findPath(source, destination)` method implements the following steps:

#### 1. **Initialization**
```cpp
// Initialize pheromone matrix with value 1.0 for all edges
pheromoneMatrix[from][to] = 1.0;
```

#### 2. **Main Loop** (30 iterations)
For each iteration:

##### a. **Ant Creation**
- Create `N` ants (where `N` = number of vertices)
- All ants start at the source vertex

##### b. **Ant Movement**
Each ant moves until it reaches the destination or exceeds `maxIterations`:
- **Current position**: Get the ant's current vertex
- **Next vertex selection**: Use probabilistic selection based on transition probability
- **Transition probability** calculation:
  ```
  P(i,j) = (τᵢⱼ^α × ηᵢⱼ^β) / Σ(τᵢₖ^α × ηᵢₖ^β)
  ```
  Where:
  - `τᵢⱼ` = pheromone level on edge (i,j)
  - `ηᵢⱼ` = 1/weight(i,j) (heuristic value - shorter edges are more attractive)
  - `α` = pheromone importance (1.0)
  - `β` = heuristic importance (2.0)
  - The sum is over all available unvisited neighbors

- **Roulette wheel selection**: Randomly select next vertex based on calculated probabilities
- If an ant gets stuck (no available unvisited neighbors), it resets to the source vertex

##### c. **Pheromone Update**
After all ants complete their paths:

- **Deposit**: Ants that successfully reached the destination deposit pheromones:
  ```
  deposit = depositAmount / pathLength
  ```
  Shorter paths receive more pheromone per edge, making them more attractive in future iterations.

- **Evaporation**: All edges lose pheromone:
  ```
  new_pheromone = old_pheromone × (1 - evaporationRate)
  ```
  This prevents the algorithm from getting stuck in local optima and allows exploration of new paths.

#### 3. **Best Path Selection**
After all the iterations are done, return the shortest path found among all successful ant paths.

### Key Methods

#### `calculateTransitionProbability(from, to, visited)`
Calculates the probability of an ant moving from vertex `from` to vertex `to`, given the vertices already visited. This implements the core ACO transition rule.

#### `selectNextVertex(current, visited, rng)`
Uses roulette wheel selection to probabilistically choose the next vertex based on transition probabilities.

#### `depositPheromones(antPaths, destination)`
Deposits pheromones on edges of paths taken by ants that successfully reached the destination. The amount deposited is inversely proportional to the path length.

#### `evaporatePheromones()`
Reduces pheromone levels on all edges by the evaporation rate, allowing the algorithm to forget less optimal paths over time.

### Algorithm Characteristics

1. **Probabilistic Exploration**: Ants don't always choose the best immediate option, allowing exploration of potentially better paths.

2. **Positive Feedback**: Successful paths (shorter paths) receive more pheromone, making them more likely to be followed in future iterations.

3. **Negative Feedback**: Pheromone evaporation prevents the algorithm from converging too quickly to suboptimal solutions.

4. **Multiple Ants**: Using multiple ants (equal to the number of vertices) provides parallel exploration of the solution space.

5. **Iterative Improvement**: The 30 outer iterations allow the pheromone matrix to evolve and guide ants toward better solutions.

### Example Usage

```cpp
// Create graph and add edges
Graph graph;
graph.addVertex("A");
graph.addVertex("B");
graph.addVertex("C");
graph.addEdge("A", "B", 5);
graph.addEdge("B", "C", 3);
graph.addEdge("A", "C", 10);

// Run ACO
AcoRunner runner;
AcoResult result = runner.run(graph, "A", "C");

// Result contains:
// - result.path: vector of vertices in the shortest path
// - result.totalWeight: total weight of the path
```

### Performance Considerations

- **Time Complexity**: O(I × A × V × E), where:
  - I = number of iterations (30)
  - A = number of ants (equal to number of vertices)
  - V = number of vertices
  - E = average number of edges per vertex

- **Space Complexity**: O(V²) for the pheromone matrix

- The algorithm is designed for graphs with up to 16 vertices, with a maximum path length of 15 edges.

## References

This implementation follows the classic Ant System (AS) variant of the Ant Colony Optimization metaheuristic, originally proposed by Marco Dorigo in the 1990s for solving combinatorial optimization problems.

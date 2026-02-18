/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
& Purpose: Build a graph of cities and          &
& run BFS and DFS with readable output          &
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

#include "Graph.hpp"

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::size_t;
using std::string;
using std::vector;

// print a title section so output is easier to scan
void printSection(const string &title);

// explain what BFS and DFS are showing
void printTraversalLegend();

// build the city graph from matrix data
Graph buildCityGraph();

// run a fixed set of traversal demos
void runTraversalDemo(const Graph &cityGraph);

int main() {
  const Graph cityGraph = buildCityGraph();

  printSection("Graph");
  cityGraph.display();

  printSection("Traversal Legend");
  printTraversalLegend();

  printSection("Traversal Demo");
  runTraversalDemo(cityGraph);

  return 0;
}

void printSection(const string &title) {
  cout << "==================== " << title << " ====================\n";
}

void printTraversalLegend() {
  cout << "BFS = breadth-first search by level\n";
  cout << "DFS = depth-first search by branch\n\n";
}

Graph buildCityGraph() {
  Graph cityGraph;

  // city names used as graph vertices
  const vector<string> cityNames{"Austin", "Boston", "Chicago", "Denver", "El Paso", "Fresno"};

  // adjacency matrix that defines city connections
  const vector<vector<int>> adjacencyMatrix{
      {0, 1, 1, 0, 0, 0}, {1, 0, 0, 1, 1, 0}, {1, 0, 0, 1, 0, 1},
      {0, 1, 1, 0, 1, 1}, {0, 1, 0, 1, 0, 0}, {0, 0, 1, 1, 0, 0},
  };

  // only use upper triangle to avoid duplicate edge inserts
  for (size_t rowIndex = 0; rowIndex < cityNames.size(); ++rowIndex) {
    for (size_t columnIndex = rowIndex + 1; columnIndex < cityNames.size(); ++columnIndex) {
      if (adjacencyMatrix[rowIndex][columnIndex] != 0) {
        cityGraph.addEdge(cityNames[rowIndex], cityNames[columnIndex]);
      }
    }
  }

  return cityGraph;
}

void runTraversalDemo(const Graph &cityGraph) {
  // valid BFS start points
  cityGraph.BFS("Austin");
  cityGraph.BFS("Denver");

  // invalid start point to show not-found behavior
  cityGraph.BFS("Miami");

  // valid DFS start points
  cityGraph.DFS("Austin");
  cityGraph.DFS("Fresno");
}

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef>
#include <limits>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

using std::list;
using std::numeric_limits;
using std::size_t;
using std::string;
using std::unordered_map;
using std::vector;

class Graph {
public:
  // add an edge between two named vertices
  void addEdge(const string &fromVertex, const string &toVertex, bool bidirectional = true);

  // show each vertex and its neighbors
  void display() const;

  // run BFS starting from a named vertex
  void breadthFirstSearch(const string &startVertex) const;

  // run DFS starting from a named vertex
  void depthFirstSearch(const string &startVertex) const;

  // simple call for BFS
  void BFS(const string &startVertex) const;

  // simple call for DFS
  void DFS(const string &startVertex) const;

private:
  // create the vertex if missing and return its index
  size_t ensureVertex(const string &vertex);

  // find the index of a vertex name
  size_t findVertexIndex(const string &vertex) const;

  // check if an index is valid
  [[nodiscard]] bool isValidIndex(size_t index) const;

  // count undirected edges for display output
  [[nodiscard]] size_t edgeCount() const;

  // DFS helper that works with indices
  void depthFirstSearchImpl(size_t startIndex, vector<bool> &visited, vector<size_t> &order) const;

  // value used when a vertex is not found
  static constexpr size_t invalidIndex = numeric_limits<size_t>::max();

  // names of all vertices
  vector<string> vertexNames;

  // adjacency lists using indices
  vector<list<size_t>> adjacencyLists;

  // lookup from name to index
  unordered_map<string, size_t> vertexLookup;
};

#endif

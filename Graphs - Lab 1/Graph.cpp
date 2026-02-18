#include "Graph.hpp"

#include <algorithm>
#include <iostream>
#include <queue>

using std::cout;
using std::find;
using std::queue;

size_t Graph::ensureVertex(const string &vertex) {
  // return existing index when the vertex already exists
  const auto lookup = vertexLookup.find(vertex);
  if (lookup != vertexLookup.end()) {
    return lookup->second;
  }

  // add a new vertex across all graph containers
  const size_t newIndex = vertexNames.size();
  vertexNames.push_back(vertex);
  adjacencyLists.emplace_back();
  vertexLookup[vertex] = newIndex;
  return newIndex;
}

size_t Graph::findVertexIndex(const string &vertex) const {
  // map name to index or return invalid when missing
  const auto lookup = vertexLookup.find(vertex);
  if (lookup == vertexLookup.end()) {
    return invalidIndex;
  }
  return lookup->second;
}

bool Graph::isValidIndex(size_t index) const {
  // validate graph index before access
  return index != invalidIndex && index < vertexNames.size();
}

size_t Graph::edgeCount() const {
  // each undirected edge appears twice in adjacency lists
  size_t neighborLinks = 0;
  for (const list<size_t> &neighbors : adjacencyLists) {
    neighborLinks += neighbors.size();
  }
  return neighborLinks / 2;
}

void Graph::BFS(const string &startVertex) const {
  // keep short alias for demo calls
  breadthFirstSearch(startVertex);
}

void Graph::DFS(const string &startVertex) const {
  // keep short alias for demo calls
  depthFirstSearch(startVertex);
}

void Graph::addEdge(const string &fromVertex, const string &toVertex, bool bidirectional) {
  const size_t fromIndex = ensureVertex(fromVertex);
  const size_t toIndex = ensureVertex(toVertex);

  // add one directional link only when missing
  auto connect = [this](size_t sourceIndex, size_t destinationIndex) {
    list<size_t> &neighbors = adjacencyLists[sourceIndex];
    const auto neighborPosition = find(neighbors.begin(), neighbors.end(), destinationIndex);
    if (neighborPosition == neighbors.end()) {
      neighbors.push_back(destinationIndex);
    }
  };

  connect(fromIndex, toIndex);
  if (bidirectional) {
    connect(toIndex, fromIndex);
  }
}

void Graph::display() const {
  // summary before detailed adjacency lists
  cout << "Vertices: " << vertexNames.size() << '\n';
  cout << "Undirected edges: " << edgeCount() << '\n';
  cout << "---------------------------------------------------------\n";

  for (size_t vertexIndex = 0; vertexIndex < vertexNames.size(); ++vertexIndex) {
    cout << "[" << vertexNames[vertexIndex] << "] -> ";

    bool hasNeighbors = false;
    bool isFirstNeighbor = true;
    for (size_t neighborIndex : adjacencyLists[vertexIndex]) {
      hasNeighbors = true;
      if (!isFirstNeighbor) {
        cout << ", ";
      }
      cout << vertexNames[neighborIndex];
      isFirstNeighbor = false;
    }

    if (!hasNeighbors) {
      cout << "(no neighbors)";
    }
    cout << '\n';
  }
  cout << "=========================================================\n\n";
}

void Graph::breadthFirstSearch(const string &startVertex) const {
  // resolve start point from label
  const size_t startIndex = findVertexIndex(startVertex);
  if (!isValidIndex(startIndex)) {
    cout << "BFS start vertex '" << startVertex << "' was not found\n\n";
    return;
  }

  // track visited state and resulting order
  vector<bool> visited(vertexNames.size(), false);
  queue<size_t> vertexQueue;
  vector<size_t> visitOrder;

  visited[startIndex] = true;
  vertexQueue.push(startIndex);

  while (!vertexQueue.empty()) {
    const size_t currentIndex = vertexQueue.front();
    vertexQueue.pop();
    visitOrder.push_back(currentIndex);

    for (size_t neighborIndex : adjacencyLists[currentIndex]) {
      if (!visited[neighborIndex]) {
        visited[neighborIndex] = true;
        vertexQueue.push(neighborIndex);
      }
    }
  }

  cout << "BFS starting at " << startVertex << '\n';
  for (size_t orderIndex = 0; orderIndex < visitOrder.size(); ++orderIndex) {
    cout << "  Step " << orderIndex + 1 << ": visit " << vertexNames[visitOrder[orderIndex]] << '\n';
  }

  cout << "  Order: ";
  for (size_t orderIndex = 0; orderIndex < visitOrder.size(); ++orderIndex) {
    if (orderIndex > 0) {
      cout << " -> ";
    }
    cout << vertexNames[visitOrder[orderIndex]];
  }
  cout << "\n\n";
}

void Graph::depthFirstSearch(const string &startVertex) const {
  // resolve start point from label
  const size_t startIndex = findVertexIndex(startVertex);
  if (!isValidIndex(startIndex)) {
    cout << "DFS start vertex '" << startVertex << "' was not found\n\n";
    return;
  }

  // track visited state and resulting order
  vector<bool> visited(vertexNames.size(), false);
  vector<size_t> traversalOrder;
  depthFirstSearchImpl(startIndex, visited, traversalOrder);

  cout << "DFS starting at " << startVertex << '\n';
  for (size_t orderIndex = 0; orderIndex < traversalOrder.size(); ++orderIndex) {
    cout << "  Step " << orderIndex + 1 << ": visit " << vertexNames[traversalOrder[orderIndex]] << '\n';
  }

  cout << "  Order: ";
  for (size_t orderIndex = 0; orderIndex < traversalOrder.size(); ++orderIndex) {
    if (orderIndex > 0) {
      cout << " -> ";
    }
    cout << vertexNames[traversalOrder[orderIndex]];
  }
  cout << "\n\n";
}

void Graph::depthFirstSearchImpl(size_t startIndex, vector<bool> &visited, vector<size_t> &order) const {
  // pre-order visit for DFS traversal
  visited[startIndex] = true;
  order.push_back(startIndex);

  for (size_t neighborIndex : adjacencyLists[startIndex]) {
    if (!visited[neighborIndex]) {
      depthFirstSearchImpl(neighborIndex, visited, order);
    }
  }
}

#include <functional>

#include "LinkedList.h"

#include <iostream>
#include <vector>

// Define a graph class using the LinkedList as adjacency lists
template <typename KeyType, typename ValueType, typename CompareFn>
class Graph
{
public:
  // Constructor for the graph
  Graph(int numVertices) : numVertices(numVertices)
  {
    adjacencyLists.resize(numVertices);
  }

  // Add an edge to the graph
  void addEdge(KeyType source, KeyType destination, ValueType weight)
  {
    // Create a key-value pair to represent the edge
    KeyNode<KeyType, ValueType> edge(destination, weight);

    // Add the edge to the adjacency list of the source vertex
    adjacencyLists[source].insert(edge);
  }

  // Print the graph
  void printGraph()
  {
    for (int i = 0; i < numVertices; ++i)
    {
      std::cout << "Vertex " << i << " adjacency list:" << std::endl;
      for (auto it = adjacencyLists[i].begin(); it != adjacencyLists[i].end(); ++it)
      {
        // Dereference the iterator before accessing its members
        const KeyNode<int, double> &edge = *it;
        std::cout << "  -> Vertex: " << edge.getKey() << ", Weight: " << edge.getValue() << std::endl;
      }
    }
  }

private:
  int numVertices;
  std::vector<LinkedList<KeyType, ValueType, CompareFn>> adjacencyLists;
};
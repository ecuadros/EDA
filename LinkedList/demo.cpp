#include <iostream> // cout
#include <fstream>  // ofstream, ifstream
#include <cmath>
#include <memory>
#include <functional>

#include "KeyNode.h"
#include "LinkedList.h"
#include "DoubleLinkedList.h"
#include "HashTable.h"
#include "Graph.h"
#include "demo.h"
using namespace std;

void DemoLinkedList()
{
    std::cout << "------------LINKED LIST------------" << std::endl;
    // Example usage of the NodeLinkedList class
    KeyNode<int, std::string> keyValue(1, "Hello");
    NodeLinkedList<int, std::string> node(keyValue);

    KeyNode<int, std::string> keyValue2(2, "World");
    NodeLinkedList<int, std::string> node2(keyValue2);

    node.setNext(&node2);

    // Accessing values
    std::cout << "Node 1 Key: " << node.getValue().getKey() << ", Value: " << node.getValue().getValue() << std::endl;
    std::cout << "Node 2 Key: " << node.getNext()->getValue().getKey() << ", Value: " << node.getNext()->getValue().getValue() << std::endl;

    // Using AscendingLinkedList trait
    LinkedList<int, std::string, std::less<int>> ascendingList;
    ascendingList.insert(KeyNode<int, std::string>(3, "Three"));
    ascendingList.insert(KeyNode<int, std::string>(1, "One"));
    ascendingList.insert(KeyNode<int, std::string>(2, "Two"));

    std::cout << "Ascending Linked List:" << std::endl;
    ascendingList.printList();

    // Using DescendingLinkedList trait
    LinkedList<int, std::string, std::greater<int>> descendingList;
    descendingList.insert(KeyNode<int, std::string>(3, "Three"));
    descendingList.insert(KeyNode<int, std::string>(1, "One"));
    descendingList.insert(KeyNode<int, std::string>(2, "Two"));

    std::cout << "Descending Linked List:" << std::endl;
    // Use a foreach loop with the custom iterator
    for (const auto &item : descendingList)
    {
        std::cout << "Key: " << item.getKey() << ", Value: " << item.getValue() << std::endl;
    }

    // Access elements by index
    size_t index = 1;
    LinkedList<int, std::string, std::less<int>>::Node *selectedNode = ascendingList[index];
    if (selectedNode)
    {
        std::cout << "Element at index " << index << ": Key=" << selectedNode->getValue().getKey() << ", Value=" << selectedNode->getValue().getValue() << std::endl;
    }
    else
    {
        std::cout << "Index " << index << " is out of range." << std::endl;
    }

    LinkedList<int, std::string, std::less<int>> myList;

    // Use operator>> to input elements into the list
    std::cout << "Enter elements for the list (Ctrl+Z/Ctrl+D to stop):" << std::endl;
    while (std::cin >> myList)
    {
        // Continue reading elements
    }

    std::cout << "Linked List:" << std::endl;
    std::cout << myList;
    std::cout << std::endl << "----------END LINKED LIST----------" << std::endl;
}

void DemoDoubleLinkedList()
{
    std::cout << "---------DOUBLE LINKED LIST---------" << std::endl;
    // Using AscendingLinkedList trait
    DoubleLinkedList<int, std::string, std::less<int>> myReverseList;
    myReverseList.insert(KeyNode<int, std::string>(3, "Three"));
    myReverseList.insert(KeyNode<int, std::string>(1, "One"));
    myReverseList.insert(KeyNode<int, std::string>(2, "Two"));

    for (auto it = myReverseList.rbegin(); it != myReverseList.rend(); --it)
    {
        const KeyNode<int, std::string> &node = *it;
        std::cout << "Key: " << node.getKey() << ", Value: " << node.getValue() << std::endl;
    }
    std::cout << std::endl << "-------END DOUBLE LINKED LIST-------" << std::endl;
}

void DemoHashTable()
{
    std::cout << "----------HASH LINKED LIST----------" << std::endl;
    // Create a hash table with a size of 10.
    HashTable<std::string, int, std::hash<std::string>> hashTable(10);

    // Insert key-value pairs.
    hashTable.insert(KeyNode<std::string, int>("Alice", 25));
    hashTable.insert(KeyNode<std::string, int>("Bob", 30));
    hashTable.insert(KeyNode<std::string, int>("Charlie", 35));
    hashTable.insert(KeyNode<std::string, int>("David", 40));

    // Print the hash table.
    hashTable.printTable();
    std::cout << std::endl << "--------END HASH LINKED LIST--------" << std::endl;
}

void DemoGraph()
{
    std::cout << "----------GRAPH LINKED LIST----------" << std::endl;
    // Create a graph with 4 vertices
    Graph<int, double, std::less<int>> graph(4);

    // Add edges to the graph
    graph.addEdge(0, 1, 1.5);
    graph.addEdge(0, 2, 2.3);
    graph.addEdge(1, 2, 0.8);
    graph.addEdge(2, 3, 1.2);

    // Print the graph
    graph.printGraph();

    std::cout << std::endl << "--------END GRAPH LINKED LIST--------" << std::endl;
}
#include <iostream> // cout
#include <fstream>  // ofstream, ifstream
#include <cmath>
#include <memory>
#include <functional>

#include "KeyNode.h"
#include "BinaryTree.h"
#include "BinaryTreeAVL.h"
#include "BPlusTree.h"
#include "demo.h"
using namespace std;

void DemoBinaryTree()
{
    // Create KeyNode objects
    KeyNode<int, std::string> node1(0, "Root");
    KeyNode<int, std::string> node2(1, "left");
    KeyNode<int, std::string> node3(2, "right");
    // Create a new BinaryTree object.
    BinaryTree<int, std::string, std::less<int>> myTreeLess(node1);
    BinaryTree<int, std::string, std::greater<int>> myTreeGreater(node1);
    // Insert some nodes into the tree.
    myTreeLess.insert(node2);
    myTreeLess.insert(node3);
    // Print the tree structure using the printTree function.
    std::function<std::string(const KeyNode<int, std::string> &)> print_value =
        [](const KeyNode<int, std::string> &keyNode)
    {
        return keyNode.getValue() + " [" + std::to_string(keyNode.getKey()) + "] ";
    };

    std::cout << "Tree Structure:" << std::endl;
    myTreeLess.printTree(print_value);
    // Iterator
    cout << "Preorder traversal:" << endl;
    for (auto it = myTreeLess.beginPreorder(); it != myTreeLess.endPreorder(); ++it)
    {
        const KeyNode<int, std::string> &node = *it;
        cout << "Key: " << node.getKey() << ", Value: " << node.getValue() << endl;
    }
    cout << "Postorder traversal:" << endl;
    for (auto it = myTreeLess.beginPostorder(); it != myTreeLess.endPostorder(); ++it)
    {
        const KeyNode<int, std::string> &node = *it;
        cout << "Key: " << node.getKey() << ", Value: " << node.getValue() << endl;
    }
    cout << "Inorder traversal:" << endl;
    for (auto it = myTreeLess.beginInorder(); it != myTreeLess.endInorder(); ++it)
    {
        const KeyNode<int, std::string> &node = *it;
        cout << "Key: " << node.getKey() << ", Value: " << node.getValue() << endl;
    }

    BPlusTree<int, std::string> bplusTree;

    // Insert key-value pairs
    bplusTree.insert(10, "Value 10");
    bplusTree.insert(20, "Value 20");
    bplusTree.insert(5, "Value 5");
    bplusTree.print();
}
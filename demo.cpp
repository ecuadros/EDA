#include <iostream> 
#include <iostream> 
#include <fstream>  
#include <cmath>
#include <memory>
#include "demo.h"
#include "foreach.h"
#include "types.h"
#include "xtrait.h"
#include <vector>
#include "binarytree.h"



template <typename Node>

void printing(Node *pNode){
     cout << pNode->getData() << "  ";
    }
void Dumie(){}

template <typename Container>
void DemoBinaryTree(Container &container){


    using value_type = typename Container::value_type;
    using LinkedValueType = typename Container::LinkedValueType;
    using Node = typename Container::Node;
     vector<value_type> keys = {30,20,40,15,25,35,50,5,18,45,60};
    //vector<value_type> keys = {40,30,50,25,35,45,60,15,28,55,70};
    //vector<value_type> keys = {10,4,23,12,37,40,6,28};
    //40  30 50 25  35 45 60 15 28 55 70
    // 10,4,23,12,37,40,6,28
    // 30,20,40,15,25,35,50,5,18,45,60
    
    vector<LinkedValueType> values = {-1, -2, -3, -4, -5, -6, -7,-8,-9,-10,-11};
   // vector<LinkedValueType> values = {-1, -2, -3, -4, -5, -6, -7,-8};
    size_t n = keys.size();
    for(size_t i = 0;i<n;i++){
        container.insert(keys[i],values[i]);
    }
   
    auto Dumie = [](auto &x) {};
  

   //container.print(Dumie);

 	cout<<" Inorder Traversal "<<endl;
	 container.inorder([](Node& node) {printing(&node);});

	cout<<endl<<" Postorder Traversal "<<endl;
	container.postorder([](Node& node) {printing(&node);});
	
	cout<<endl<<" Preorder Traversal "<<endl;
	container.preorder([](Node& node) {printing(&node);});
	// container.printPostorder();
   // container.print(cout);
   /*
    cout << "\nTREE (p: parent, v: linked value): " << endl;
    container.print(cout);
    container.print(cout,printAsTree<Node>);
    cout << endl;
    cout << "Recorrido inorden: " << endl;
    container.inorder(cout);
    container.inorder(cout,printAsLine<Node>);
*/ 
}

void ShowTree(){}

void DemoBinaryTree()
{   
    cout <<endl<< "-----------------------------------DemoBinaryTree------------------------------------" << endl;

    BinaryTree< BTreeIntInt> Tree;
    DemoBinaryTree(Tree);
    cout<<endl;
}
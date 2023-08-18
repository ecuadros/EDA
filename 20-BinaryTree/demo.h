#include "array.h"
#include "matrix.h"
#include "foreach.h"
#include "binarytree.h"
using namespace std;

template <typename T, int N>
void DemoArray(){
    ofstream of("output.txt");
    Array<T, N> array1;
    Array<T, N> array2;
    array2 = array1;
    of << array2 << endl; 
    cout << "DemoArray finished!" << endl;

    using TraitStringString = ArrayTrait<string, string, std::less<KeyNode<string, string>&>>;
    CArray<TraitStringString> cArray("Juan Perez");
    cArray.insert("Juan", "Perez");
    cArray.insert("Luis", "Tejada");
    cArray.insert("John", "Doe");
    of << cArray << endl;
}

template <typename Node>
void printAsLine(Node& node, ostream& os){
    os << " --> " << node.getData() << "(" << node.getValue() << ")";
}

template <typename Node>
void printAsTree(Node& node, ostream& os){
    os << string(" | ") * node.getLevel() << node.getData() << "(p:" << (node.getParent()?to_string(node.getParent()->getData()):"Root") << ",v:" << node.getValue() << ")" << endl;
}

template <typename Container>
void DemoBinaryTree(Container& container){

    using value_type = typename Container::value_type;
    using LinkedValueType = typename Container::LinkedValueType;
    using Node = typename Container::Node;
    vector<value_type> keys = {50, 30, 20, 80, 60, 70, 40, 90};
    vector<LinkedValueType> values = {1, 2, 3, 4, 5, 6, 7, 8};
    size_t n = keys.size();
    for(size_t i = 0;i<n;i++){
        container.insert(keys[i],values[i]);
    }

    cout << "\nTREE (p: parent, v: linked value): " << endl;
    container.print(cout,printAsTree<Node>);
    cout << endl;
    cout << "Inorder traversal: " << endl;
    container.inorder(cout,printAsLine<Node>);
    cout << "\nPostorder traversal: " << endl;
    container.postorder(cout,printAsLine<Node>);
    cout << "\nPreorder traversal: " << endl;
    container.preorder(cout,printAsLine<Node>);
    cout << "\n\nVisiting the function in inorder and adding (1) to value: " << endl;
    LinkedValueType value = 1;
    container.inorder([](Node& node, LinkedValueType& value){
        node.getValueRef() = node.getValue() + value;
    },value);
    cout << "Preorder traversal: " << endl;
    container.preorder(cout,printAsLine<Node>);

    cout<<endl;

}

void DemoBinaryTree()
{
    cout << "Hello from DemoBinaryTree()" <<endl;
{   

    cout <<endl<< "-----------------------------------DemoBinaryTree------------------------------------" << endl;

    using traitAsc = BinaryTreeTrait< INT,INT, std::less< NodeBinaryTree< INT,INT > > >;
    using traitDesc = BinaryTreeTrait< INT,INT, std::greater< NodeBinaryTree< INT,INT > > >;

    cout << "---------------------Ascending Binary Tree------------------" << endl;
    BinaryTree< traitAsc > ascBinaryTree;
    DemoBinaryTree(ascBinaryTree);

    cout <<endl<< "---------------------Descending Binary Tree------------------" << endl;
    BinaryTree< traitDesc > descBinaryTree;
    DemoBinaryTree(descBinaryTree);
    cout<<endl;
}

void DemoHash() {
    // Tu código DemoHash aquí
}
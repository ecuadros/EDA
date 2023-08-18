#include "array.h"
#include "matrix.h"
#include "foreach.h"
#include "binarytree.h"
#include "avl.h"
using namespace std;

template <typename T, int N>
void DemoArray() {
    Array<T, N> v1;
    Array<T, N> v2(v1);

    ofstream of("output.txt");
    of << v2 << endl;
    cout << "DemoArray finished!" << endl;

    using TraitStringString = ArrayTrait<string, string, std::less<NodeArray<string, string> &>>;
    using TraitIntDouble = ArrayTrait<int, double, std::less<NodeArray<int, double> &>>;
    Array<TraitStringString> vx("John Doe");
    vx.insert("John", "Doe");
    vx.insert("Jane", "Smith");
    vx.insert("Alice", "Johnson");
    vx.insert("Bob", "Brown");
    vx.insert("Charlie", "Davis");
    vx.insert("Eve", "Wilson");
    vx.insert("Frank", "Taylor");
    of << vx << endl;
    cout << "DemoArray finished!" << endl;
}

template <typename Node>
void printAsLine(Node &node, ostream &os) {
    os << " --> " << node.getData() << "(" << node.getValue() << ")";
}

template <typename Node>
void printAsTree(Node &node, ostream &os) {
    os << string(" | ") * node.getLevel() << node.getData()
       << "(p:" << (node.getParent() ? to_string(node.getParent()->getData()) : "Root") << ",v:" << node.getValue()
       << ",h:" << node.getHeight() << ")" << endl;
}

template <typename Container>
void DemoBinaryTree(Container &container) {
    using value_type = typename Container::value_type;
    using LinkedValueType = typename Container::LinkedValueType;
    using Node = typename Container::Node;

    vector<value_type> keys = {50, 30, 20, 80, 60, 70, 40, 90};
    vector<LinkedValueType> values = {1, 2, 3, 4, 5, 6, 7, 8};
    size_t n = keys.size();
    for (size_t i = 0; i < n; i++) {
        container.insert(keys[i], values[i]);
    }

    cout << "\nTREE (p: parent, v: linked value): " << endl;
    container.print(cout, printAsTree<Node>);
    cout << endl;
    cout << "Inorder traversal: " << endl;
    container.inorder(cout, printAsLine<Node>);
    cout << "\nPostorder traversal: " << endl;
    container.postorder(cout, printAsLine<Node>);
    cout << "\nPreorder traversal: " << endl;
    container.preorder(cout, printAsLine<Node>);
    cout << "\n\nVisiting the function in inorder and adding (1) to value: " << endl;
    LinkedValueType value = 1;
    container.inorder([](Node &node, LinkedValueType &value) {
        node.getValueRef() = node.getValue() + value;
    }, value);
    cout << "Preorder traversal: " << endl;
    container.preorder(cout, printAsLine<Node>);

    cout << endl;
}

void DemoBinaryTree() {
    cout << "Hello from DemoBinaryTree()" << endl;
    {
        cout << endl << "-----------------------------------DemoBinaryTree------------------------------------"
             << endl;

        using traitAsc = BinaryTreeTrait<int, int, std::less<NodeBinaryTree<int, int>>>;
        using traitDesc = BinaryTreeTrait<int, int, std::greater<NodeBinaryTree<int, int>>>;

        cout << "---------------------Ascending Binary Tree------------------" << endl;
        BinaryTree<traitAsc> myAscBinaryTree;
        DemoBinaryTree(myAscBinaryTree);

        cout << endl << "---------------------Descending Binary Tree------------------" << endl;
        BinaryTree<traitDesc> myDescBinaryTree;
        DemoBinaryTree(myDescBinaryTree);
        cout << endl;
    }
}

void DemoAVLTree() {
    cout << endl << "-----------------------------------DemoAVLTree------------------------------------" << endl;
      using traitDesc = BinaryTreeTrait< INT,string, std::greater< NodeBinaryTree< INT,string > > >;

    CAVL< traitDesc > AVLTree;

    using value_type = typename traitDesc::value_type;
    using LinkedValueType = typename traitDesc::LinkedValueType;
    using Node = typename traitDesc::Node;

    vector<value_type> keys = {50,100,150,120,130,40,30,70,60};//all rotations
    vector<LinkedValueType> values = {"a","b","c","d","e","f","g","h","i"};
    size_t n = keys.size();
    for(size_t i = 0;i<n;i++){
        AVLTree.insert(keys[i],values[i]);
    }
    cout << "\nTREE (p: parent, v: linked value, h: height) :" << endl;
    AVLTree.print(cout,printAsTree<Node>);
    cout<<endl;
    cout<<"-------------Reading from test.txt-------------"<<endl;
    ifstream test("test.txt");
    using traitFloatIntDesc = BinaryTreeTrait< TX,INT, std::greater< NodeBinaryTree< TX,INT > > >;
    CAVL< traitFloatIntDesc > AVLTree2;
    test>>AVLTree2;
    cout<<"Printing using << : "<<endl<<AVLTree2<<endl;


}

void DemoHash()
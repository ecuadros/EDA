#include <iostream> // cout
#include <fstream>  // ofstream, ifstream
#include <cmath>
#include <memory>
#include "demo.h"
#include "array.h"
#include "matrix.h"
#include "foreach.h"
#include "linkedlist.h"
#include "doublelinkedlist.h"
#include <random>
using namespace std;

// template <typename T, int N>
// void increment(T &x)
// {  x+= N; }

// template <typename T>
// void print(T &x)
// {  cout << x << "  "; }

// Object function
template <typename T>
class ClassX
{          int m_inc = 0;
    public:  ClassX(int n) : m_inc(n){}
    void operator()(T &n){  n += m_inc;     }
};
template <typename T>
void Fx1(T n ) {    n++;    }
template <typename T>
void Fx2(T &n) {    n++;    }
void Fx3(int *pi){    ++*pi;  pi = nullptr; }
void Fx4(int *&rp){   ++*rp;  rp = nullptr; }

void DemoBasicPointers(){
    int i = 10, j = 20, &r = i; 
    int *b /*Peligro*/, *p = nullptr, *q = nullptr, **pp = nullptr;
    p = &i;     q = &j;     pp = &p;
    float f = 3.14;
    cout << "***** Fx1 *****" << endl;
    Fx1(i);     cout << i << endl;  //  10
    Fx1(15);
    Fx1(*p);    cout << i << endl;  //  10
    Fx1(**pp);  cout << i << endl;  //  10
    Fx1(r);     cout << i << endl;  //  10
    
    cout << "***** Fx2 *****" << endl;
    i = 10;     // r = 10;
    Fx2(i);     cout << i << endl;  // 11
    // Fx2(20);
    // Fx2(i+5);
    // Fx2(i+j);
    // Fx2(f);
    Fx2(r);     cout << i << endl;  // 12

    cout << "***** Fx3 *****" << endl;
    **pp = 10;  // *p = 10;     i = 10;
    *q = 20;    //  j = 20;
    Fx3(p);     cout << i << endl;  // 11
    Fx3(*pp);   cout << i << endl;  // 12
    Fx3(&i);    cout << i << endl;  // 13
    Fx3(q);     cout << j << endl;  // 21
    Fx3(&j);    cout << j << endl;  // 22

    cout << "***** Fx4 *****" << endl;
    p = &i;     q = &j;     pp = &p;
    **pp = 50;  // *p = 10;     i = 10;
    *q   = 60;  //  j = 20;
    Fx4(p);     cout << i << " p: :" << p << endl;  // 51
    p = &i;     // *pp = &i;
    Fx4(*pp);   cout << i << " p: :" << p << endl;  // 52, p: 0x0
    // Fx4(&i);    Error ! es un valor
    Fx4(q);     cout << j << " q: :" << q << endl;  // 61 q: 0x0
    // Fx4(&j);    cout << j << endl;  // 22
}

void DemoSmartPointers(){
    CArray< TraitArrayIntInt > v2("Lucero"), *pX; //, *pV3 = new CArray("Luis");
    
    shared_ptr< CArray< TraitArrayFloatString > > pV3(new CArray< TraitArrayFloatString >("Luis")), pV4;
    pV4 = pV3;
    auto &rA = *pV3;
    for(auto i = 100 ; i < 112 ; i++)
    {   v2.insert(i, i*i);
        pV3->insert(sqrt(i), string("**")+to_string(sqrt(i)+5)+string("**"));
        //  (*pv3).insert(i);
        //  rA.insert(i);
    }
    cout << "Printing pV3 float -> string (greater)" << endl;
    cout << *pV3 ;
}

void DemoDynamicMatrixes(){

    cout << "----------------mat1--------------------" << endl;
    CMatrix<MatrixTraitFloat> mat1(3, 4);
    mat1.fill(1);
    //cout << "k : " << mat1.m_rows<<endl;
    cout << mat1;

    cout << "----------------mat1--------------------" << endl;
    CMatrix<MatrixTraitFloat> mat2(4, 5);
    mat2.fill(2.5);
    cout << mat2;

    cout << "----------------mat1*mat2--------------------" << endl;
    // TODO #1: overload operator*(CMatrix<Traits> &other)
    CMatrix<MatrixTraitFloat> mat3 = mat1 * mat2;
    cout << mat3;

    cout <<endl;
    cout << "----------------Iterator matrix--------------------" << endl;
    // TODO #2: Create Iterator for CMatrix
    cout <<endl<< "----mat1----" << endl;
    foreach(mat1, ::print<TX>);
    cout << endl;

    cout <<endl<< "----mat1 sumando un contador----" << endl;
    TX x = 1;
    // // Lambda function
    // foreach(mat1, [x](TX &n){ n += x; x++; });
    // foreach(mat1, ::print<TX>); cout << endl;
    // ClassX<TX> ope(5);
    // foreach(mat1, ope);
    // foreach(mat1, ::print<TX>); cout << endl;
    // foreach(mat1, ClassX<TX>(8) );
    // foreach(mat1, ::print<TX>); cout << endl;

     // // TODO #3: prepare Matrix to be used as a matrix from outside
    // // overload operator[](size_t row)
    cout << "----------------Operator [] and ()-------------------" << endl;
    mat1[2][3] = 8.2;
    mat1(2, 2) = 7.5; // Operator () is returning a value_type &
    cout << mat1;
    cout << endl;
}

void DemoPreandPostIncrement(){
    int x = 10, y, z;
    y = x++;
    cout << "y=" << y << " x=" << x << endl;
    x = 10;
    z = ++x;
    cout << "z=" << z << " x=" << x << endl;
}

void DemoArray(){   
    cout << "Hello from DemoArray()" <<endl;
    cout << "Vector #1()" <<endl;
    
    CArray< TraitArrayIntInt > v1("Antonio"); 
    for(auto i = 0 ; i < 15 ; i++)
        v1.insert(i, i+5);   //  insert(&v1);

    cout << "Vector #2()" <<endl;
    CArray< TraitFloatLong > v2("Cristian Vera"), 
           *pV3 = new CArray< TraitFloatLong >("Guiomar ABC");
    auto &rA = *pV3;
    for(auto i = 100 ; i < 112 ; i++)
    {   v2.insert(sqrt(i), i);
        pV3->insert(i, sqrt(i));
        //  (*pv3).insert(i);
        //  rA.insert(i);
    }
    cout << "Printing V1 (TraitArrayIntInt)" << endl;
    cout << v1; // v1.print(cout);

    cout << "Printing V2 (TraitFloatLong)" << endl;
    ostream &tmp = cout << v2 << "More text" << endl;
    tmp << "Hola !!!" << endl;
    cout << &tmp << "..." << &cout <<endl;
    // cout << x << f << y << endl;

    cout << "Printing pv3 (TraitFloatLong)" << endl;
    pV3->print(cout);
    // (*pV3).print();     *pV3 is already an object
    // rA.print();          rA is also an object
    // pV3[0].print();      pV3 is also an array with just 1 element [0]
    // (*(pV3+0)).print();
    // (*(0+pV3)).print();
    // 0[pV3].print();
    // delete pV3;

    // Using an array with []
    for(auto i = 0 ; i < v2.size() ; i++)
        cout << "v2[" << i << "] = " << v2[i] << endl;
    ofstream of("test.txt", ios::out);
    of << v2 << endl; 
    cout << "DemoArray finished !" << endl;

    // using TraitStringString = ArrayTrait<string, string  , std::less<NodeArray<string, string> &>>;
    // CArray< TraitStringString > vx("Ernesto Cuadros");
    // vx.insert("Ernesto", "Cuadros");
    // vx.insert("Luis"   , "Tejada");
    // vx.insert("Jorge"  , "Lozano");
    // vx.insert("Edson"  , "Caceres");
    // vx.insert("Franz"  , "Maguiña");
    // vx.print(cout);
}

void DemoIterators(){
    CArray< TraitArrayIntInt > v1("Jorge");
    
    v1.insert(30, 40);
    v1.insert(18, 45);
    v1.insert(20, 35);
    v1.insert(7 , 64);
    v1.insert(12, 25);
    v1.insert(8 , 17);

    cout << v1 << endl;
    // array_forward_iterator<CArray< TraitArrayIntInt >> iter = v1.begin();
    //CArray< TraitArrayIntInt >::iterator iter = v1.begin();
    auto iter = v1.begin();
    foreach(iter, v1.end(), ::increment<TX, 7>);
    cout << v1 << endl;
    foreach(v1, ::increment<TX, 4>);
    cout << v1 << endl;

    foreach(v1, ::print<TX>);
    cout << endl;
    // Lambda function
    int x = 3;
    foreach(v1, [x](TX &n){ n *= 2*x; });
    foreach(v1, ::print<TX>); cout << endl;
    ClassX<TX> ope(5);
    foreach(v1, ope);
    foreach(v1, ::print<TX>); cout << endl;
    foreach(v1, ClassX<TX>(8) );
    foreach(v1, ::print<TX>); cout << endl;
}

void DemoReverseIterators(){
    cout << "DemoReverseIterators: " << endl;
    CArray< TraitArrayIntInt > v1("Edson Cáceres");
    v1.insert(30, 40);
    v1.insert(18, 45);
    v1.insert(20, 35);
    v1.insert(7 , 64);
    v1.insert(12, 25);
    v1.insert(8 , 17);

    cout << "Printing asc : " << endl;
    cout << v1 << endl;
    cout << "Printing desc : " << endl;
    foreach(v1.rbegin(), v1.rend(), ::print<TX>);
}

void DemoHeap()
{
    cout << "Hello from DemoHeap()" <<endl;
}

void DemoBinaryTree()
{
    cout << "Hello from DemoBinaryTree()" <<endl;
}

void DemoHash()
{
    cout << "Hello from DemoHash()" <<endl;
}

template <typename Container>
void demoLinkedList(Container &mylist)
{
    cout<< "Hello LinkedLis"<<endl;
    cout << "Inserting:       "<<endl;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1,60);
    vector<int> vect={};
    for(auto x=0; x<20; x++) { 
      //cout<<"Ins"<<endl;  
      int tmp= dist(gen);
      cout << tmp << ", "; 
      mylist.insert(tmp,tmp);
    }
    // mylist.push_front(99,3);
    // mylist.push_back(99,0);
    cout << endl;
    cout << "Lista en orden: ";
    mylist.print(cout);
    cout<<"\nMy size is: "<<mylist.size()<<endl;
    for(size_t pos = 0; pos < mylist.size(); pos++)
       cout << mylist[pos] << "-->";
    cout<<endl;   
    cout<<"Test Foreach"<<endl;
    using T = typename Container::value_type;
    foreach(mylist, Fx2<T>);  cout << endl;
    mylist.print(cout);
    cout<<"\nTest overload <<"<<endl;
    cout<<mylist;
    cout<<"\nReading External File"<<endl;
    ifstream of("file.txt",ios::in);
    of>>mylist;
    cout<<"\nPrint Value of MyList Update"<<endl;
    mylist.print(cout);
    cout<<"\nTest Read Function"<<endl;
    mylist.Read("file.txt");
    mylist.print(cout);
    cout<<"\nTest Write Function"<<endl;
    mylist.Write();

}

void demoLinkedListSorted()
{
    cout << "Ascending list" << endl;
    LinkedList<Traits_LLAsc> myAscList;
    demoLinkedList(myAscList);
    cout<<"\nTestForeach Final"<<endl;
    foreach(myAscList);
    cout<<endl;
    cout << "\nDescending list" << endl;
    LinkedList<Traits_LLDesc> myDescList;
    demoLinkedList(myDescList);
    cout<<"\nTestForeach Final"<<endl;
    foreach(myDescList);
}

template <typename Container>
void demoDoubleLinkedList(Container &mylist){
    cout<< "Hello Double LinkedList"<<endl;
    cout << "Inserting:       "<<endl;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1,60);
    vector<int> vect={26,30,27,35,3,1,100};
    for(auto x=0; x<=10; x++) { 
      int tmp= dist(gen);
      cout << tmp << ", "; 
      mylist.insert(tmp,tmp);
        // cout<<vect[x]<<",";
        // mylist.insert(vect[x],vect[x]);
    }
    //mylist.push_front(99,3);
    //mylist.push_back(99,0);
    cout << endl;
    cout << "Run Begin to End: ";
    mylist.print(cout);
    cout << endl;
    cout << "Run End to Begin: ";
    mylist.r_print(cout);
    cout << endl;
    cout<<"Test Foreach"<<endl;
    using T = typename Container::value_type;
    foreach(mylist, Fx2<T>);  cout << endl;
    foreach(mylist, print<T>);  cout << endl;
    cout<<"\nTest ForeachReverse"<<endl;
    using T = typename Container::value_type;
    foreach_reverse(mylist, Fx2<T>);  cout << endl;
    foreach_reverse(mylist, print<T>);  cout << endl;
    cout<<"\nTest overload <<"<<endl;
    cout<<mylist;
    cout<<"\nReading External File"<<endl;
    ifstream of("file.txt",ios::in);
    of>>mylist;
    cout<<"Print Value of MyList Update"<<endl;
    mylist.print(cout);
    cout<<"\nMy size is: "<<mylist.size()<<endl;
    cout<<"Overload operator []"<<endl;
    for(size_t pos = 0; pos < mylist.size(); pos++)
       cout << mylist[pos] << "-->";
    cout<<endl;  
}


void demoDoubleLinkedListSorted()
{
    cout << "Ascending double list" << endl;
    DoubleLinkedList<Traits_DLLAsc> myAscList;
    demoDoubleLinkedList(myAscList);
    cout<<"\nTest Foreach some Functions"<<endl;
    foreach(myAscList);
    cout<<endl;
    cout << "Descending double list" << endl;
    DoubleLinkedList<Traits_DLLDesc> myDescList;
    demoDoubleLinkedList(myDescList); 
    cout<<"\nTest Foreach some Functions"<<endl;
    foreach(myDescList);
    cout<<endl;
}

// template <typename Container>
// void DemoBinaryTree(Container &container)
// {   using T = typename Container::value_type;
//     vector<T> values = {50, 30, 20, 80, 60, 70, 40, 90};
//     for(auto &v: values)
//     {
//         container.insert(v);
//     }    
//     cout << endl;
//     cout << "Recorrido inorden: " << endl;
//     container.inorder(cout);
//     cout << "\nRecorrido postorden: " << endl;
//     container.postorder(cout);
//     cout << "\nRecorrido preorden: " << endl;
//     container.preorder(cout);
//     cout << "\nTREE: " << endl;
//     container.print(cout);

//     /*
//     // aplicando función
//     cout << "Recorrido aplicando funci'on duplicar (recorrido inorden) " << endl;               
//     //container.inorder(duplicate);
//     cout << "Aplicando funci'on imprimir: " << endl;               
//     //container.inorder(printTree);
//     */
// }

// #include "binarytree.h"
// void DemoBinaryTree()
// {   
//     cout << "Ascending Binarytree ..." << endl;
//     BinaryTree< BinaryTreeAscTraits<TX> > myAscBinaryTree;
//     DemoBinaryTree(myAscBinaryTree);
    
//     cout << "Descending Binarytree ..." << endl;
//     BinaryTree< BinaryTreeDescTraits<TX> > myDescBinaryTree;
//     DemoBinaryTree(myDescBinaryTree);
// }

// #include "btree.h"
// void DemoTree()
// {
//     BTree < BtreeTrait<char,long> > bt;
//     const char * keys = "DYZakHIUwxVJ203ejOP9Qc8AdtuEop1XvTRghSNbW567BfiCqrs4FGMyzKLlmn";
//     for(size_t i = 0; keys[i]; i++)
//         {
//             //cout<<"Inserting "<<keys[i]<<endl;
//             //result = bt.Insert(keys4[i], i*i);
//             bt.Insert(keys[i], i*i);
//             //bt.Print(cout);
//         }
//     bt.Print(cout);
//     exit(0);

// }


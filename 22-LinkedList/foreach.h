#include <iostream>

template <typename T>
void f1(T &x)
{
    x += 5;
}

template <typename Node>
void printLine(Node &node)
{
    std::cout << "{" << node.getData() << ":" << node.getValue() << "}" << ", ";
}

template <typename Iterator, typename F>
void foreach(Iterator ItBegin, Iterator ItEnd, F ope)
{
    while (ItBegin != ItEnd)
    {
        ope(*ItBegin);
        ++ItBegin;
    }
}

int main()
{
    int arr[] = {1, 2, 3, 4, 5};
    foreach(std::begin(arr), std::end(arr), f1<int>);
    foreach(std::begin(arr), std::end(arr), printLine<int>);

    return 0;
}
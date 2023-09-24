#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <iostream>
#include <set>
#include <map>
#include <stack>
#include <algorithm> // swap algorithm
#include <cassert>
#include <vector>
#include "types.h"
#include "keynode.h"
#include "xtrait.h"
#include "array.h"

using namespace std;

template <typename Traits>
class CGraphVertex
{
public:
    using value_type = typename Traits::value_type;
    using KeyType = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node = typename Traits::Node;
    using CompareFn = typename Traits::CompareFn;
    using myself = CGraphVertex<Traits>;

public:
    CGraphVertex() {}
    CGraphVertex(const value_type &key, LinkedValueType value) : m_key(key), m_value(value) {}
    virtual ~CGraphVertex() {}

    vector<value_type> &getEdges()
    {
        return m_edges;
    }

    constexpr operator LinkedValueType&() noexcept {
        return m_value;
    }

private:
    value_type m_key;
    LinkedValueType m_value;
    vector<value_type> m_edges;
};

// Created by: @ecuadros
template <typename Traits>
class CGraph
{
public:
    using value_type = typename Traits::value_type;
    using KeyType = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node = typename Traits::Node;
    using CompareFn = typename Traits::CompareFn;
    using myself = CGraph<Traits>;
    using Vertex = CGraphVertex<Traits>;

private:
    string m_name = "Empty";
    // DONE definir colección de nodes
    map<value_type, Vertex> m_vertices;

public:
    CGraph(string name) : m_name(name) { destroy(); }
    CGraph() { destroy(); }

    virtual ~CGraph()
    {
        cerr << "Destroying " << m_name << "..." << endl;
        // reset();
    }

    void destroy()
    {
    }

    // CGraph operator=(CGraph &other){
    //     destroy();

    // }

    // DONE: Replantear, en lugar de insert, 2 funciones
    // insertVertex y insertEdge
    void insertVertex(const value_type &key, LinkedValueType value)
    {
        m_vertices[key] = Vertex(key, value);
    }

    // Non directed edge
    void insertEdge(const value_type &key1, const value_type &key2)
    {
        assert(m_vertices.find(key1) != m_vertices.end());
        assert(m_vertices.find(key2) != m_vertices.end());
        m_vertices[key1].getEdges().push_back(key2);
        m_vertices[key2].getEdges().push_back(key1);
    }

    void print(ostream &os)
    {
        os << "Graph (" << m_name << "):" << endl;
        os << "Edges: " << endl;
        for (auto &vertex : m_vertices)
        {
            os << '[' << vertex.first << ']' << " -> ";
            for (auto &edge : vertex.second.getEdges())
            {
                os << edge << " ";
            }
            os << endl;
        }

        os << "Vertices (key, value): " << endl;
        for (auto &vertex : m_vertices)
        {
            // Implicit LinkedValueType cast on vertex.second
            os << '[' << vertex.first << ']' << " -> " << vertex.second << endl;
        }
    }

    void readVertices(istream &is)
    {
        size_t vertex_count;
        is >> vertex_count;
        for (size_t i = 0; i < vertex_count; i++)
        {
            value_type key;
            LinkedValueType value;
            is >> key >> value;
            insertVertex(key, value);
        }
    }

    void readEdges(istream &is)
    {
        size_t edge_count;
        is >> edge_count;
        for (size_t i = 0; i < edge_count; i++)
        {
            value_type key1, key2;
            is >> key1 >> key2;
            insertEdge(key1, key2);
        }
    }

    void read(istream &is)
    {
        readVertices(is);
        readEdges(is);
    }

    size_t size()
    {
        return m_vertices.size();
    }

    // DONE : agregar el operator LinkedValueType &
    LinkedValueType &operator[](value_type pos)
    {
        return m_vertices[pos];
    }

    template <typename T>
    friend ostream &operator<<(ostream &os, CGraph<T> &graph);

    template <typename T>
    friend istream &operator>>(istream &is, CGraph<T> &graph);
};


template <typename T>
ostream &operator<<(ostream &os, CGraph<T> &graph)
{
    graph.print(os);
    return os;
}

template <typename T>
istream &operator>>(istream &is, CGraph<T> &graph)
{
    graph.read(is);
    return is;
}

#endif

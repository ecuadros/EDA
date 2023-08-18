#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

template <typename T1, typename T2>
class Node {
private:
    T1 m_data;
    T2 m_value;
public:
    Node(T1 data = T1(), T2 value = T2()) : m_data(data), m_value(value) {}

    T1 getData() const {
        return m_data;
    }

    T2 getValue() const {
        return m_value;
    }

    T1& getDataRef() {
        return m_data;
    }

    void setData(const T1& data) {
        m_data = data;
    }

    void setValue(const T2& value) {
        m_value = value;
    }
};

template <typename T1, typename T2>
class CArray {
private:
    typedef Node<T1, T2> Node;
    Node* m_pVect;
    size_t m_vcount;
    size_t m_vmax;
    string m_name;

public:
    CArray(size_t size = 0, const string& name = "") : m_name(name), m_vcount(0), m_vmax(size) {
        m_pVect = new Node[m_vmax];
    }

    ~CArray() {
        delete[] m_pVect;
    }

    void insert(const T1& key, const T2& value) {
        if (m_vcount == m_vmax) {
            size_t newVmax = m_vmax * 2 + 1;
            Node* newVect = new Node[newVmax];
            for (size_t i = 0; i < m_vcount; ++i) {
                newVect[i] = m_pVect[i];
            }
            delete[] m_pVect;
            m_pVect = newVect;
            m_vmax = newVmax;
        }
        m_pVect[m_vcount++] = Node(key, value);
    }

    Node back() const {
        return m_pVect[m_vcount - 1];
    }

    void pop_back() {
        --m_vcount;
    }

    void resize(size_t newSize) {
        if (newSize < m_vcount) {
            m_vcount = newSize;
        } else if (newSize > m_vmax) {
            Node* newVect = new Node[newSize];
            for (size_t i = 0; i < m_vcount; ++i) {
                newVect[i] = m_pVect[i];
            }
            delete[] m_pVect;
            m_pVect = newVect;
            m_vmax = newSize;
        }
    }

    void destroy() {
        delete[] m_pVect;
        m_pVect = nullptr;
        m_vcount = 0;
        m_vmax = 0;
    }

    void print(ostream& os) const {
        os << m_vcount << " " << m_vmax << endl;
        for (size_t i = 0; i < m_vcount; ++i) {
            os << m_pVect[i].getData() << "\t: " << m_pVect[i].getValue() << endl;
        }
    }

    size_t size() const {
        return m_vcount;
    }

    Node& operator[](size_t pos) {
        return m_pVect[pos];
    }

    const Node& operator[](size_t pos) const {
        return m_pVect[pos];
    }

    class iterator {
    private:
        CArray* m_array;
        Node* m_current;

    public:
        iterator(CArray* arr, Node* ptr) : m_array(arr), m_current(ptr) {}

        iterator& operator++() {
            ++m_current;
            return *this;
        }

        bool operator!=(const iterator& other) const {
            return m_current != other.m_current;
        }

        Node& operator*() {
            return *m_current;
        }
    };

    iterator begin() {
        iterator iter(this, m_pVect);
        return iter;
    }

    iterator end() {
        iterator iter(this, m_pVect + m_vcount);
        return iter;
    }
};
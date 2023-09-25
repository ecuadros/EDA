#ifndef __BPLUS_H__
#define __BPLUS_H__

#include <iostream>
#include <algorithm> // swap algorithm
#include <cassert>
#include <iostream>
#include "types.h"
#include "keynode.h"
#include "xtrait.h"
#include "array.h"

using namespace std;

template <typename Container>
class forward_iterator
{
  public: 
    typedef typename Container::Node Node;
    typedef forward_iterator<Container> myself;
  protected:
    Container *m_pContainer;
    Node *m_pNode;
  public:
    forward_iterator(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode) {}
    forward_iterator(myself &other)  : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) {}
    forward_iterator(myself &&other) {   
      m_pContainer = move(other.m_pContainer);
      m_pNode      = move(other.m_pNode);
    }

  public:
    myself operator++() 
    {
      m_pNode = m_pNode->getpNext();  
      return *this;
    }

    forward_iterator operator=(forward_iterator &iter)
    {   
      m_pContainer = move(iter.m_pContainer);
      m_pNode      = move(iter.m_pNode);
      return *(forward_iterator *)this;
    }

    bool operator==(forward_iterator iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(forward_iterator iter)   { return !(*this == iter);        }
    Node *operator*() { return m_pNode;}

};

template <typename T, typename V>
class BPlusNode
{
public:
    typedef BPlusNode<T,V> Node;
    using value_type = T;
    using LinkedValueType = V;
    int *keys;
	int t;
	Node **C;
    Node *m_pNext;
	int n;
	bool leaf;
    KeyNode<T,V> m_data;

    BPlusNode(KeyNode<T, V> data, Node *pNext = nullptr)
      : m_data(data), m_pNext(pNext){};
    BPlusNode(value_type key, LinkedValueType valor, Node *pNext = nullptr){
        m_data = KeyNode(key, valor);
        m_pNext = pNext;
      }
    value_type getKey() { return m_data.getData(); }
    value_type &getKeyRef() { return m_data.getDataRef(); }

    LinkedValueType getValue(){ return m_data.getValue(); }
    LinkedValueType &getValueRef(){ return m_data.getValueRef(); }

	BPlusNode(int _t, bool _leaf);
	void write(ostream &os);

	Node *search(int k); 
	int findKey(int k);
	void insertNonFull(int k);
	void splitChild(int i, Node *y);

	void remove(LinkedValueType k);
	void removeFromLeaf(LinkedValueType idx);
	void removeFromNonLeaf(LinkedValueType idx);

	int getPred(int idx);
	int getSucc(int idx);
	void fill(int idx);
	void borrowFromPrev(int idx);
	void borrowFromNext(int idx);

	void merge(int idx);

    Node *getpNext(){return getpNextRef();}
    Node *&getpNextRef(){return m_pNext;}
};

template <typename _T, typename _V>
struct CBPlusTrait
{
    using  T         = _T;
    using  V         = _V;
    using  Node      = BPlusNode<T,V>;
    using  LinkedValueType = _V;
};

template <typename Traits>
class CBPlus
{
public:
    typedef typename Traits::Node Node;
    typedef typename Traits::T value_type;
    typedef typename Traits::V LinkedValueType;
    typedef CBPlus<Traits> myself;
    typedef forward_iterator<myself> iterator;
    Node *root;
	int t;

	CBPlus(int _t)
	{
		root = NULL;
		t = _t;
	}

	void write(ostream &os)
	{
		if (root != NULL) root->write(os);
	}

	Node* search(int k)
	{
		return (root == NULL)? NULL : root->search(k);
	}

	void insert(value_type v,LinkedValueType k);
	void remove(LinkedValueType k);

    //TODO: Tarea 42: B+: read
    void read(istream &is)
    {
        int filas;
        is >> filas;
        value_type key;
        LinkedValueType value;
        string puntuacion;
        while (filas--)
        {
        is >> key;
        is >> puntuacion;
        is >> value;
        this->insert(key, value);
        }
    }

    iterator begin() { iterator iter(this, root);return iter;}
    iterator end()   { iterator iter(this, nullptr);return iter;}

};

template <typename T, typename V>
BPlusNode<T, V>::BPlusNode(int t1, bool leaf1)
{
	t = t1;
	leaf = leaf1;

	keys = new int[2*t-1];
	C = new BPlusNode *[2*t];
	n = 0;
}

template <typename T, typename V>
int BPlusNode<T, V>::findKey(int k)
{
	int idx=0;
	while (idx<n && keys[idx] < k)
		++idx;
	return idx;
}

//TODO: Tarea 40: B+: remove
template <typename T, typename V>
void BPlusNode<T, V>::remove(LinkedValueType k)
{
	int idx = findKey(k);
	if (idx < n && keys[idx] == k)
	{
		if (leaf)
			removeFromLeaf(idx);
		else
			removeFromNonLeaf(idx);
	}
	else
	{
		if (leaf)
		{
			cout<<"La llave "<<k<<" no existe en el arbol\n";
			return;
		}

		bool flag = ( (idx==n)? true : false );

		if (C[idx]->n < t)
			fill(idx);

		if (flag && idx > n)
			C[idx-1]->remove(k);
		else
			C[idx]->remove(k);
	}
	return;
}

//TODO: Tarea 40: B+: remove
template <typename T, typename V>
void BPlusNode<T,V>::removeFromLeaf (LinkedValueType idx)
{

	for (int i=idx+1; i<n; ++i)
		keys[i-1] = keys[i];
	n--;

	return;
}

//TODO: Tarea 40: B+: remove
template <typename T, typename V>
void BPlusNode<T,V>::removeFromNonLeaf(LinkedValueType idx)
{

	int k = keys[idx];

	if (C[idx]->n >= t)
	{
		int pred = getPred(idx);
		keys[idx] = pred;
		C[idx]->remove(pred);
	}

	else if (C[idx+1]->n >= t)
	{
		int succ = getSucc(idx);
		keys[idx] = succ;
		C[idx+1]->remove(succ);
	}

	else
	{
		merge(idx);
		C[idx]->remove(k);
	}
	return;
}

template <typename T, typename V>
int BPlusNode<T,V>::getPred(int idx)
{
	BPlusNode *cur=C[idx];
	while (!cur->leaf)
		cur = cur->C[cur->n];

	return cur->keys[cur->n-1];
}

template <typename T, typename V>
int BPlusNode<T,V>::getSucc(int idx)
{
	BPlusNode *cur = C[idx+1];
	while (!cur->leaf)
		cur = cur->C[0];
	
    return cur->keys[0];
}

template <typename T, typename V>
void BPlusNode<T,V>::fill(int idx)
{
	if (idx!=0 && C[idx-1]->n>=t)
		borrowFromPrev(idx);

	else if (idx!=n && C[idx+1]->n>=t)
		borrowFromNext(idx);

	else
	{
		if (idx != n)
			merge(idx);
		else
			merge(idx-1);
	}
	return;
}

template <typename T, typename V>
void BPlusNode<T,V>::borrowFromPrev(int idx)
{

	BPlusNode *child=C[idx];
	BPlusNode *sibling=C[idx-1];

	for (int i=child->n-1; i>=0; --i)
		child->keys[i+1] = child->keys[i];

	if (!child->leaf)
	{
		for(int i=child->n; i>=0; --i)
			child->C[i+1] = child->C[i];
	}

	child->keys[0] = keys[idx-1];

	if(!child->leaf)
		child->C[0] = sibling->C[sibling->n];

	keys[idx-1] = sibling->keys[sibling->n-1];

	child->n += 1;
	sibling->n -= 1;

	return;
}

template <typename T, typename V>
void BPlusNode<T,V>::borrowFromNext(int idx)
{

	BPlusNode *child=C[idx];
	BPlusNode *sibling=C[idx+1];

	child->keys[(child->n)] = keys[idx];

	if (!(child->leaf))
		child->C[(child->n)+1] = sibling->C[0];

	keys[idx] = sibling->keys[0];

	for (int i=1; i<sibling->n; ++i)
		sibling->keys[i-1] = sibling->keys[i];

	if (!sibling->leaf)
	{
		for(int i=1; i<=sibling->n; ++i)
			sibling->C[i-1] = sibling->C[i];
	}

	child->n += 1;
	sibling->n -= 1;

	return;
}

template <typename T, typename V>
void BPlusNode<T,V>::merge(int idx)
{
	BPlusNode *child = C[idx];
	BPlusNode *sibling = C[idx+1];

	child->keys[t-1] = keys[idx];

	for (int i=0; i<sibling->n; ++i)
		child->keys[i+t] = sibling->keys[i];

	if (!child->leaf)
	{
		for(int i=0; i<=sibling->n; ++i)
			child->C[i+t] = sibling->C[i];
	}

	for (int i=idx+1; i<n; ++i)
		keys[i-1] = keys[i];

	for (int i=idx+2; i<=n; ++i)
		C[i-1] = C[i];

	child->n += sibling->n+1;
	n--;

	delete(sibling);
	return;
}

//TODO: Tarea 39: B+: Insert
template <typename Traits>
void CBPlus<Traits>::insert(value_type v,LinkedValueType k)
{
	if (root == NULL)
	{
		root = new Node(t, true);
		root->keys[0] = k;
		root->n = 1;
	}
	else
	{
		if (root->n == 2*t-1)
		{
			Node *s = new Node(t, false);

			s->C[0] = root;

			s->splitChild(0, root);

			int i = 0;
			if (s->keys[0] < k)
				i++;
			s->C[i]->insertNonFull(k);

			root = s;
		}
		else
			root->insertNonFull(k);
	}
}

//TODO: Tarea 39: B+: Insert
template <typename T, typename V>
void BPlusNode<T,V>::insertNonFull(int k)
{
	int i = n-1;

	if (leaf == true)
	{
		while (i >= 0 && keys[i] > k)
		{
			keys[i+1] = keys[i];
			i--;
		}

		keys[i+1] = k;
		n = n+1;
	}
	else
	{
		while (i >= 0 && keys[i] > k)
			i--;

		if (C[i+1]->n == 2*t-1)
		{
			splitChild(i+1, C[i+1]);

			if (keys[i+1] < k)
				i++;
		}
		C[i+1]->insertNonFull(k);
	}
}

template <typename T, typename V>
void BPlusNode<T,V>::splitChild(int i, BPlusNode *y)
{
	BPlusNode *z = new BPlusNode(y->t, y->leaf);
	z->n = t - 1;

	for (int j = 0; j < t-1; j++)
		z->keys[j] = y->keys[j+t];

	if (y->leaf == false)
	{
		for (int j = 0; j < t; j++)
			z->C[j] = y->C[j+t];
	}

	y->n = t - 1;

	for (int j = n; j >= i+1; j--)
		C[j+1] = C[j];

	C[i+1] = z;

	for (int j = n-1; j >= i; j--)
		keys[j+1] = keys[j];

	keys[i] = y->keys[t-1];

	n = n + 1;
}

//TODO: Tarea 43: B+ write
template <typename T, typename V>
void BPlusNode<T,V>::write(ostream &os)
{
	int i;
	for (i = 0; i < n; i++)
	{
		if (leaf == false)
			C[i]->write(os);
		os << " " << keys[i];
	}

	if (leaf == false)
		C[i]->write(os);
}

template <typename T, typename V>
BPlusNode<T,V> *BPlusNode<T,V>::search(int k)
{
	int i = 0;
	while (i < n && k > keys[i])
		i++;

	if (keys[i] == k)
		return this;

	if (leaf == true)
		return NULL;

	return C[i]->search(k);
}

//TODO: Tarea 40: B+: remove
template <typename Traits>
void CBPlus<Traits>::remove(LinkedValueType k)
{
	if (!root)
	{
		cout << "El arbol esta vacio\n";
		return;
	}

	root->remove(k);

	if (root->n==0)
	{
		Node *tmp = root;
		if (root->leaf)
			root = NULL;
		else
			root = root->C[0];

		delete tmp;
	}
	return;
}

//TODO: Tarea 43: B+ write
template <typename T>
ostream &operator<<(ostream &os, CBPlus<T> &arbol)
{
    arbol.write(os);
    return os;
}

//TODO: Tarea 42: B+: read
template <typename T>
istream &operator>>(istream &is, CBPlus<T> &arbol)
{
    arbol.read(is);
    return is;
}

#endif
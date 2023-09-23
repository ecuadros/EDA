#ifndef __CBTreePage_H__
#define __CBTreePage_H__

#include <vector>
#include <assert.h>
#include <iostream>
#include <functional>
#include "iterator.h"
#include <stack>

// DONE: #1 Crear una function para agregarla al demo.cpp ( no trivial )
// DemoBTree.cpp, muestra la funcionalidad y el iterador creado

// DONE: #2 Agregarle un Trait (prueba git) ( no trivial )
// Ahora CBTreePage puede tomar el Trait XTrait o sus variantes,
// definidas en xtrait.h

// DONE: #3 crear un iterator ( no trivial )
//       Sugerencia: Tarea1 cada pagina debe tener un puntero al padre primero ( no trivial )
//       En lugar de esto se uso un stack

// DONE: #4 integrarlo al recorrer ( no trivial )
// Ya integrado, se ve en el demo

template <typename Trait>
class BTree;

using namespace std;
enum bt_ErrorCode
{
    bt_ok,
    bt_overflow,
    bt_underflow,
    bt_duplicate,
    bt_nofound,
    bt_rootmerged
};

template <typename Container>
class btree_forward_iterator
{
public:
    typedef typename Container::Node Node;
    typedef btree_forward_iterator<Container> myself;
    typedef typename Node::value_type         NodeType;

protected:
    Container *m_pContainer;
    Node *m_pNode;

public:
    btree_forward_iterator(Container *pContainer, Node *pNode)
        : m_pContainer(pContainer), m_pNode(pNode)
    {
        if (pNode)
        {
            m_stack.push(pNode);
            m_index_stack.push(0);
            this->m_pNode = pNode;
            addToStack();
        }
    }
    btree_forward_iterator(myself &other) : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) {}
    btree_forward_iterator(myself &&other)
    {
        m_pContainer = move(other.m_pContainer);
        m_pNode = move(other.m_pNode);
    }

    btree_forward_iterator operator++()
    {
        if (m_stack.empty())
        {
            this->m_pNode = nullptr;
            return *this;
        }

        this->m_index_stack.top()++;
        addToStack();
        if (this->m_index_stack.top() >= this->m_stack.top()->m_KeyCount)
        {
            this->m_pNode = nullptr;
        }
        return *this;
    }

    void addToStack()
    {
        while (!m_stack.empty())
        {
            // Si no es una hoja, bajar
            bool esHoja = true;
            for (size_t i = 0; i < this->m_pNode->m_SubPages.size(); i++)
            {
                if (this->m_pNode->m_SubPages[i])
                {
                    esHoja = false;
                    break;
                }
            }
            // cout<<"Hoja " << esHoja<< " "<< (**this) << " " <<m_index_stack.top() << " "<< this->m_pNode->m_KeyCount<<endl;

            if (!esHoja)
            {
                this->m_pNode = this->m_pNode->m_SubPages[m_index_stack.top()];
                if (this->m_pNode)
                    m_stack.push(this->m_pNode);
                m_index_stack.push(0);
            }

            // Si es una hoja y quedan keys por visitar
            else if (m_index_stack.top() < this->m_pNode->m_KeyCount)
            {
                return;
            }

            // Si es una hoja y no queda nada por visitar
            else
            {
                m_stack.pop();
                m_index_stack.pop();
                // m_index_stack.top()++;
                if (!m_stack.empty())
                {
                    this->m_pNode = m_stack.top();
                }
                else
                {
                    this->m_pNode = nullptr;
                }
                return;
            }
        }
    }
    using Type = typename Container::ObjectInfo;
    typename Container::ObjectInfo &operator*()
    {
        size_t topIdx = this->m_index_stack.top();
        return this->m_pNode->m_Keys[topIdx];
    }
    bool operator==(const myself &other) const
    {
        return this->m_pNode == other.m_pNode;
    }
    bool operator!=(const myself &other) const
    {
        return !(*this == other);
    }

    myself operator=(myself &iter)
          {   m_pContainer = move(iter.m_pContainer);
              m_pNode      = move(iter.m_pNode);
              return *(myself *)this; // Pending static_cast?
          }
private:
    stack<Node *> m_stack;
    stack<size_t> m_index_stack;
};

#define MINUS_ONE 10000

template <typename Container>
class btree_backward_iterator
    : public general_iterator<Container, class btree_backward_iterator<Container>> //
{
    _ITER_TYPEDEFS(Container, btree_backward_iterator)
public:
    btree_backward_iterator(Container *pContainer, Node *pNode)
        : Parent(pContainer, pNode)
    {
        if (pNode)
        {
            m_stack.push(pNode);
            m_index_stack.push(pNode->m_KeyCount - 1);
            this->m_pNode = pNode;
            addToStack();
        }
    }
    btree_backward_iterator(myself &other) : Parent(other) {}
    btree_backward_iterator(myself &&other) : Parent(move(other)) {}
    btree_backward_iterator operator++()
    {
        if (m_stack.empty())
        {
            this->m_pNode = nullptr;
            return *this;
        }
        if(this->m_index_stack.top() == 0)
        {
            this->m_index_stack.top() = MINUS_ONE;
        } else {
            this->m_index_stack.top()--;
        }
        addToStack();
        if (this->m_index_stack.top() == MINUS_ONE)
        {
            this->m_pNode = nullptr;
        }
        return *this;
    }

    void addToStack()
    {
        while (!m_stack.empty())
        {

            bool esHoja = true;
            for (size_t i = 0; i < this->m_pNode->m_SubPages.size(); i++)
            {
                if (this->m_pNode->m_SubPages[i])
                {
                    esHoja = false;
                    break;
                }
            }

            // Si no es una hoja, bajar
            if (!esHoja)
            {
                if(m_index_stack.top() == MINUS_ONE) {
                    this->m_pNode = this->m_pNode->m_SubPages[0];
                } else {
                    this->m_pNode = this->m_pNode->m_SubPages[m_index_stack.top()+1];
                }
                if (this->m_pNode)
                    m_stack.push(this->m_pNode);
                m_index_stack.push(this->m_pNode->m_KeyCount - 1);
            }

            // Si es una hoja y quedan keys por visitar
            else if (m_index_stack.top() != MINUS_ONE)
            {
                return;
            }

            // Si es una hoja y no queda nada por visitar
            else
            {
                m_stack.pop();
                m_index_stack.pop();
                // m_index_stack.top()++;
                if (!m_stack.empty())
                {
                    this->m_pNode = m_stack.top();
                }
                else
                {
                    this->m_pNode = nullptr;
                }
                return;
            }
        }
    }
    using Type = typename Container::ObjectInfo;
    typename Container::ObjectInfo &operator*()
    {
        size_t topIdx = this->m_index_stack.top();
        return this->m_pNode->m_Keys[topIdx];
    }
    bool operator==(const myself &other) const
    {
        return this->m_pNode == other.m_pNode;
    }
    bool operator!=(const myself &other) const
    {
        return !(*this == other);
    }

private:
    stack<Node *> m_stack;
    stack<size_t> m_index_stack;
};

template <typename Node>
struct NodeInfo
{
    size_t level;
    size_t pos = 0;
    Node *pPage;
    NodeInfo(size_t _level, Node *_pPage) : level(_level), pPage(_pPage) {}
};

template <typename Container>
class btree_print_iterator
    : public general_iterator<Container, class btree_print_iterator<Container>> //
{
    _ITER_TYPEDEFS(Container, btree_print_iterator)
public:
    btree_print_iterator(Container *pContainer, Node *pNode)
        : Parent(pContainer, pNode)
    {
        if (pNode)
        {
            m_stack.push(NodeInfo(0, pNode));
            m_index_stack.push(0);
            this->m_pNode = pNode;
            addToStack();
        }
    }
    btree_print_iterator(myself &other) : Parent(other) {}
    btree_print_iterator(myself &&other) : Parent(move(other)) {}
    btree_print_iterator operator++()
    {
        if (m_stack.empty())
        {
            this->m_pNode = nullptr;
            return *this;
        }

        this->m_index_stack.top()++;
        addToStack();
        if (this->m_index_stack.top() >= this->m_stack.top().pPage->m_KeyCount)
        {
            // cout << (**this);
            this->m_pNode = nullptr;
        }
        return *this;
    }

    void addToStack()
    {
        while (!m_stack.empty())
        {
            // Si no es una hoja, bajar
            bool esHoja = true;
            for (size_t i = 0; i < this->m_pNode->m_SubPages.size(); i++)
            {
                if (this->m_pNode->m_SubPages[i])
                {
                    esHoja = false;
                    break;
                }
            }
            if (!esHoja)
            {
                this->m_pNode = this->m_pNode->m_SubPages[m_index_stack.top()];
                m_stack.push(NodeInfo(m_stack.top().level + 1, this->m_pNode));
                m_index_stack.push(0);
            }
            // Si es una hoja y quedan keys por visitar
            else if (m_index_stack.top() < this->m_pNode->m_KeyCount)
            {
                return;
            }
            // Si es una hoja y no queda nada por visitar
            else
            {
                m_stack.pop();
                m_index_stack.pop();
                // m_index_stack.top()++;
                if (!m_stack.empty())
                {
                    this->m_pNode = m_stack.top().pPage;
                }
                else
                {
                    this->m_pNode = nullptr;
                }
                return;
            }
        }
    }
    using Type = typename Container::ObjectInfo;

    typename Container::ObjectInfo &getObjectInfo()
    {
        size_t topIdx = this->m_index_stack.top();
        return this->m_pNode->m_Keys[topIdx];
    }

    NodeInfo<Node>& operator*()
    {
        NodeInfo<Node>& ret = this->m_stack.top();
        ret.pos = this->m_index_stack.top();
        return ret;
    }

    bool operator==(const myself &other) const
    {
        bool eq = this->m_pNode == other.m_pNode;
        return eq;
    }
    bool operator!=(const myself &other) const
    {
        return !(*this == other);
    }

private:
    stack<NodeInfo<Node>> m_stack;
    stack<size_t> m_index_stack;
};

template <typename Container, typename ObjType>
size_t binary_search(Container &container, size_t first, size_t last, ObjType &object)
{
    if (first >= last)
        return first;
    while (first < last)
    {
        size_t mid = (first + last) / 2;
        if (object == (ObjType)container[mid])
            return mid;
        if (object > (ObjType)container[mid])
            first = mid + 1;
        else
            last = mid;
    }
    if (object <= (ObjType)container[first])
        return first;
    return last;
}

// DONE: Error al poner size_t (resuelto)
// Motivo: El i está disminuyendo y es size_t, daba un underflow
template <typename Container, typename ObjType>
void insert_at(Container &container, ObjType object, size_t pos)
{
    // DONE: #5 replace int, long by types such as size_t
    size_t size = container.size();
    assert(pos <= size);
    size_t i = size - 2; // Penultimo
    while (i >= pos)
    {
        container[i + 1] = container[i];
        if (i == 0)
            break; // Prevents size_t underflow
        i--;
    }

    container[pos] = object;
}

template <typename Container>
void remove(Container &container, size_t pos)
{
    size_t size = container.size();
    for (auto i = pos + 1; i < size; i++)
        container[i - 1] = container[i];
}

template <typename value_type, typename LinkedValueType>
struct tagObjectInfo
{
    value_type key;
    LinkedValueType ObjID;
    size_t UseCounter;
    tagObjectInfo(const value_type &_key, LinkedValueType _ObjID)
        : key(_key), ObjID(_ObjID), UseCounter(0) {}
    tagObjectInfo(const tagObjectInfo &objInfo)
        : key(objInfo.key), ObjID(objInfo.ObjID), UseCounter(0) {}
    tagObjectInfo() {}
    operator value_type() { return key; }
    size_t GetUseCounter() { return UseCounter; }
};

template <typename Trait>
class CBTreePage //: public SimpleIndex <value_type>
// this is the in-memory version of the CBTreePage
{
    friend class BTree<Trait>;
    friend class btree_forward_iterator<CBTreePage<Trait>>;
    friend class btree_backward_iterator<CBTreePage<Trait>>;
    friend class btree_print_iterator<CBTreePage<Trait>>;
    template <typename T>
    friend void Print(NodeInfo<typename CBTreePage<T>::Node>& info, void* pExtra);
    typedef typename Trait::value_type value_type;
    typedef typename Trait::LinkedValueType LinkedValueType;
    typedef CBTreePage<Trait> BTPage; // useful shorthand

    typedef tagObjectInfo<value_type, LinkedValueType> ObjectInfo;

    // DONE replace by general foreach (foreach.h)
    //        typedef void (*lpfnForEach2)(ObjectInfo &info, size_t level, void *pExtra1);
    //        typedef void (*lpfnForEach3)(ObjectInfo &info, size_t level, void *pExtra1, void *pExtra2);

    // DONE create firstthat (foreach.h)
    // typedef ObjectInfo *(*lpfnFirstThat2)(ObjectInfo &info, size_t level, void *pExtra1);
    // typedef ObjectInfo *(*lpfnFirstThat3)(ObjectInfo &info, size_t level, void *pExtra1, void *pExtra2);

public:
    typedef CBTreePage<Trait> Node; // Used in the iterator
    typedef value_type Type;        // Used in the iterator

    value_type &getDataRef()
    {
        return m_Keys[0].key;
    }

    btree_forward_iterator<CBTreePage<Trait>> begin()
    {
        return btree_forward_iterator<CBTreePage<Trait>>(this, this);
    }

    btree_forward_iterator<CBTreePage<Trait>> end()
    {
        return btree_forward_iterator<CBTreePage<Trait>>(this, nullptr);
    }

    btree_backward_iterator<CBTreePage<Trait>> rbegin()
    {
        return btree_backward_iterator<CBTreePage<Trait>>(this, this);
    }

    btree_backward_iterator<CBTreePage<Trait>> rend()
    {
        return btree_backward_iterator<CBTreePage<Trait>>(this, nullptr);
    }

    btree_print_iterator<CBTreePage<Trait>> printbegin()
    {
        return btree_print_iterator<CBTreePage<Trait>>(this, this);
    }

    btree_print_iterator<CBTreePage<Trait>> printend()
    {
        return btree_print_iterator<CBTreePage<Trait>>(this, nullptr);
    }

    CBTreePage(size_t maxKeys, bool unique = true);
    virtual ~CBTreePage();
    // DONE replace 2nd param by LinkedValueType value
    bt_ErrorCode Insert(const value_type &key, const LinkedValueType ObjID);
    bt_ErrorCode Remove(const value_type &key, const LinkedValueType ObjID);
    bool Search(const value_type &key, LinkedValueType &ObjID);
    void Print(ostream &os);
    void Print(NodeInfo<Node> &nodeInfo, ostream &os);
    void Read(istream &is);

    // DONE: #7 ForEach must be a template inside this template
    template <typename Callable, typename... Args>
    void ForEach(Callable f, Args &&...args);

    template <typename Callable, typename... Args>
    void BackwardsForEach(Callable f, Args &&...args);

    template <typename Callable, typename... Args>
    void PrintForEach(Callable f, Args &&...args);

    // DONE: Replace by general foreach
    //        void            ForEach(lpfnForEach2 lpfn, size_t level, void *pExtra1);
    //        void            ForEach(lpfnForEach3 lpfn, size_t level, void *pExtra1, void *pExtra2);

    // DONE: #8 You may reduce these two function by using variadic templates
    template <typename Callable, typename... Args>
    ObjectInfo *FirstThat(Callable f, Args &&...args);
    //        ObjectInfo*     FirstThat(lpfnFirstThat2 lpfn, size_t level, void *pExtra1);
    //        ObjectInfo*     FirstThat(lpfnFirstThat3 lpfn, size_t level, void *pExtra1, void *pExtra2);

protected:
    size_t m_MinKeys; // minimum number of keys in a node
    size_t m_MaxKeys, // maximum number of keys in a node

        m_MaxKeysForChilds; // just to distinguish the root
    bool m_Unique;
    bool m_isRoot;
    // size_t           NextNode; // address of next node at same level
    // size_t RecAddr; // address of this node in the BTree file
    vector<ObjectInfo> m_Keys;
    vector<BTPage *> m_SubPages;

    size_t m_KeyCount;
    void Create();
    void Reset();
    void Destroy()
    {
        Reset();
        delete this;
    }
    void clear();

    bool Redistribute1(size_t &pos);
    bool Redistribute2(size_t pos);
    void RedistributeR2L(size_t pos);
    void RedistributeL2R(size_t pos);

    bool TreatUnderflow(size_t &pos)
    {
        return Redistribute1(pos) || Redistribute2(pos);
    }

    bt_ErrorCode Merge(size_t pos);
    bt_ErrorCode MergeRoot();
    void SplitChild(size_t pos);

    ObjectInfo &GetFirstObjectInfo();

    bool Overflow() { return m_KeyCount > m_MaxKeys; }
    bool Underflow() { return m_KeyCount < MinNumberOfKeys(); }
    bool IsFull() { return m_KeyCount >= m_MaxKeys; }

    size_t MinNumberOfKeys() { return 2 * m_MaxKeys / 3.0; }
    size_t GetFreeCells() { return m_MaxKeys - m_KeyCount; }
    size_t &NumberOfKeys() { return m_KeyCount; }
    size_t GetNumberOfKeys() { return m_KeyCount; }
    bool IsRoot() { return m_MaxKeysForChilds != m_MaxKeys; }
    void SetMaxKeysForChilds(size_t orderforchilds)
    {
        m_MaxKeysForChilds = orderforchilds;
    }
    size_t GetFreeCellsOnLeft(size_t pos)
    {
        if (pos > 0) // there is some page on left ?
            return m_SubPages[pos - 1]->GetFreeCells();
        return 0;
    }
    size_t GetFreeCellsOnRight(size_t pos)
    {
        if (pos < GetNumberOfKeys()) // there is some page on right ?
            return m_SubPages[pos + 1]->GetFreeCells();
        return 0;
    }

    template <typename T>
    friend ostream &operator<<(ostream &os, const CBTreePage<T> &page);

    template <typename T>
    friend istream &operator>>(istream &is, CBTreePage<T> &page);

private:
    bool SplitRoot();
    void SplitPageInto3(vector<ObjectInfo> &tmpKeys,
                        vector<BTPage *> &SubPages,
                        BTPage *&pChild1,
                        BTPage *&pChild2,
                        BTPage *&pChild3,
                        ObjectInfo &oi1,
                        ObjectInfo &oi2);
    void MovePage(BTPage *pChildPage, vector<ObjectInfo> &tmpKeys, vector<BTPage *> &tmpSubPages);
};

template <typename Trait>
CBTreePage<Trait>::CBTreePage(size_t maxKeys, bool unique)
    : m_MaxKeys(maxKeys), m_Unique(unique), m_KeyCount(0)
{
    Create();
    SetMaxKeysForChilds(m_MaxKeys);
}

template <typename Trait>
CBTreePage<Trait>::~CBTreePage()
{
    Reset();
}

template <typename Trait>
bt_ErrorCode CBTreePage<Trait>::Insert(const value_type &key, const LinkedValueType ObjID)
{
    size_t pos = binary_search(m_Keys, 0, m_KeyCount, key);
    bt_ErrorCode error = bt_ok;

    if (pos < m_KeyCount && (value_type)m_Keys[pos] == key && m_Unique)
        return bt_duplicate; // this key is duplicate

    if (!m_SubPages[pos]) // this is a leave
    {
        ::insert_at(m_Keys, ObjectInfo(key, ObjID), pos);
        m_KeyCount++;
        if (Overflow())
            return bt_overflow;
        return bt_ok;
    }
    else
    {
        // recursive insertion
        error = m_SubPages[pos]->Insert(key, ObjID);
        if (error == bt_overflow)
        {
            if (!Redistribute1(pos))
                SplitChild(pos);
            if (Overflow()) // Propagate overflow
                return bt_overflow;
            return bt_ok;
        }
    }
    if (Overflow()) // node overflow
        return bt_overflow;
    return bt_ok;
}

template <typename Trait>
bool CBTreePage<Trait>::Redistribute1(size_t &pos)
{
    if (m_SubPages[pos]->Underflow())
    {
        size_t NumberOfKeyOnLeft = 0,
               NumberOfKeyOnRight = 0;
        // is this the first element or there are more elements on right brother
        if (pos > 0)
            NumberOfKeyOnLeft = m_SubPages[pos - 1]->NumberOfKeys();
        if (pos < NumberOfKeys())
            NumberOfKeyOnRight = m_SubPages[pos + 1]->NumberOfKeys();

        if (NumberOfKeyOnLeft > NumberOfKeyOnRight)
            if (m_SubPages[pos - 1]->NumberOfKeys() > m_SubPages[pos - 1]->MinNumberOfKeys())
                RedistributeL2R(pos - 1); // bring elements from left brother
            else if (pos == NumberOfKeys())
                return (--pos, false);
            else
                return false;
        else // NumberOfKeyOnLeft < NumberOfKeyOnRight )
            if (m_SubPages[pos + 1]->NumberOfKeys() > m_SubPages[pos + 1]->MinNumberOfKeys())
                RedistributeR2L(pos + 1); // bring elements from right brother
            else if (pos == 0)
                return (++pos, false);
            else
                return false;
    }
    else // it is due to overflow
    {
        size_t FreeCellsOnLeft = GetFreeCellsOnLeft(pos), // Free Cells On Left
            fcor = GetFreeCellsOnRight(pos);              // Free Cells On Right

        if (!FreeCellsOnLeft && !fcor && m_SubPages[pos]->IsFull())
            return false;
        if (FreeCellsOnLeft > fcor) // There is more space on left
            RedistributeR2L(pos);
        else
            RedistributeL2R(pos);
    }
    return true;
}

// Redistribute2 function
// it considers two brothers m_SubPages[pos-1] && m_SubPages[pos+1]
// if it fails the only way is merge !
template <typename Trait>
bool CBTreePage<Trait>::Redistribute2(size_t pos)
{
    assert(pos > 0 && pos < NumberOfKeys());
    assert(m_SubPages[pos - 1] != 0 && m_SubPages[pos] != 0 && m_SubPages[pos + 1] != 0);
    assert(m_SubPages[pos - 1]->Underflow() ||
           m_SubPages[pos]->Underflow() ||
           m_SubPages[pos + 1]->Underflow());

    if (m_SubPages[pos - 1]->Underflow())
    { // Rotate R2L
        RedistributeR2L(pos + 1);
        RedistributeR2L(pos);
        if (m_SubPages[pos - 1]->Underflow())
            return false;
    }
    else if (m_SubPages[pos + 1]->Underflow())
    { // Rotate L2R
        RedistributeL2R(pos - 1);
        RedistributeL2R(pos);
        if (m_SubPages[pos + 1]->Underflow())
            return false;
    }
    else // The problem is exactly at pos !
    {
        // Rotate L2R
        RedistributeL2R(pos - 1);
        RedistributeR2L(pos + 1);
        if (m_SubPages[pos]->Underflow())
            return false;
    }
    return true;
}

template <typename Trait>
void CBTreePage<Trait>::RedistributeR2L(size_t pos)
{
    BTPage *pSource = m_SubPages[pos],
           *pTarget = m_SubPages[pos - 1];

    while (pSource->GetNumberOfKeys() > pSource->MinNumberOfKeys() &&
           pTarget->GetNumberOfKeys() < pSource->GetNumberOfKeys())
    {
        // Move from this page to the down-left page \/
        ::insert_at(pTarget->m_Keys, m_Keys[pos - 1], pTarget->NumberOfKeys()++);
        // Move the pointer leftest pointer to the rightest position
        ::insert_at(pTarget->m_SubPages, pSource->m_SubPages[0], pTarget->NumberOfKeys());

        // Move the leftest element to the root
        m_Keys[pos - 1] = pSource->m_Keys[0];

        // Remove the leftest element from rigth page
        ::remove(pSource->m_Keys, 0);
        ::remove(pSource->m_SubPages, 0);
        pSource->NumberOfKeys()--;
    }
}

template <typename Trait>
void CBTreePage<Trait>::RedistributeL2R(size_t pos)
{
    BTPage *pSource = m_SubPages[pos],
           *pTarget = m_SubPages[pos + 1];
    while (pSource->GetNumberOfKeys() > pSource->MinNumberOfKeys() &&
           pTarget->GetNumberOfKeys() < pSource->GetNumberOfKeys())
    {
        // Move from this page to the down-RIGHT page \/
        ::insert_at(pTarget->m_Keys, m_Keys[pos], 0);
        // Move the pointer rightest pointer to the leftest position
        ::insert_at(pTarget->m_SubPages, pSource->m_SubPages[pSource->NumberOfKeys()], 0);
        pTarget->NumberOfKeys()++;

        // Move the rightest element to the root
        m_Keys[pos] = pSource->m_Keys[pSource->NumberOfKeys() - 1];

        // Remove the leftest element from rigth page
        // it is not necessary erase because m_KeyCount controls
        pSource->NumberOfKeys()--;
    }
}

template <typename Trait>
void CBTreePage<Trait>::SplitChild(size_t pos)
{
    // FIRST: deciding the second page to split
    BTPage *pChild1 = 0, *pChild2 = 0;
    if (pos > 0) // is left page full ?
        if (m_SubPages[pos - 1]->IsFull())
        {
            pChild1 = m_SubPages[pos - 1];
            pChild2 = m_SubPages[pos--];
        }
    if (pos < GetNumberOfKeys()) // is right page full ?
        if (m_SubPages[pos + 1]->IsFull())
        {
            pChild1 = m_SubPages[pos];
            pChild2 = m_SubPages[pos + 1];
        }
    size_t nKeys = pChild1->GetNumberOfKeys() + pChild2->GetNumberOfKeys() + 1;

    // SECOND: copy both pages to a temporal one
    // Create two tmp vector
    vector<ObjectInfo> tmpKeys;
    // tmpKeys.resize(nKeys);
    vector<BTPage *> tmpSubPages;
    // tmpKeys.resize(nKeys+1);

    // copy from left child
    MovePage(pChild1, tmpKeys, tmpSubPages);
    // copy a key from parent
    tmpKeys.push_back(m_Keys[pos]);

    // copy from right child
    MovePage(pChild2, tmpKeys, tmpSubPages);

    BTPage *pChild3 = 0;
    ObjectInfo oi1, oi2;
    SplitPageInto3(tmpKeys, tmpSubPages, pChild1, pChild2, pChild3, oi1, oi2);

    // copy the first element to the root
    m_Keys[pos] = oi1;
    m_SubPages[pos] = pChild1;

    // copy the second element to the root
    ::insert_at(m_Keys, oi2, pos + 1);
    ::insert_at(m_SubPages, pChild2, pos + 1);
    NumberOfKeys()++;

    m_SubPages[pos + 2] = pChild3;
}

template <typename Trait>
void CBTreePage<Trait>::SplitPageInto3(vector<ObjectInfo> &tmpKeys,
                                       vector<BTPage *> &tmpSubPages,
                                       BTPage *&pChild1,
                                       BTPage *&pChild2,
                                       BTPage *&pChild3,
                                       ObjectInfo &oi1,
                                       ObjectInfo &oi2)
{
    assert(tmpKeys.size() >= 8);
    assert(tmpSubPages.size() >= 9);
    if (!pChild1)
        pChild1 = new BTPage(m_MaxKeysForChilds, m_Unique);

    // Split tmpKeys page into 3 pages
    // copy 1/3 elements to the first child
    pChild1->clear();
    size_t nKeys = (tmpKeys.size() - 2) / 3;
    size_t i = 0;
    for (; i < nKeys; i++)
    {
        pChild1->m_Keys[i] = tmpKeys[i];
        pChild1->m_SubPages[i] = tmpSubPages[i];
        pChild1->NumberOfKeys()++;
    }
    pChild1->m_SubPages[i] = tmpSubPages[i];

    // first element to go up !
    oi1 = tmpKeys[i++];

    if (!pChild2)
        pChild2 = new BTPage(m_MaxKeysForChilds, m_Unique);
    pChild2->clear();
    // copy 1/3 to the second child
    nKeys += (tmpKeys.size() - 2) / 3 + 1;
    size_t j = 0;
    for (; i < nKeys; i++, j++)
    {
        pChild2->m_Keys[j] = tmpKeys[i];
        pChild2->m_SubPages[j] = tmpSubPages[i];
        pChild2->NumberOfKeys()++;
    }
    pChild2->m_SubPages[j] = tmpSubPages[i];

    // copy the second element to the root
    oi2 = tmpKeys[i++];

    // copy 1/3 to the third child
    if (!pChild3)
        pChild3 = new BTPage(m_MaxKeysForChilds, m_Unique);
    pChild3->clear();
    nKeys = tmpKeys.size();
    for (j = 0; i < nKeys; i++, j++)
    {
        pChild3->m_Keys[j] = tmpKeys[i];
        pChild3->m_SubPages[j] = tmpSubPages[i];
        pChild3->NumberOfKeys()++;
    }
    pChild3->m_SubPages[j] = tmpSubPages[i];
}

template <typename Trait>
bool CBTreePage<Trait>::SplitRoot()
{
    BTPage *pChild1 = 0, *pChild2 = 0, *pChild3 = 0;
    ObjectInfo oi1, oi2;
    SplitPageInto3(m_Keys, m_SubPages, pChild1, pChild2, pChild3, oi1, oi2);
    clear();

    // copy the first element to the root
    m_Keys[0] = oi1;
    m_SubPages[0] = pChild1;
    NumberOfKeys()++;

    // copy the second element to the root
    m_Keys[1] = oi2;
    m_SubPages[1] = pChild2;
    NumberOfKeys()++;

    m_SubPages[2] = pChild3;
    return true;
}

template <typename Trait>
bool CBTreePage<Trait>::Search(const value_type &key, LinkedValueType &ObjID)
{
    size_t pos = binary_search(m_Keys, 0, m_KeyCount, key);
    if (pos >= m_KeyCount)
    {
        if (m_SubPages[pos])
            return m_SubPages[pos]->Search(key, ObjID);
        else
            return false;
    }
    if (key == m_Keys[pos].key)
    {
        ObjID = m_Keys[pos].ObjID;
        m_Keys[pos].UseCounter++;
        return true;
    }
    if (key < m_Keys[pos].key)
        if (m_SubPages[pos])
            return m_SubPages[pos]->Search(key, ObjID);
    return false;
}

/*template <typename value_type, typename LinkedValueType>
void CBTreePage<value_type, LinkedValueType>::ForEachReverse(lpfnForEach2 lpfn, size_t level, void *pExtra1)
{
       if( m_SubPages[m_KeyCount] )
               m_SubPages[m_KeyCount]->ForEach(lpfn, level+1, pExtra1);
       for(size_t i = m_KeyCount-1 ; i >= 0  ; i--)
       {
               lpfn(m_Keys[i], level, pExtra1);
               if( m_SubPages[i] )
                       m_SubPages[i]->ForEach(lpfn, level+1, pExtra1);
       }
}*/

// DONE replace by generic foreach
// template <typename Trait>
// void CBTreePage<Trait>::ForEach(lpfnForEach2 lpfn, size_t level, void *pExtra1)
// {
//        for(size_t i = 0 ; i < m_KeyCount ; i++)
//        {
//                if( m_SubPages[i] )
//                        m_SubPages[i]->ForEach(lpfn, level+1, pExtra1);
//                lpfn(m_Keys[i], level, pExtra1);
//        }
//        if( m_SubPages[m_KeyCount] )
//                m_SubPages[m_KeyCount]->ForEach(lpfn, level+1, pExtra1);
// }

/*template <typename value_type, typename LinkedValueType>
void CBTreePage<value_type, LinkedValueType>::ForEachReverse(lpfnForEach3 lpfn,
                                                                                                       size_t level, void *pExtra1, void *pExtra2)
{
       if( m_SubPages[m_KeyCount] )
               m_SubPages[m_KeyCount]->ForEach(lpfn, level+1, pExtra1, pExtra2);
       for(size_t i = m_KeyCount-1 ; i >= 0  ; i--)
       {
               lpfn(m_Keys[i], level, pExtra1, pExtra2);
               if( m_SubPages[i] )
                       m_SubPages[i]->ForEach(lpfn, level+1, pExtra1, pExtra2);
       }
}*/

// DONE replace by generic foreach
// template <typename Trait>
// void CBTreePage<Trait>::ForEach(lpfnForEach3 lpfn, size_t level, void *pExtra1, void *pExtra2)
// {
//        for(size_t i = 0 ; i < m_KeyCount ; i++)
//        {
//                if( m_SubPages[i] )
//                        m_SubPages[i]->ForEach(lpfn, level+1, pExtra1, pExtra2);
//                lpfn(m_Keys[i], level, pExtra1, pExtra2);
//        }
//        if( m_SubPages[m_KeyCount] )
//                m_SubPages[m_KeyCount]->ForEach(lpfn, level+1, pExtra1, pExtra2);
// }

template <typename Trait>
template <typename Callable, typename... Args>
void CBTreePage<Trait>::ForEach(Callable c, Args &&...args)
{
    foreach (this->begin(), this->end(), c, args...)
        ;
}

template <typename Trait>
template <typename Callable, typename... Args>
void CBTreePage<Trait>::BackwardsForEach(Callable c, Args &&...args)
{
    foreach (this->rbegin(), this->rend(), c, args...)
        ;
}

template <typename Trait>
template <typename Callable, typename... Args>
void CBTreePage<Trait>::PrintForEach(Callable c, Args &&...args)
{
    foreach (this->printbegin(), this->printend(), c, args...)
        ;
}

template <typename Trait>
template <typename Callable, typename... Args>
typename CBTreePage<Trait>::ObjectInfo *
CBTreePage<Trait>::FirstThat(Callable ope, Args &&...args)
{
    return firstThat(this->begin(), this->end(), ope, args...);
}

// DONE: replace by generic firstthat
// template <typename Trait>
// typename CBTreePage<Trait>::ObjectInfo *
// CBTreePage<Trait>::FirstThat(lpfnFirstThat2 lpfn, size_t level, void *pExtra1)
// {
//         // return firstThat(this->begin(), this->end(), ope, ...args);
//        ObjectInfo *pTmp;
//        for(size_t i = 0 ; i < m_KeyCount ; i++)
//        {
//                if( m_SubPages[i] )
//                        if( (pTmp = m_SubPages[i]->FirstThat(lpfn, level+1, pExtra1)) )
//                                return pTmp;
//                if( lpfn(m_Keys[i], level, pExtra1) )
//                        return &m_Keys[i];
//        }
//        if( m_SubPages[m_KeyCount] )
//                if( (pTmp = m_SubPages[m_KeyCount]->FirstThat(lpfn, level+1, pExtra1)) )
//                        return pTmp;
//        return 0;
// }

// // DONE: replace by generic firstthat
// template <typename Trait>
// typename CBTreePage<Trait>::ObjectInfo *
// CBTreePage<Trait>::FirstThat(lpfnFirstThat3 lpfn,size_t level, void *pExtra1, void *pExtra2)
// {
//        ObjectInfo *pTmp;
//        for(size_t i = 0 ; i < m_KeyCount ; i++)
//        {
//                if( m_SubPages[i] )
//                        if( (pTmp = m_SubPages[i]->FirstThat(lpfn, level+1, pExtra1, pExtra2) ) )
//                                return pTmp;
//                if( lpfn(m_Keys[i], level, pExtra1, pExtra2) )
//                        return &m_Keys[i];
//        }
//        if( m_SubPages[m_KeyCount] )
//                if( (pTmp = m_SubPages[m_KeyCount]->FirstThat(lpfn, level+1, pExtra1, pExtra2) ) )
//                        return pTmp;
//        return 0;
// }

template <typename Trait>
bt_ErrorCode CBTreePage<Trait>::Remove(const value_type &key, const LinkedValueType ObjID)
{
    bt_ErrorCode error = bt_ok;
    size_t pos = binary_search(m_Keys, 0, m_KeyCount, key);
    if (pos < NumberOfKeys() && key == m_Keys[pos].key /*&& m_Keys[pos].m_ObjID == ObjID*/) // We found it !
    {
        // This is a leave: First
        if (!m_SubPages[pos + 1]) // This is a leave ? FIRST CASE !
        {
            ::remove(m_Keys, pos);
            NumberOfKeys()--;
            if (Underflow())
                return bt_underflow;
            return bt_ok;
        }

        // We FOUND IT BUT it is NOT a leave ? SECOND CASE !
        {
            // Get the first element from right branch
            ObjectInfo &rFirstFromRight = m_SubPages[pos + 1]->GetFirstObjectInfo();
            // change with a leave
            swap(m_Keys[pos], rFirstFromRight);
            // Remove it from this leave

            // Print(cout);
            error = m_SubPages[++pos]->Remove(key, ObjID);
        }
    }
    else if (pos == NumberOfKeys()) // it is not here, go by the last branch
        error = m_SubPages[pos]->Remove(key, ObjID);
    else if (key <= m_Keys[pos].key) // = is because identical keys are inserted on left (see Insert)
    {
        if (m_SubPages[pos])
            error = m_SubPages[pos]->Remove(key, ObjID);
        else
            return bt_nofound;
    }
    if (error == bt_underflow)
    {
        // THIRD CASE: After removing the element we have an underflow
        // Print(cout);
        if (TreatUnderflow(pos))
            return bt_ok;
        // FOURTH CASE: it was not possible to redistribute -> Merge
        if (IsRoot() && NumberOfKeys() == 2)
            return MergeRoot();
        return Merge(pos);
    }
    if (error == bt_nofound)
        return bt_nofound;
    return bt_ok;
}

template <typename Trait>
bt_ErrorCode CBTreePage<Trait>::Merge(size_t pos)
{
    assert(m_SubPages[pos - 1]->NumberOfKeys() +
               m_SubPages[pos]->NumberOfKeys() +
               m_SubPages[pos + 1]->NumberOfKeys() ==
           3 * m_SubPages[pos]->MinNumberOfKeys() - 1);

    // FIRST: Put all the elements into a vector
    vector<ObjectInfo> tmpKeys;
    // tmpKeys.resize(nKeys);
    vector<BTPage *> tmpSubPages;

    BTPage *pChild1 = m_SubPages[pos - 1],
           *pChild2 = m_SubPages[pos],
           *pChild3 = m_SubPages[pos + 1];
    MovePage(pChild1, tmpKeys, tmpSubPages);
    tmpKeys.push_back(m_Keys[pos - 1]);
    MovePage(pChild2, tmpKeys, tmpSubPages);
    tmpKeys.push_back(m_Keys[pos]);
    MovePage(pChild3, tmpKeys, tmpSubPages);
    pChild3->Destroy();
    ;

    // Move 1/2 elements to pChild1
    size_t nKeys = pChild1->GetFreeCells();
    size_t i = 0;
    for (; i < nKeys; i++)
    {
        pChild1->m_Keys[i] = tmpKeys[i];
        pChild1->m_SubPages[i] = tmpSubPages[i];
        pChild1->NumberOfKeys()++;
    }
    pChild1->m_SubPages[i] = tmpSubPages[i];

    m_Keys[pos - 1] = tmpKeys[i];
    m_SubPages[pos - 1] = pChild1;

    ::remove(m_Keys, pos);
    ::remove(m_SubPages, pos);
    NumberOfKeys()--;

    nKeys = pChild2->GetFreeCells();
    size_t j = ++i;
    for (i = 0; i < nKeys; i++, j++)
    {
        pChild2->m_Keys[i] = tmpKeys[j];
        pChild2->m_SubPages[i] = tmpSubPages[j];
        pChild2->NumberOfKeys()++;
    }
    pChild2->m_SubPages[i] = tmpSubPages[j];
    m_SubPages[pos] = pChild2;

    if (Underflow())
        return bt_underflow;
    return bt_ok;
}

template <typename Trait>
bt_ErrorCode CBTreePage<Trait>::MergeRoot()
{
    size_t pos = 1;
    assert(m_SubPages[pos - 1]->NumberOfKeys() +
               m_SubPages[pos]->NumberOfKeys() +
               m_SubPages[pos + 1]->NumberOfKeys() ==
           3 * m_SubPages[pos]->MinNumberOfKeys() - 1);

    BTPage *pChild1 = m_SubPages[pos - 1], *pChild2 = m_SubPages[pos], *pChild3 = m_SubPages[pos + 1];
    size_t nKeys = pChild1->NumberOfKeys() + pChild2->NumberOfKeys() + pChild3->NumberOfKeys() + 2;

    // FIRST: Put all the elements into a vector
    vector<ObjectInfo> tmpKeys;
    // tmpKeys.resize(nKeys);
    vector<BTPage *> tmpSubPages;

    MovePage(pChild1, tmpKeys, tmpSubPages);
    tmpKeys.push_back(m_Keys[pos - 1]);
    MovePage(pChild2, tmpKeys, tmpSubPages);
    tmpKeys.push_back(m_Keys[pos]);
    MovePage(pChild3, tmpKeys, tmpSubPages);

    clear();
    size_t i = 0;
    for (; i < nKeys; i++)
    {
        m_Keys[i] = tmpKeys[i];
        m_SubPages[i] = tmpSubPages[i];
        NumberOfKeys()++;
    }
    m_SubPages[i] = tmpSubPages[i];

    // Print(cout);
    pChild1->Destroy();
    pChild2->Destroy();
    pChild3->Destroy();

    return bt_rootmerged;
}

template <typename Trait>
typename CBTreePage<Trait>::ObjectInfo &
CBTreePage<Trait>::GetFirstObjectInfo()
{
    if (m_SubPages[0])
        return m_SubPages[0]->GetFirstObjectInfo();
    return m_Keys[0];
}

template <typename Trait>
void Print(NodeInfo<typename CBTreePage<Trait>::Node>& info, void* pExtra)
{
    ostream &os = *(ostream *)pExtra;
    for (size_t i = 0; i < info.level; i++)
        os << "\t";
    // os<<info.level << endl;
    auto inf = info.pPage->m_Keys[info.pos];
    os << inf.key << "->" << inf.ObjID << "\n";
}

template <typename Trait>
void CBTreePage<Trait>::Print(ostream &os)
{
    PrintForEach(&::Print<Trait>, &os);
}

template <typename Trait>
void CBTreePage<Trait>::Read(istream &is)
{
    while (!is.eof())
    {
        using value_type = typename CBTreePage<Trait>::value_type;
        using LinkedValueType = typename CBTreePage<Trait>::LinkedValueType;
        value_type key;
        LinkedValueType val;
        is >> key >> val;
        Insert(key, val);
    }
}

template <typename Trait>
void CBTreePage<Trait>::Create()
{
    Reset();
    m_Keys.resize(m_MaxKeys + 1);
    m_SubPages.resize(m_MaxKeys + 2, NULL);
    m_KeyCount = 0;
    m_MinKeys = 2 * m_MaxKeys / 3;
}

template <typename Trait>
void CBTreePage<Trait>::Reset()
{
    for (size_t i = 0; i < m_KeyCount; i++)
        delete m_SubPages[i];
    clear();
}

template <typename Trait>
void CBTreePage<Trait>::clear()
{
    // m_Keys.clear();
    // m_SubPages.clear();
    m_KeyCount = 0;
}

template <typename Trait>
CBTreePage<Trait> *CreateBTreeNode(size_t maxKeys, bool unique)
{
    return new CBTreePage<Trait>(maxKeys, unique);
}

template <typename Trait>
void CBTreePage<Trait>::MovePage(BTPage *pChildPage, vector<ObjectInfo> &tmpKeys, vector<BTPage *> &tmpSubPages)
{
    // DONE: #37 change int by size_t
    size_t nKeys = pChildPage->GetNumberOfKeys();
    size_t i = 0;
    for (i = 0; i < nKeys; i++)
    {
        tmpKeys.push_back(pChildPage->m_Keys[i]);
        tmpSubPages.push_back(pChildPage->m_SubPages[i]);
    }
    tmpSubPages.push_back(pChildPage->m_SubPages[i]);
    pChildPage->clear();
}

// DONE Add opertor<<
template <typename Trait>
ostream &operator<<(ostream &os, CBTreePage<Trait> &page)
{
    page.Print(os);
    return os;
}

// DONE: Add opertor >>
template <typename Trait>
istream &operator>>(istream &is, CBTreePage<Trait> &page)
{
    page.Read(is);
    return is;
}

#endif
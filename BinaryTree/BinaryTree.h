#include <utility>
#include <algorithm>
#include <cassert>
#include <stack>
using namespace std;

template <typename KeyType, typename ValueType>
class NodeBinaryTree
{
private:
  using Node = NodeBinaryTree<KeyType, ValueType>;

  KeyNode<KeyType, ValueType> value;
  Node *parent_ptr = nullptr;
  Node *left_child_ptr = nullptr;
  Node *right_child_ptr = nullptr;
  size_t height;
  bool is_printed = false;

public:
  NodeBinaryTree(const KeyNode<KeyType, ValueType> &value,
                 Node *parent_ptr = nullptr,
                 Node *left_child_ptr = nullptr,
                 Node *right_child_ptr = nullptr)
      : value(value),
        parent_ptr(parent_ptr),
        left_child_ptr(left_child_ptr),
        right_child_ptr(right_child_ptr),
        height(parent_ptr ? parent_ptr->getHeight() + 1 : 0) {}

  int getHeight() const
  {
    return height;
  }

  void setLeftChild(Node *child_ptr)
  {
    left_child_ptr = child_ptr;
  }

  Node *getLeftChild() const
  {
    return left_child_ptr;
  }

  void setRightChild(Node *child_ptr)
  {
    right_child_ptr = child_ptr;
  }

  Node *getRightChild() const
  {
    return right_child_ptr;
  }

  Node *getParent() const
  {
    return parent_ptr;
  }

  bool getPrinted() const
  {
    return is_printed;
  }

  void setPrinted(bool printed)
  {
    is_printed = printed;
  }

  const KeyNode<KeyType, ValueType> &getValue() const
  {
    return value;
  }

  std::string toString(std::function<std::string(const KeyNode<KeyType, ValueType> &)> print_value) const
  {
    std::string str;
    str += "\n (height: " + std::to_string(height) + ") " + print_value(value) + ": ";
    if (left_child_ptr != nullptr || right_child_ptr != nullptr)
    {
      if (left_child_ptr != nullptr)
      {
        str += print_value(left_child_ptr->getValue()) + " ";
      }
      if (right_child_ptr != nullptr)
      {
        str += print_value(right_child_ptr->getValue()) + " ";
      }
    }

    if (left_child_ptr != nullptr)
    {
      str += left_child_ptr->toString(print_value);
    }
    else
    {
      str += " NULL";
    }
    if (right_child_ptr != nullptr)
    {
      str += right_child_ptr->toString(print_value);
    }
    else
    {
      str += " NULL";
    }
    return str;
  }
};

template <typename KeyType, typename ValueType>
struct LessBinaryTree
{
  using T = KeyType;
  using Node = LessBinaryTree<KeyType, ValueType>;
  using CompareFn = less<T>;
};

template <typename KeyType, typename ValueType>
struct GreaterBinaryTree
{
  using T = KeyType;
  using Node = GreaterBinaryTree<KeyType, ValueType>;
  using CompareFn = greater<T>;
};

template <typename KeyType, typename ValueType, typename CompareFn>
class BinaryTree
{
private:
  using Node = NodeBinaryTree<KeyType, ValueType>;

  Node *root_ptr = nullptr;

  void internal_insert(const KeyNode<KeyType, ValueType> &value, Node *const &node, Node *parent_ptr)
  {
    if (node == nullptr)
    {
      parent_ptr->setLeftChild(new Node(value, parent_ptr));
    }
    else if (CompareFn()(value.getKey(), node->getValue().getKey()))
    {
      internal_insert(value, node->getLeftChild(), node);
    }
    else
    {
      internal_insert(value, node->getRightChild(), node);
    }
  }

  void internal_printTree(Node *node, std::function<std::string(const KeyNode<KeyType, ValueType> &)> print_value, int height = 0)
  {
    if (node == nullptr)
    {
      return;
    }

    std::cout << "(height: " << height << ") " << node->toString(print_value) << std::endl;

    internal_printTree(node->getLeftChild(), print_value, height + 1);
    internal_printTree(node->getRightChild(), print_value, height + 1);
  }

  void resetPrintedFlags(NodeBinaryTree<KeyType, ValueType> *node)
  {
    if (node != nullptr)
    {
      node->setPrinted(false);                  // Reset the 'is_printed' flag for this node.
      resetPrintedFlags(node->getLeftChild());  // Recurse left.
      resetPrintedFlags(node->getRightChild()); // Recurse right.
    }
  }

public:
  BinaryTree(const KeyNode<KeyType, ValueType> &value)
  {
    root_ptr = new Node(value);
  }

  Node *getRoot() const
  {
    return root_ptr;
  }

  Node *leftMost()
  {
    assert(root_ptr != nullptr);
    Node *node_ptr = root_ptr;
    while (node_ptr != nullptr)
    {
      node_ptr = node_ptr->getLeftChild();
    }
    return node_ptr;
  }

  Node *rightMost()
  {
    assert(root_ptr != nullptr);
    Node *node_ptr = root_ptr;
    while (node_ptr != nullptr)
    {
      node_ptr = node_ptr->getRightChild();
    }
    return node_ptr;
  }

  void resetPrintedFlags()
  {
    resetPrintedFlags(root_ptr);
  }

  void insert(const KeyNode<KeyType, ValueType> &value)
  {
    internal_insert(value, root_ptr, nullptr);
  }

  // Print the tree using the toString method from NodeBinaryTree.
  void printTree(std::function<std::string(const KeyNode<KeyType, ValueType> &)> print_value)
  {
    internal_printTree(root_ptr, print_value);
  }

  class PreorderIterator
  {
  private:
    using Node = NodeBinaryTree<KeyType, ValueType>;

    Node *current;
    std::stack<Node *> nodeStack;

  public:
    PreorderIterator(Node *root) : current(root)
    {
      // Initialize the iterator by pushing nodes onto the stack
      if (current)
      {
        nodeStack.push(current);
      }
    }

    PreorderIterator &operator++()
    {
      if (nodeStack.empty())
      {
        current = nullptr;
        return *this;
      }

      current = nodeStack.top();
      nodeStack.pop();

      if (current->getRightChild())
      {
        nodeStack.push(current->getRightChild());
      }

      if (current->getLeftChild())
      {
        nodeStack.push(current->getLeftChild());
      }

      return *this;
    }

    bool operator!=(const PreorderIterator &other) const
    {
      return current != other.current;
    }

    const KeyNode<KeyType, ValueType> &operator*() const
    {
      return current->getValue();
    }
  };

  class PostorderIterator
  {
  private:
    // Define the Node type and stack for traversal
    using Node = NodeBinaryTree<KeyType, ValueType>;
    std::stack<Node *> stack;

    void pushLeftSubtree(Node *node)
    {
      while (node)
      {
        stack.push(node);
        node = node->getLeftChild();
      }
    }

  public:
    PostorderIterator(Node *root)
    {
      pushLeftSubtree(root);
    }

    PostorderIterator &operator++()
    {
      if (!stack.empty())
      {
        Node *current = stack.top();
        stack.pop();
        pushLeftSubtree(current->getRightChild());
        return *this;
      }

      return *this;
    }

    const KeyNode<KeyType, ValueType> &operator*() const
    {
      return stack.top()->getValue();
    }

    bool operator==(const PostorderIterator &other) const
    {
      return stack == other.stack;
    }

    bool operator!=(const PostorderIterator &other) const
    {
      return stack != other.stack;
    }
  };

  class InorderIterator
  {
  private:
    using Node = NodeBinaryTree<KeyType, ValueType>;
    std::stack<Node *> stack;

    void pushLeftSubtree(Node *node)
    {
      while (node)
      {
        stack.push(node);
        node = node->getLeftChild();
      }
    }

  public:
    InorderIterator(Node *root)
    {
      pushLeftSubtree(root);
    }

    InorderIterator &operator++()
    {
      if (!stack.empty())
      {
        Node *current = stack.top();
        stack.pop();
        pushLeftSubtree(current->getRightChild());
        return *this;
      }

      return *this;
    }

    const KeyNode<KeyType, ValueType> &operator*() const
    {
      return stack.top()->getValue();
    }

    bool operator==(const InorderIterator &other) const
    {
      return stack == other.stack;
    }

    bool operator!=(const InorderIterator &other) const
    {
      return stack != other.stack;
    }
  };

  PreorderIterator beginPreorder() const
  {
    return PreorderIterator(root_ptr);
  }

  InorderIterator beginInorder() const
  {
    return InorderIterator(root_ptr);
  }

  PostorderIterator beginPostorder() const
  {
    return PostorderIterator(root_ptr);
  }

  PreorderIterator endPreorder() const
  {
    return PreorderIterator(nullptr);
  }

  InorderIterator endInorder() const
  {
    return InorderIterator(nullptr);
  }

  PostorderIterator endPostorder() const
  {
    return PostorderIterator(nullptr);
  }
};

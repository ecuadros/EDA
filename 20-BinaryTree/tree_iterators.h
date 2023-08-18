#include <iostream>
#include <stack>

template <typename T>
struct Node {
    T data;
    Node<T>* left;
    Node<T>* right;

    Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
};

template <typename T>
class BinaryTree {
public:
    typedef Node<T> Node;

private:
    Node* root;

public:
    BinaryTree() : root(nullptr) {}

    void insert(const T& value) {
        insertNode(root, value);
    }

    void printInOrder() const {
        printInOrder(root);
        std::cout << std::endl;
    }

    void printPreOrder() const {
        printPreOrder(root);
        std::cout << std::endl;
    }

    void printPostOrder() const {
        printPostOrder(root);
        std::cout << std::endl;
    }

    class InOrderIterator {
    private:
        std::stack<Node*> stack;

    public:
        InOrderIterator(Node* root) {
            traverseLeftSubtree(root);
        }

        bool operator!=(const InOrderIterator& other) const {
            return !stack.empty() || !other.stack.empty();
        }

        T operator*() const {
            return stack.top()->data;
        }

        InOrderIterator& operator++() {
            Node* node = stack.top();
            stack.pop();
            traverseLeftSubtree(node->right);
            return *this;
        }

    private:
        void traverseLeftSubtree(Node* node) {
            while (node != nullptr) {
                stack.push(node);
                node = node->left;
            }
        }
    };

    class PreOrderIterator {
    private:
        std::stack<Node*> stack;

    public:
        PreOrderIterator(Node* root) {
            if (root != nullptr) {
                stack.push(root);
            }
        }

        bool operator!=(const PreOrderIterator& other) const {
            return !stack.empty() || !other.stack.empty();
        }

        T operator*() const {
            return stack.top()->data;
        }

        PreOrderIterator& operator++() {
            Node* node = stack.top();
            stack.pop();
            if (node->right != nullptr) {
                stack.push(node->right);
            }
            if (node->left != nullptr) {
                stack.push(node->left);
            }
            return *this;
        }
    };

    class PostOrderIterator {
    private:
        std::stack<Node*> stack;
        Node* lastVisited;

    public:
        PostOrderIterator(Node* root) : lastVisited(nullptr) {
            traverseLeftSubtree(root);
        }

        bool operator!=(const PostOrderIterator& other) const {
            return !stack.empty() || !other.stack.empty();
        }

        T operator*() const {
            return stack.top()->data;
        }

        PostOrderIterator& operator++() {
            Node* node = stack.top();
            if (node->right == nullptr || node->right == lastVisited) {
                stack.pop();
                lastVisited = node;
            } else {
                traverseLeftSubtree(node->right);
            }
            return *this;
        }

    private:
        void traverseLeftSubtree(Node* node) {
            while (node != nullptr) {
                stack.push(node);
                if (node->left != nullptr) {
                    node = node->left;
                } else {
                    node = node->right;
                }
            }
        }
    };

    void insertNode(Node*& node, const T& value) {
        if (node == nullptr) {
            node = new Node(value);
        } else if (value < node->data) {
            insertNode(node->left, value);
        } else if (value > node->data) {
            insertNode(node->right, value);
        }
    }

    void printInOrder(const Node* node) const {
        if (node != nullptr) {
            printInOrder(node->left);
            std::cout << node->data << " ";
            printInOrder(node->right);
        }
    }

    void printPreOrder(const Node* node) const {
        if (node != nullptr) {
            std::cout << node->data << " ";
            printPreOrder(node->left);
            printPreOrder(node->right);
        }
    }

    void printPostOrder(const Node* node) const {
        if (node != nullptr) {
            printPostOrder(node->left);
            printPostOrder(node->right);
            std::cout << node->data << " ";
        }
    }
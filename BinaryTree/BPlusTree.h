#include <iostream>
#include <vector>

template <typename KeyType, typename ValueType>
struct BPlusTreeNode {
    KeyType key;
    ValueType value;
    BPlusTreeNode* next;  
    std::vector<BPlusTreeNode*> children;  // Pointers to child nodes (for internal nodes)

    BPlusTreeNode(KeyType k, ValueType v) : key(k), value(v), next(nullptr) {}
};

template <typename KeyType, typename ValueType>
class BPlusTree {
private:
    BPlusTreeNode<KeyType, ValueType>* root;

public:
    BPlusTree() : root(nullptr) {}

    void insert(KeyType key, ValueType value) {
        if (!root) {
            root = new BPlusTreeNode<KeyType, ValueType>(key, value);
        } else {
            insertNode(root, key, value);
        }
    }

    void print() {
        printTree(root, 0);
    }

private:
    void insertNode(BPlusTreeNode<KeyType, ValueType>* node, KeyType key, ValueType value) {
        if (!node->children.empty()) {
            // Internal node
            int i = 0;
            while (i < node->children.size() && key >= node->children[i]->key) {
                i++;
            }
            insertNode(node->children[i], key, value);
        } else {
            // Leaf node
            if (node->key <= key) {
                BPlusTreeNode<KeyType, ValueType>* newNode = new BPlusTreeNode<KeyType, ValueType>(key, value);
                newNode->next = node->next;
                node->next = newNode;
            } else {
                BPlusTreeNode<KeyType, ValueType>* newNode = new BPlusTreeNode<KeyType, ValueType>(node->key, node->value);
                newNode->next = node->next;
                node->next = newNode;
                node->key = key;
                node->value = value;
            }
        }
    }

    void printTree(BPlusTreeNode<KeyType, ValueType>* node, int depth) {
        if (node) {
            for (int i = 0; i < depth; i++) {
                std::cout << "  "; // Indent based on depth
            }
            std::cout << "(" << node->key << ", " << node->value << ")" << std::endl;

            for (const auto& child : node->children) {
                printTree(child, depth + 1);
            }
        }
    }
};
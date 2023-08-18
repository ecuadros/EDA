#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__

#include <iostream>
#include <functional>

template <typename T>
class AVLNode
{
public:
    T data;
    AVLNode<T> *left;
    AVLNode<T> *right;
    int height;

    AVLNode(T value)
    {
        data = value;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

template <typename T, typename Compare = std::less<T>>
class AVLTree
{
private:
    AVLNode<T> *root;

    int getHeight(AVLNode<T> *node)
    {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    int getBalanceFactor(AVLNode<T> *node)
    {
        if (node == nullptr)
            return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    AVLNode<T> *rotateLeft(AVLNode<T> *node)
    {
        AVLNode<T> *newRoot = node->right;
        AVLNode<T> *subtree = newRoot->left;

        newRoot->left = node;
        node->right = subtree;

        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
        newRoot->height = std::max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

        return newRoot;
    }

    AVLNode<T> *rotateRight(AVLNode<T> *node)
    {
        AVLNode<T> *newRoot = node->left;
        AVLNode<T> *subtree = newRoot->right;

        newRoot->right = node;
        node->left = subtree;

        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
        newRoot->height = std::max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

        return newRoot;
    }

    AVLNode<T> *insertNode(AVLNode<T> *node, T value)
    {
        if (node == nullptr)
            return new AVLNode<T>(value);

        Compare compare;
        if (compare(value, node->data))
            node->left = insertNode(node->left, value);
        else
            node->right = insertNode(node->right, value);

        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;

        int balanceFactor = getBalanceFactor(node);

        if (balanceFactor > 1)
        {
            if (compare(value, node->left->data))
                return rotateRight(node->left);
            else
            {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }
        else if (balanceFactor < -1)
        {
            if (compare(value, node->right->data))
            {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            else
                return rotateLeft(node->right);
        }

        return node;
    }

    void inorderTraversal(AVLNode<T> *node)
    {
        if (node != nullptr)
        {
            inorderTraversal(node->left);
            std::cout << node->data << " ";
            inorderTraversal(node->right);
        }
    }

public:
    AVLTree()
    {
        root = nullptr;
    }

    void insert(T value)
    {
        root = insertNode(root, value);
    }

    void printInorder()
    {
        inorderTraversal(root);
        std::cout << std::endl;
    }
};

#endif
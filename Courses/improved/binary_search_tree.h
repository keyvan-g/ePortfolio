//============================================================================
// Name        : binary_search_tree.h
// Author      : Keyvan Gharehtapeh
// Version     : 1.0
// Description : Binary Search Tree implementation of IDataStructure
//============================================================================

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "data_structure_interface.h"
#include "course.h"
#include <functional>

// Internal structure for tree node
struct Node {
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() : left(nullptr), right(nullptr) {}

    // initialize with a course
    Node(const Course& aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
};

// Comparison operators for Node
bool operator<(Node *node, const Course &b);
bool operator>(Node *node, const Course &b);
bool operator<(Node *node, const std::string& courseId);
bool operator>(Node *node, const std::string& courseId);
bool operator==(Node *node, const std::string& courseId);

/**
 * Binary Search Tree implementation of the IDataStructure interface
 */
class BinarySearchTree : public IDataStructure {
private:
    Node *root;
    size_t nodeCount;

    // Helper methods
    void addNode(Node *node, const Course &course);
    void inOrder(Node *node, std::function<void(const Course&)> callback);
    void postOrder(Node *node);
    void preOrder(Node *node);
    Node *removeNode(Node *node, const std::string& courseId);
    void deleteNode(Node *node);
    size_t countNodes(Node *node) const;

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();

    // IDataStructure interface implementation
    void Insert(const Course& course) override;
    void Remove(const std::string& courseId) override;
    Course Search(const std::string& courseId) override;
    void PrintAll() override;
    bool IsEmpty() const override;
    size_t Size() const override;
    void Clear() override;
    void ForEach(std::function<void(const Course&)> callback) override;

    // Additional BST-specific methods
    void InOrder();
    void PostOrder();
    void PreOrder();
    void InOrder(std::function<void(const Course&)> callback);
};

#endif // BINARY_SEARCH_TREE_H
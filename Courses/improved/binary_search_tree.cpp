//============================================================================
// Name        : binary_search_tree.cpp
// Author      : Keyvan Gharehtapeh
// Version     : 1.0
// Description : Binary Search Tree implementation
//============================================================================

#include "binary_search_tree.h"
#include <iostream>

// Node comparison operators
bool operator<(Node *node, const Course &b) { 
    return node->course < b; 
}

bool operator>(Node *node, const Course &b) { 
    return node->course > b; 
}

bool operator<(Node *node, const std::string& courseId) {
    return node->course.courseId.compare(courseId) < 0;
}

bool operator>(Node *node, const std::string& courseId) {
    return node->course.courseId.compare(courseId) > 0;
}

bool operator==(Node *node, const std::string& courseId) {
    return node->course.courseId == courseId;
}

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() : root(nullptr), nodeCount(0) {
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    Clear();
}

void BinarySearchTree::deleteNode(Node *node) {
    if (node == nullptr)
        return;
    // applies post order traversing
    deleteNode(node->left);
    deleteNode(node->right);
    delete node;
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(const Course& course) {
    if (root == nullptr) {
        root = new Node(course);
        nodeCount = 1;
    } else {
        this->addNode(root, course);
    }
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(const std::string& courseId) {
    if (root != nullptr) {
        root = removeNode(root, courseId);
    }
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(const std::string& courseId) {
    Node *curr = root;

    // keep looping downwards until bottom reached or matching courseId found
    while (curr != nullptr) {
        // if match found, return current course
        if (curr == courseId) {
            return curr->course;
        }
        // if courseId is smaller than current node then traverse left
        else if (curr > courseId) {
            curr = curr->left;
        } else {
            // else larger so traverse right
            curr = curr->right;
        }
    }

    return Course(); // empty course if not found
}

/**
 * Print all courses in order
 */
void BinarySearchTree::PrintAll() {
    InOrder();
}

/**
 * Check if tree is empty
 */
bool BinarySearchTree::IsEmpty() const {
    return root == nullptr;
}

/**
 * Get size of tree
 */
size_t BinarySearchTree::Size() const {
    return nodeCount;
}

/**
 * Clear all nodes from tree
 */
void BinarySearchTree::Clear() {
    deleteNode(root);
    root = nullptr;
    nodeCount = 0;
}

/**
 * Execute callback for each course
 */
void BinarySearchTree::ForEach(std::function<void(const Course&)> callback) {
    inOrder(root, callback);
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    this->inOrder(root, [](const Course& course) {
        std::cout << course.courseId << ", " << course.title << std::endl;
    });
}

/**
 * Traverse the tree in order with custom callback
 */
void BinarySearchTree::InOrder(std::function<void(const Course&)> callback) {
    this->inOrder(root, callback);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
    this->postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
    this->preOrder(root);
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node *node, const Course& course) {
    if (node > course) {
        // if no left node
        if (node->left == nullptr) {
            node->left = new Node(course);
            nodeCount++;
        } else {
            // else recurse down the left node
            addNode(node->left, course);
        }
    } else {
        // if no right node
        if (node->right == nullptr) {
            node->right = new Node(course);
            nodeCount++;
        } else {
            // recurse down the right node
            addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node *node, std::function<void(const Course&)> callback) {
    if (node != nullptr) {
        inOrder(node->left, callback);
        callback(node->course);
        inOrder(node->right, callback);
    }
}

void BinarySearchTree::postOrder(Node *node) {
    if (node != nullptr) {
        postOrder(node->left);
        postOrder(node->right);
        std::cout << node->course.courseId << ", " << node->course.title << std::endl;
    }
}

void BinarySearchTree::preOrder(Node *node) {
    if (node != nullptr) {
        std::cout << node->course.courseId << ", " << node->course.title << std::endl;
        preOrder(node->left);
        preOrder(node->right);
    }
}

/**
 * Remove a course from some node (recursive)
 */
Node *BinarySearchTree::removeNode(Node *node, const std::string& courseId) {
    if (node == nullptr)
        return node;

    if (node > courseId) {
        // recursively go down left child and update the left node
        node->left = removeNode(node->left, courseId);
    } else if (node < courseId) {
        // recursively go down right child and update the right node
        node->right = removeNode(node->right, courseId);
    } else {
        // match found
        nodeCount--;

        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        } else if (node->left != nullptr && node->right == nullptr) {
            // save the left child before deleting the current node
            Node *left = node->left;
            delete node;
            return left;
        } else if (node->left == nullptr && node->right != nullptr) {
            Node *right = node->right;
            delete node;
            return right;
        } else {
            // has both left and right children
            // replace current node with the left-most leaf of the right child (the successor)
            Node *successor = node->right;
            while (successor->left != nullptr) {
                successor = successor->left;
            }

            // copy successor's content into current node
            node->course = successor->course;
            // remove the successor from its old position
            node->right = removeNode(node->right, successor->course.courseId);
            nodeCount++; // compensate for the decrement above since we're not actually removing a node
        }
    }
    return node;
}
//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Keyvan Gharehtapeh
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Project 2 - Refactored with separate CSV handling
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "csv_parser.h"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course
{
    string courseId; // unique identifier
    string title;
    vector<string> prereqs;
    Course() {}
};

// Internal structure for tree node
struct Node
{
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node()
    {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) : Node()
    {
        course = aCourse;
    }
};

bool operator<(const Course &b1, const Course &b2)
{
    return b1.courseId.compare(b2.courseId) < 0;
}

bool operator>(const Course &b1, const Course &b2)
{
    return b1.courseId.compare(b2.courseId) > 0;
}

bool operator<(Node *node, const Course &b) { return node->course < b; }

bool operator>(Node *node, const Course &b) { return node->course > b; }

bool operator<(Node *node, string courseId)
{
    return node->course.courseId.compare(courseId) < 0;
}

bool operator>(Node *node, string courseId)
{
    return node->course.courseId.compare(courseId) > 0;
}

bool operator==(Node *node, string courseId)
{
    return node->course.courseId == courseId;
}

void displayCourse(Course &course, bool showPrereqs = false);

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree
{

private:
    Node *root;

    void addNode(Node *node, const Course &course);
    void inOrder(Node *node);
    void postOrder(Node *node);
    void preOrder(Node *node);
    Node *removeNode(Node *node, string courseId);
    void deleteNode(Node *node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void PostOrder();
    void PreOrder();
    void Insert(const Course &course);
    void Remove(string courseId);
    Course Search(string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree()
{
    // root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree()
{
    //  recurse from root deleting every node

    deleteNode(root);
}

void BinarySearchTree::deleteNode(Node *node)
{
    if (node == nullptr)
        return;
    // applies post order traversing
    deleteNode(node->left);
    deleteNode(node->right);
    delete node;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder()
{
    // call inOrder fuction and pass root
    this->inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder()
{
    // postOrder root
    this->postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder()
{
    // preOrder root
    this->preOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(const Course &course)
{
    // if root equal to null ptr
    // root is equal to new node course
    // else
    // add Node root and course
    if (root == nullptr)
    {
        root = new Node(course);
    }
    else
    {
        this->addNode(root, course);
    }
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseId)
{
    // remove node root courseID
    if (root != nullptr)
    {
        root = removeNode(root, courseId);
    }
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId)
{
    // set current node equal to root
    Course course;
    course.courseId = courseId;
    Node *curr = root;

    // keep looping downwards until bottom reached or matching courseId found
    while (curr != nullptr)
    {
        // if match found, return current course
        if (curr == courseId)
        {
            return curr->course;
        }
        // if course is smaller than current node then traverse left
        else if (curr > course)
        {
            curr = curr->left;
        }
        else
        {
            // else larger so traverse right
            curr = curr->right;
        }
    }

    course = Course(); // empty course;
    return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node *node, const Course &course)
{
    // if node is larger then add to left
    //
    if (node > course)
    {
        // if no left node
        // this node becomes left
        if (node->left == nullptr)
        {
            node->left = new Node(course);
        }
        else
        {
            // else recurse down the left node
            addNode(node->left, course);
        }
    }
    else
    {
        // if no right node
        // this node becomes right
        if (node->right == nullptr)
        {
            node->right = new Node(course);
        }
        else
        {
            // recurse down the left node
            addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node *node)
{
    // if node is not equal to null ptr
    if (node != nullptr)
    {
        // InOrder left
        inOrder(node->left);
        // output courseID, title, amount, prereqs
        displayCourse(node->course);
        // InOder right
        inOrder(node->right);
    }
}
void BinarySearchTree::postOrder(Node *node)
{
    // if node is not equal to null ptr
    if (node != nullptr)
    {
        postOrder(node->left);
        postOrder(node->right);
        displayCourse(node->course);
    }
}

void BinarySearchTree::preOrder(Node *node)
{
    // if node is not equal to null ptr
    // output courseID, title, amount, prereqs
    // postOrder left
    // postOrder right
    if (node != nullptr)
    {
        displayCourse(node->course);
        preOrder(node->left);
        preOrder(node->right);
    }
}

/**
 * Remove a course from some node (recursive)
 */
Node *BinarySearchTree::removeNode(Node *node, string courseId)
{
    if (node == nullptr)
        return node;

    // (otherwise recurse down the left subtree)

    if (node > courseId)
    {

        // recursively go down left child and update the left node
        node->left = removeNode(node->left, courseId);
    }
    else if (node < courseId)
    {

        // recursively go down right child and update the left node
        node->right = removeNode(node->right, courseId);
    }
    else
    {

        // match found

        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;
            return nullptr; // makes the parent node to update the child to
                            // nullptr now that it is deleted
        }
        else if (node->left != nullptr && node->right == nullptr)
        {

            // save the left child before deleting the current node
            Node *left = node->left;
            delete node;
            // return the saved child so the parent will assign it to the
            // appropriate child
            return left;
        }
        else if (node->left == nullptr && node->right != nullptr)
        {
            Node *right = node->right;
            delete node;
            // return the saved child so the parent will assign it to the
            // appropriate child
            return right;
        }
        else
        {
            // has both left and right children
            // we'll replace current node with the left-most leaf of the
            // right child (the successor)
            Node *successor = node->right;
            while (successor->left != nullptr)
            {
                successor = successor->left;
            }

            // copy successor's content into current node, essentially
            // removing the found courseId
            node->course = successor->course;
            //  remove the successor from its old position since it has been
            //  rotated to the current node's position
            node->right = removeNode(node->right, successor->course.courseId);
        }
    }
    return node;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course &course, bool showPrereqs)
{
    cout << course.courseId << ", " << course.title << endl;

    if (showPrereqs)
    {
        cout << "Prerequisites: ";
        bool first = true;
        std::vector<std::string>::const_iterator it;
        for (it = course.prereqs.begin(); it != course.prereqs.end(); ++it)
        {

            if (first)
                first = false;
            else
                cout << ", ";

            cout << *it;
        }
        cout << endl;
    }

    return;
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree *bst)
{
    cout << "Loading CSV file " << csvPath << endl;

    try
    {
        // initialize the CSV Parser using the given path
        csv::Parser file = csv::Parser(csvPath);

        // read and display header row - optional
        vector<string> header = file.getHeader();
        for (auto const &c : header)
        {
            cout << c << " | ";
        }
        cout << "" << endl;

        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++)
        {
            // Create a data structure and add to the collection of courses
            Course course;
            course.courseId = file[i][0];
            course.title = file[i][1];

            // Skip rows with empty course ID or title
            if (course.courseId.empty() || course.title.empty())
            {
                cout << "Warning: Skipping row " << i + 1 << " due to missing course ID or title" << endl;
                continue;
            }

            // Convert course ID to uppercase for consistency
            transform(course.courseId.begin(), course.courseId.end(), course.courseId.begin(), ::toupper);

            for (int k = 2; k < file[i].size(); k++)
            {
                string prereq = file[i][k];
                if (!prereq.empty())
                {
                    transform(prereq.begin(), prereq.end(), prereq.begin(), ::toupper);
                    course.prereqs.push_back(prereq);
                }
            }

            // push this course to the end
            bst->Insert(course);
        }
        cout << "Successfully loaded " << file.rowCount() << " courses." << endl;
    }
    catch (csv::Error &e)
    {
        std::cerr << "CSV Error: " << e.what() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << "Error loading courses: " << e.what() << std::endl;
    }
}

/**
 * The one and only main() method
 */
int main(int argc, char *argv[])
{

    // process command line arguments
    string csvPath, courseId;
    switch (argc)
    {
    case 2:
        csvPath = argv[1];

    default:
        csvPath = "ABCU_Advising_Program_Input.csv";
    }

    // Define a binary search tree to hold all courses
    BinarySearchTree *bst;
    bst = new BinarySearchTree();
    Course course;

    int choice = 0;
    while (choice != 9)
    {
        cout << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << endl
             << "What would you like to do? ";
        cin >> choice;
        switch (choice)
        {

        case 1:

            // Complete the method call to load the courses
            loadCourses(csvPath, bst);

            break;

        case 2:
            cout << "Here is a sample schedule: " << endl
                 << endl;

            bst->InOrder();
            break;

        case 3:

            cout << "What course do you want to know about? ";
            cin >> courseId;

            // Convert to uppercase for consistent searching
            transform(courseId.begin(), courseId.end(), courseId.begin(), ::toupper);

            if (courseId.empty())
            {
                cout << "Invalid course ID. Please enter a valid course ID." << endl;
                break;
            }

            course = bst->Search(courseId);

            if (!course.courseId.empty())
            {
                displayCourse(course, true);
            }
            else
            {
                cout << "Course Id " << courseId << " not found." << endl;
            }

            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
        }
    }

    // Clean up allocated memory
    delete bst;
    return 0;
}
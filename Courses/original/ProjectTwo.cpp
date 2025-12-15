//============================================================================
// Name        : CourseInfo.cpp
// Author      : Keyvan Gharehtapeh
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Project 2
//============================================================================

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
	string courseId; // unique identifier
	string title;
	vector<string> prereqs;
	Course() {}
};

// Internal structure for tree node
struct Node {
	Course course;	
	Node *left;
	Node *right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a course
	Node(Course aCourse) :
		Node() {
		course = aCourse;
	}
};

bool operator<(Course &b1, Course &b2) {
	return b1.courseId.compare(b2.courseId) < 0;
}

bool operator>(Course &b1, Course &b2) {
	return b1.courseId.compare(b2.courseId) > 0;
}

bool operator<(Node *node, Course &b) { return node->course < b; }

bool operator>(Node *node, Course &b) { return node->course > b; }

bool operator<(Node *node, string courseId) {
	return node->course.courseId.compare(courseId) < 0;
}

bool operator>(Node *node, string courseId) {
	return node->course.courseId.compare(courseId) > 0;
}

bool operator==(Node *node, string courseId) {
	return node->course.courseId == courseId;
}

void displayCourse(Course &course, bool showPrereqs = false);

namespace csv {
class Error : public std::runtime_error {

public:
	Error(const std::string &msg) :
		std::runtime_error(std::string("CSVparser : ").append(msg)) {}
};

class Row {
public:
	Row(const std::vector<std::string> &);
	~Row(void);

public:
	unsigned int size(void) const;
	void push(const std::string &);
	bool set(const std::string &, const std::string &);

private:
	const std::vector<std::string> _header;
	std::vector<std::string> _values;

public:
	template <typename T> const T getValue(unsigned int pos) const {
		if (pos < _values.size()) {
			T res;
			std::stringstream ss;
			ss << _values[pos];
			ss >> res;
			return res;
		}
		throw Error("can't return this value (doesn't exist)");
	}
	const std::string operator[](unsigned int) const;
	const std::string operator[](const std::string &valueName) const;
	friend std::ostream &operator<<(std::ostream &os, const Row &row);
	friend std::ofstream &operator<<(std::ofstream &os, const Row &row);
};

enum DataType { eFILE = 0, ePURE = 1 };

class Parser {

public:
	Parser(const std::string &, const DataType &type = eFILE, char sep = ',');
	~Parser(void);

public:
	Row &getRow(unsigned int row) const;
	unsigned int rowCount(void) const;
	unsigned int columnCount(void) const;
	std::vector<std::string> getHeader(void) const;
	const std::string getHeaderElement(unsigned int pos) const;
	const std::string &getFileName(void) const;

public:
	bool deleteRow(unsigned int row);
	bool addRow(unsigned int pos, const std::vector<std::string> &);
	void sync(void) const;

protected:
	void parseHeader(void);
	void parseContent(void);

private:
	std::string _file;
	const DataType _type;
	const char _sep;
	std::vector<std::string> _originalFile;
	std::vector<std::string> _header;
	std::vector<Row *> _content;

public:
	Row &operator[](unsigned int row) const;
};

Parser::Parser(const std::string &data, const DataType &type, char sep) :
	_type(type), _sep(sep) {
	std::string line;
	if (type == eFILE) {
		_file = data;
		std::ifstream ifile(_file.c_str());
		if (ifile.is_open()) {
			while (ifile.good()) {
				getline(ifile, line);
				if (line != "")
					_originalFile.push_back(line);
			}
			ifile.close();

			if (_originalFile.size() == 0)
				throw Error(std::string("No Data in ").append(_file));

			parseHeader();
			parseContent();
		} else
			throw Error(std::string("Failed to open ").append(_file));
	} else {
		std::istringstream stream(data);
		while (std::getline(stream, line))
			if (line != "")
				_originalFile.push_back(line);
		if (_originalFile.size() == 0)
			throw Error(std::string("No Data in pure content"));

		parseHeader();
		parseContent();
	}
}

Parser::~Parser(void) {
	std::vector<Row *>::iterator it;

	for (it = _content.begin(); it != _content.end(); it++)
		delete *it;
}

void Parser::parseHeader(void) {
	std::stringstream ss(_originalFile[0]);
	std::string item;

	while (std::getline(ss, item, _sep)) {
		_header.push_back(item);
		//cout << "added header field " << item << endl;
		}
}

void Parser::parseContent(void) {
	std::vector<std::string>::iterator it;

	it = _originalFile.begin();
	it++; // skip header

	for (; it != _originalFile.end(); it++) {
		bool quoted = false;
		int tokenStart = 0;
		unsigned int i = 0;

		Row *row = new Row(_header);

		for (; i != it->length(); i++) {
			if (it->at(i) == '"')
				quoted = ((quoted) ? (false) : (true));
			else if (it->at(i) == ',' && !quoted) {
				row->push(it->substr(tokenStart, i - tokenStart));
				tokenStart = i + 1;
			}
		}

		// end
		row->push(it->substr(tokenStart, it->length() - tokenStart));

		// if value(s) missing
		if (row->size() != _header.size())
			throw Error("corrupted data !");
		_content.push_back(row);
	}
}

Row &Parser::getRow(unsigned int rowPosition) const {
	if (rowPosition < _content.size())
		return *(_content[rowPosition]);
	throw Error("can't return this row (doesn't exist)");
}

Row &Parser::operator[](unsigned int rowPosition) const {
	return Parser::getRow(rowPosition);
}

unsigned int Parser::rowCount(void) const { return _content.size(); }

unsigned int Parser::columnCount(void) const { return _header.size(); }

std::vector<std::string> Parser::getHeader(void) const { return _header; }

const std::string Parser::getHeaderElement(unsigned int pos) const {
	if (pos >= _header.size())
		throw Error("can't return this header (doesn't exist)");
	return _header[pos];
}

bool Parser::deleteRow(unsigned int pos) {
	if (pos < _content.size()) {
		delete *(_content.begin() + pos);
		_content.erase(_content.begin() + pos);
		return true;
	}
	return false;
}

bool Parser::addRow(unsigned int pos, const std::vector<std::string> &r) {
	Row *row = new Row(_header);

	for (auto it = r.begin(); it != r.end(); it++)
		row->push(*it);

	if (pos <= _content.size()) {
		_content.insert(_content.begin() + pos, row);
		return true;
	}
	return false;
}

void Parser::sync(void) const {
	if (_type == DataType::eFILE) {
		std::ofstream f;
		f.open(_file, std::ios::out | std::ios::trunc);

		// header
		unsigned int i = 0;
		for (auto it = _header.begin(); it != _header.end(); it++) {
			f << *it;
			if (i < _header.size() - 1)
				f << ",";
			else
				f << std::endl;
			i++;
		}

		for (auto it = _content.begin(); it != _content.end(); it++)
			f << **it << std::endl;
		f.close();
	}
}

const std::string &Parser::getFileName(void) const { return _file; }

/*
** ROW
*/

Row::Row(const std::vector<std::string> &header) :
	_header(header) {}

Row::~Row(void) {}

unsigned int Row::size(void) const { return _values.size(); }

void Row::push(const std::string &value) { _values.push_back(value); }

bool Row::set(const std::string &key, const std::string &value) {
	std::vector<std::string>::const_iterator it;
	int pos = 0;

	for (it = _header.begin(); it != _header.end(); it++) {
		if (key == *it) {
			_values[pos] = value;
			return true;
		}
		pos++;
	}
	return false;
}

const std::string Row::operator[](unsigned int valuePosition) const {
	if (valuePosition < _values.size())
		return _values[valuePosition];
	throw Error("can't return this value (doesn't exist)");
}

const std::string Row::operator[](const std::string &key) const {
	std::vector<std::string>::const_iterator it;
	int pos = 0;

	for (it = _header.begin(); it != _header.end(); it++) {
		if (key == *it)
			return _values[pos];
		pos++;
	}

	throw Error("can't return this value (doesn't exist)");
}

std::ostream &operator<<(std::ostream &os, const Row &row) {
	for (unsigned int i = 0; i != row._values.size(); i++)
		os << row._values[i] << " | ";

	return os;
}

std::ofstream &operator<<(std::ofstream &os, const Row &row) {
	for (unsigned int i = 0; i != row._values.size(); i++) {
		os << row._values[i];
		if (i < row._values.size() - 1)
			os << ",";
	}
	return os;
}
} // namespace csv

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
	Node *root;

	void addNode(Node *node, Course course);
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
	void Insert(Course course);
	void Remove(string courseId);
	Course Search(string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
	// root is equal to nullptr
	root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
	//  recurse from root deleting every node

	deleteNode(root);
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
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
	// call inOrder fuction and pass root
	this->inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
	// postOrder root
	this->postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
	// preOrder root
	this->preOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
	// if root equarl to null ptr
	// root is equal to new node course
	// else
	// add Node root and course
	if (root == nullptr) {
		root = new Node(course);
	} else {
		this->addNode(root, course);
	}
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseId) {
	// remove node root courseID
	if (root != nullptr) {
		Node *node = removeNode(root, courseId);
		if (node == root) {
		}
	}
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId) {
	// set current node equal to root
	Course course;
	course.courseId = courseId;
	Node *curr = root;

	// keep looping downwards until bottom reached or matching courseId found
	while (curr != nullptr) {
		// if match found, return current course
		if (curr == courseId) {
			return curr->course;
		}
		// if course is smaller than current node then traverse left
		else if (curr > course) {
			curr = curr->left;
		} else {
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
void BinarySearchTree::addNode(Node *node, Course course) {
	// if node is larger then add to left
	//
	if (node > course) {
		// if no left node
		// this node becomes left
		if (node->left == nullptr) {
			node->left = new Node(course);
		} else {
			// else recurse down the left node
			addNode(node->left, course);
		}

	} else {
		// if no right node
		// this node becomes right
		if (node->right == nullptr) {
			node->right = new Node(course);
		} else {
			// recurse down the left node
			addNode(node->right, course);
		}
	}
}

void BinarySearchTree::inOrder(Node *node) {
	// if node is not equal to null ptr
	if (node != nullptr) {
		// InOrder left
		inOrder(node->left);
		// output courseID, title, amount, prereqs
		displayCourse(node->course);
		// InOder right
		inOrder(node->right);
	}
}
void BinarySearchTree::postOrder(Node *node) {
	// if node is not equal to null ptr
	if (node != nullptr) {
		postOrder(node->left);
		postOrder(node->right);
		displayCourse(node->course);
	}
}

void BinarySearchTree::preOrder(Node *node) {
	// if node is not equal to null ptr
	// output courseID, title, amount, prereqs
	// postOrder left
	// postOrder right
	if (node != nullptr) {
		displayCourse(node->course);
		preOrder(node->left);
		preOrder(node->right);
	}
}

/**
 * Remove a course from some node (recursive)
 */
Node *BinarySearchTree::removeNode(Node *node, string courseId) {
	if (node == nullptr)
		return node;

	// (otherwise recurse down the left subtree)

	if (node > courseId) {

		// recursively go down left child and update the left node
		node->left = removeNode(node->left, courseId);

	} else if (node < courseId) {

		// recursively go down right child and update the left node
		node->right = removeNode(node->right, courseId);

	} else {

		// match found

		if (node->left == nullptr && node->right == nullptr) {
			delete node;
			return nullptr; // makes the parent node to update the child to
							// nullptr now that it is deleted
		} else if (node->left != nullptr && node->right == nullptr) {

			// save the left child before deleting the current node
			Node *left = node->left;
			delete node;
			// return the saved child so the parent will assign it to the
			// appropriate child
			return left;

		} else if (node->left != nullptr && node->right == nullptr) {

			Node *right = node->right;
			delete node;
			// return the saved child so the parent will assign it to the
			// appropriate child
			return right;

		} else {
			// has both left and right children
			// we'll replace current node with the left-most leaf of the
			// right child (the successor)
			Node *successor = node->right;
			while (successor->left != nullptr) {
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
void displayCourse(Course &course, bool showPrereqs) {
	cout << course.courseId << ", " << course.title << endl;

	if (showPrereqs) {
		cout << "Prerequisites: ";
		bool first = true;
		std::vector<std::string>::const_iterator it;
		for (it = course.prereqs.begin(); it != course.prereqs.end(); ++it) {

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
void loadCourses(string csvPath, BinarySearchTree *bst) {
	cout << "Loading CSV file " << csvPath << endl;

	// initialize the CSV Parser using the given path
	csv::Parser file = csv::Parser(csvPath);

	// read and display header row - optional
	vector<string> header = file.getHeader();
	for (auto const &c : header) {
		cout << c << " | ";
	}
	cout << "" << endl;
	try {
		// loop to read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			// Create a data structure and add to the collection of courses
			Course course;
			course.courseId = file[i][0];
			course.title = file[i][1];

			for (int k = 2; k < file[i].size(); k++) {
				course.prereqs.push_back(file[i][k]);
			}

			// push this course to the end
			bst->Insert(course);
		}
	} catch (csv::Error &e) {
		std::cerr << e.what() << std::endl;
	}
}

/**
 * The one and only main() method
 */
int main(int argc, char *argv[]) {

	// process command line arguments
	string csvPath, courseId;
	switch (argc) {
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
	while (choice != 9) {
		cout << endl;
		cout << "  1. Load Data Structure." << endl;
		cout << "  2. Print Course List." << endl;
		cout << "  3. Print Course." << endl;
		cout << "  9. Exit" << endl;
		cout << endl << "What would you like to do? ";
		cin >> choice;
		switch (choice) {

		case 1:

			// Complete the method call to load the courses
			loadCourses(csvPath, bst);

			break;

		case 2:
			cout << "Here is a sample schedule: " << endl << endl;

			bst->InOrder();
			break;

		case 3:

			cout << "What course do you want to know about? ";
			cin >> courseId;

			course = bst->Search(courseId);

			if (!course.courseId.empty()) {
				displayCourse(course, true);
			} else {
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

	return 0;
}

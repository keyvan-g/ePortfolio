//============================================================================
// Name        : dag_data_structure.cpp
// Author      : Keyvan Gharehtapeh
// Version     : 1.0
// Description : Directed Acyclic Graph implementation
//============================================================================

#include "dag_data_structure.h"
#include <iostream>
#include <algorithm>
#include <stack>

/**
 * Default constructor
 */
DAG::DAG() : nodeCount(0)
{
}

/**
 * Destructor
 */
DAG::~DAG()
{
    Clear();
}

/**
 * Insert a course
 */
void DAG::Insert(const Course &course)
{
    // Check if course already exists
    if (nodes.find(course.courseId) != nodes.end())
    {
        return; // Course already exists, don't insert duplicate
    }

    // Create new node
    DAGNode* newNode = new DAGNode(course);
    nodes[course.courseId] = newNode;
    nodeCount++;

    // Build edges after all courses are loaded
    buildEdges();
}

/**
 * Remove a course
 */
void DAG::Remove(const std::string &courseId)
{
    auto it = nodes.find(courseId);
    if (it == nodes.end())
    {
        return; // Course not found
    }

    DAGNode* nodeToRemove = it->second;

    // Remove edges from prerequisites
    for (DAGNode* prereq : nodeToRemove->prerequisites)
    {
        auto& dependents = prereq->dependents;
        dependents.erase(std::remove(dependents.begin(), dependents.end(), nodeToRemove), 
                        dependents.end());
    }

    // Remove edges to dependents and update their in-degrees
    for (DAGNode* dependent : nodeToRemove->dependents)
    {
        auto& prereqs = dependent->prerequisites;
        prereqs.erase(std::remove(prereqs.begin(), prereqs.end(), nodeToRemove), 
                     prereqs.end());
        dependent->inDegree--;
    }

    // Remove from nodes map and delete
    nodes.erase(it);
    delete nodeToRemove;
    nodeCount--;
}

/**
 * Search for a course
 */
Course DAG::Search(const std::string &courseId)
{
    auto it = nodes.find(courseId);
    if (it != nodes.end())
    {
        return it->second->course;
    }
    else
    {
        return Course(); // empty course if not found
    }
}

/**
 * Print all courses in topological order
 */
void DAG::PrintAll()
{
    PrintTopologicalOrder();
}

/**
 * Check if DAG is empty
 */
bool DAG::IsEmpty() const
{
    return nodes.empty();
}

/**
 * Get size of DAG
 */
size_t DAG::Size() const
{
    return nodeCount;
}

/**
 * Clear all nodes from DAG
 */
void DAG::Clear()
{
    for (auto& pair : nodes)
    {
        delete pair.second;
    }
    nodes.clear();
    nodeCount = 0;
}

/**
 * Execute callback for each course in topological order
 */
void DAG::ForEach(std::function<void(const Course &)> callback)
{
    std::vector<std::string> sortedCourses = topologicalSort();
    for (const std::string& courseId : sortedCourses)
    {
        auto it = nodes.find(courseId);
        if (it != nodes.end())
        {
            callback(it->second->course);
        }
    }
}

/**
 * Print courses in dependency graph format
 */
void DAG::PrintTopologicalOrder()
{
    std::cout << "\n========== COURSE DEPENDENCY GRAPH ==========\n" << std::endl;
    
    // Get courses organized by levels (foundation courses first)
    std::vector<std::string> sortedCourses = topologicalSort();
    
    // Track which courses we've already displayed to avoid duplicates
    std::unordered_set<std::string> displayed;
    
    for (const std::string& courseId : sortedCourses)
    {
        auto it = nodes.find(courseId);
        if (it == nodes.end() || displayed.count(courseId))
            continue;
            
        DAGNode* courseNode = it->second;
        
        // Display course information
        std::cout << "[" << courseNode->course.courseId << "] " 
                  << courseNode->course.title << std::endl;
        
        // Show prerequisites (incoming edges)
        if (!courseNode->prerequisites.empty())
        {
            std::cout << "  Prerequisites: ";
            for (size_t i = 0; i < courseNode->prerequisites.size(); ++i)
            {
                if (i > 0) std::cout << ", ";
                std::cout << courseNode->prerequisites[i]->course.courseId;
            }
            std::cout << std::endl;
        }
        
        // Show dependents (outgoing edges)
        if (!courseNode->dependents.empty())
        {
            std::cout << "  Enables: ";
            for (size_t i = 0; i < courseNode->dependents.size(); ++i)
            {
                if (i > 0) std::cout << ", ";
                std::cout << courseNode->dependents[i]->course.courseId;
            }
            std::cout << std::endl;
        }
        
        // Visual representation of dependencies
        if (!courseNode->prerequisites.empty() || !courseNode->dependents.empty())
        {
            std::cout << "  Graph: ";
            
            // Show prerequisite arrows pointing to this course
            if (!courseNode->prerequisites.empty())
            {
                for (DAGNode* prereq : courseNode->prerequisites)
                {
                    std::cout << prereq->course.courseId << " → ";
                }
            }
            
            std::cout << "[" << courseNode->course.courseId << "]";
            
            // Show arrows pointing to dependent courses
            if (!courseNode->dependents.empty())
            {
                std::cout << " → ";
                for (size_t i = 0; i < courseNode->dependents.size(); ++i)
                {
                    if (i > 0) std::cout << ", ";
                    std::cout << courseNode->dependents[i]->course.courseId;
                }
            }
            std::cout << std::endl;
        }
        
        std::cout << std::endl; // Blank line between courses
        displayed.insert(courseId);
    }
    
    // Show summary statistics
    std::cout << "========== GRAPH STATISTICS ==========\n";
    std::cout << "Total Courses: " << nodeCount << std::endl;
    
    // Count courses by dependency level
    int foundationCourses = 0;
    int intermediateCourses = 0;
    int advancedCourses = 0;
    
    for (auto& pair : nodes)
    {
        int prereqCount = pair.second->prerequisites.size();
        if (prereqCount == 0) foundationCourses++;
        else if (prereqCount <= 2) intermediateCourses++;
        else advancedCourses++;
    }
    
    std::cout << "Foundation Courses (no prerequisites): " << foundationCourses << std::endl;
    std::cout << "Intermediate Courses (1-2 prerequisites): " << intermediateCourses << std::endl;
    std::cout << "Advanced Courses (3+ prerequisites): " << advancedCourses << std::endl;
    std::cout << "Has Cycles: " << (hasCycle() ? "Yes" : "No") << std::endl;
    std::cout << "=============================================\n" << std::endl;
}

/**
 * Get prerequisites for a course
 */
std::vector<std::string> DAG::GetPrerequisites(const std::string& courseId)
{
    std::vector<std::string> prereqs;
    auto it = nodes.find(courseId);
    if (it != nodes.end())
    {
        for (DAGNode* prereq : it->second->prerequisites)
        {
            prereqs.push_back(prereq->course.courseId);
        }
    }
    return prereqs;
}

/**
 * Get courses that depend on this course
 */
std::vector<std::string> DAG::GetDependents(const std::string& courseId)
{
    std::vector<std::string> dependents;
    auto it = nodes.find(courseId);
    if (it != nodes.end())
    {
        for (DAGNode* dependent : it->second->dependents)
        {
            dependents.push_back(dependent->course.courseId);
        }
    }
    return dependents;
}

/**
 * Check if the DAG has cycles
 */
bool DAG::HasCycle()
{
    return hasCycle();
}

/**
 * Add edge from prerequisite to dependent course
 */
void DAG::addEdge(const std::string& fromCourseId, const std::string& toCourseId)
{
    auto fromIt = nodes.find(fromCourseId);
    auto toIt = nodes.find(toCourseId);
    
    if (fromIt != nodes.end() && toIt != nodes.end())
    {
        DAGNode* fromNode = fromIt->second;
        DAGNode* toNode = toIt->second;
        
        // Check if edge already exists
        for (DAGNode* existing : fromNode->dependents)
        {
            if (existing == toNode)
                return; // Edge already exists
        }
        
        // Add edge: from -> to
        fromNode->dependents.push_back(toNode);
        toNode->prerequisites.push_back(fromNode);
        toNode->inDegree++;
    }
}

/**
 * Build all prerequisite edges after all courses are loaded
 */
void DAG::buildEdges()
{
    // Clear existing edges
    for (auto& pair : nodes)
    {
        pair.second->prerequisites.clear();
        pair.second->dependents.clear();
        pair.second->inDegree = 0;
    }
    
    // Build edges based on course prerequisites
    for (auto& pair : nodes)
    {
        DAGNode* courseNode = pair.second;
        const Course& course = courseNode->course;
        
        for (const auto& prereqId : course.prereqs)
        {
            if (prereqId.empty())
                continue;
                
            auto prereqIt = nodes.find(prereqId);
            if (prereqIt != nodes.end())
            {
                addEdge(prereqId, course.courseId);
            }
        }
    }
}

/**
 * Topological sort using Kahn's algorithm
 */
std::vector<std::string> DAG::topologicalSort()
{
    std::vector<std::string> result;
    std::queue<DAGNode*> zeroInDegreeQueue;
    
    // Create a copy of in-degrees to avoid modifying the original
    std::unordered_map<DAGNode*, int> inDegrees;
    for (auto& pair : nodes)
    {
        inDegrees[pair.second] = pair.second->inDegree;
        if (pair.second->inDegree == 0)
        {
            zeroInDegreeQueue.push(pair.second);
        }
    }
    
    while (!zeroInDegreeQueue.empty())
    {
        DAGNode* current = zeroInDegreeQueue.front();
        zeroInDegreeQueue.pop();
        result.push_back(current->course.courseId);
        
        // Decrease in-degree of dependent courses
        for (DAGNode* dependent : current->dependents)
        {
            inDegrees[dependent]--;
            if (inDegrees[dependent] == 0)
            {
                zeroInDegreeQueue.push(dependent);
            }
        }
    }
    
    return result;
}

/**
 * Check for cycles using DFS
 */
bool DAG::hasCycle()
{
    std::unordered_set<DAGNode*> visited;
    std::unordered_set<DAGNode*> recursionStack;
    
    for (auto& pair : nodes)
    {
        if (visited.find(pair.second) == visited.end())
        {
            if (hasCycleDFS(pair.second, visited, recursionStack))
            {
                return true;
            }
        }
    }
    return false;
}

/**
 * DFS helper for cycle detection
 */
bool DAG::hasCycleDFS(DAGNode* node, std::unordered_set<DAGNode*>& visited, 
                      std::unordered_set<DAGNode*>& recursionStack)
{
    visited.insert(node);
    recursionStack.insert(node);
    
    for (DAGNode* dependent : node->dependents)
    {
        if (recursionStack.find(dependent) != recursionStack.end())
        {
            return true; // Back edge found, cycle detected
        }
        if (visited.find(dependent) == visited.end() && 
            hasCycleDFS(dependent, visited, recursionStack))
        {
            return true;
        }
    }
    
    recursionStack.erase(node);
    return false;
}

/**
 * DFS visit helper for traversal
 */
void DAG::dfsVisit(DAGNode* node, std::unordered_set<DAGNode*>& visited, 
                   std::function<void(const Course&)> callback)
{
    visited.insert(node);
    callback(node->course);
    
    for (DAGNode* dependent : node->dependents)
    {
        if (visited.find(dependent) == visited.end())
        {
            dfsVisit(dependent, visited, callback);
        }
    }
}
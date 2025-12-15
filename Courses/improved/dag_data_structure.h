//============================================================================
// Name        : dag_data_structure.h
// Author      : Keyvan Gharehtapeh
// Version     : 1.0
// Description : Directed Acyclic Graph implementation of IDataStructure
//============================================================================

#ifndef DAG_H
#define DAG_H

#include "data_structure_interface.h"
#include "course.h"
#include <functional>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

// Internal structure for DAG node
struct DAGNode {
    Course course;
    std::vector<DAGNode*> prerequisites;  // Incoming edges (courses that must be taken before this one)
    std::vector<DAGNode*> dependents;     // Outgoing edges (courses that depend on this one)
    int inDegree;                         // Number of prerequisites

    // default constructor
    DAGNode() : inDegree(0) {}

    // initialize with a course
    DAGNode(const Course& aCourse) : course(aCourse), inDegree(0) {}
};

/**
 * Directed Acyclic Graph implementation of the IDataStructure interface
 */
class DAG : public IDataStructure {
private:
    std::unordered_map<std::string, DAGNode*> nodes;  // Map from courseId to node
    size_t nodeCount;

    // Helper methods
    void addEdge(const std::string& fromCourseId, const std::string& toCourseId);
    void buildEdges();
    std::vector<std::string> topologicalSort();
    void dfsVisit(DAGNode* node, std::unordered_set<DAGNode*>& visited, 
                  std::function<void(const Course&)> callback);
    bool hasCycle();
    bool hasCycleDFS(DAGNode* node, std::unordered_set<DAGNode*>& visited, 
                     std::unordered_set<DAGNode*>& recursionStack);

public:
    DAG();
    virtual ~DAG();

    // IDataStructure interface implementation
    void Insert(const Course& course) override;
    void Remove(const std::string& courseId) override;
    Course Search(const std::string& courseId) override;
    void PrintAll() override;
    bool IsEmpty() const override;
    size_t Size() const override;
    void Clear() override;
    void ForEach(std::function<void(const Course&)> callback) override;

    // Additional DAG-specific methods
    void PrintTopologicalOrder();
    std::vector<std::string> GetPrerequisites(const std::string& courseId);
    std::vector<std::string> GetDependents(const std::string& courseId);
    bool HasCycle();
};

#endif // DAG_H
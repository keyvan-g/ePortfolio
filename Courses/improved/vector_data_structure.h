//============================================================================
// Name        : vector_data_structure.h
// Author      : Keyvan Gharehtapeh
// Version     : 1.0
// Description : Vector-based implementation of IDataStructure for comparison
//============================================================================

#ifndef VECTOR_DATA_STRUCTURE_H
#define VECTOR_DATA_STRUCTURE_H

#include "data_structure_interface.h"
#include "course.h"
#include <vector>
#include <functional>
#include <algorithm>

/**
 * Vector-based implementation of the IDataStructure interface
 * Provides O(1) insertion but O(n) search and removal
 * Useful for comparison with BST performance
 */
class VectorDataStructure : public IDataStructure {
private:
    std::vector<Course> courses;

public:
    VectorDataStructure() = default;
    virtual ~VectorDataStructure() = default;

    // IDataStructure interface implementation
    void Insert(const Course& course) override;
    void Remove(const std::string& courseId) override;
    Course Search(const std::string& courseId) override;
    void PrintAll() override;
    bool IsEmpty() const override;
    size_t Size() const override;
    void Clear() override;
    void ForEach(std::function<void(const Course&)> callback) override;

    // Additional vector-specific methods
    void SortById();
    const std::vector<Course>& GetCourses() const { return courses; }
};

#endif // VECTOR_DATA_STRUCTURE_H
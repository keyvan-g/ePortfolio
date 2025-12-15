//============================================================================
// Name        : data_structure_interface.h
// Author      : Keyvan Gharehtapeh
// Version     : 1.0
// Description : Abstract interface for data structures that store courses
//============================================================================

#ifndef DATA_STRUCTURE_INTERFACE_H
#define DATA_STRUCTURE_INTERFACE_H

#include "course.h"
#include <functional>

/**
 * Abstract interface for data structures that can store and manipulate Course objects.
 * This allows for easy swapping between different implementations (BST, Hash Table, Vector, etc.)
 */
class IDataStructure {
public:
    virtual ~IDataStructure() = default;

    // Core operations
    virtual void Insert(const Course& course) = 0;
    virtual void Remove(const std::string& courseId) = 0;
    virtual Course Search(const std::string& courseId) = 0;
    
    // Traversal operations
    virtual void PrintAll() = 0;
    
    // Utility operations
    virtual bool IsEmpty() const = 0;
    virtual size_t Size() const = 0;
    virtual void Clear() = 0;

    // Callback-based traversal for more flexibility
    virtual void ForEach(std::function<void(const Course&)> callback) = 0;
};

#endif // DATA_STRUCTURE_INTERFACE_H
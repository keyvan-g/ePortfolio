//============================================================================
// Name        : vector_data_structure.cpp
// Author      : Keyvan Gharehtapeh
// Version     : 1.0
// Description : Vector-based implementation of IDataStructure
//============================================================================

#include "vector_data_structure.h"
#include <iostream>
#include <algorithm>

void VectorDataStructure::Insert(const Course& course) {
    // Check if course already exists
    auto it = std::find_if(courses.begin(), courses.end(),
        [&course](const Course& c) { return c.courseId == course.courseId; });
    
    if (it == courses.end()) {
        courses.push_back(course);
    } else {
        // Update existing course
        *it = course;
    }
}

void VectorDataStructure::Remove(const std::string& courseId) {
    courses.erase(
        std::remove_if(courses.begin(), courses.end(),
            [&courseId](const Course& c) { return c.courseId == courseId; }),
        courses.end()
    );
}

Course VectorDataStructure::Search(const std::string& courseId) {
    auto it = std::find_if(courses.begin(), courses.end(),
        [&courseId](const Course& c) { return c.courseId == courseId; });
    
    if (it != courses.end()) {
        return *it;
    }
    
    return Course(); // empty course if not found
}

void VectorDataStructure::PrintAll() {
    // Sort before printing for consistent output
    std::vector<Course> sortedCourses = courses;
    std::sort(sortedCourses.begin(), sortedCourses.end());
    
    for (const auto& course : sortedCourses) {
        std::cout << course.courseId << ", " << course.title << std::endl;
    }
}

bool VectorDataStructure::IsEmpty() const {
    return courses.empty();
}

size_t VectorDataStructure::Size() const {
    return courses.size();
}

void VectorDataStructure::Clear() {
    courses.clear();
}

void VectorDataStructure::ForEach(std::function<void(const Course&)> callback) {
    for (const auto& course : courses) {
        callback(course);
    }
}

void VectorDataStructure::SortById() {
    std::sort(courses.begin(), courses.end());
}
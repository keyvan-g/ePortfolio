//============================================================================
// Name        : course.h
// Author      : Keyvan Gharehtapeh
// Version     : 1.0
// Description : Course data structure definition
//============================================================================

#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>

// define a structure to hold course information
struct Course {
    std::string courseId; // unique identifier
    std::string title;
    std::vector<std::string> prereqs;
    
    Course() = default;
    Course(const std::string& id, const std::string& courseTitle) 
        : courseId(id), title(courseTitle) {}
    Course(const std::string& id, const std::string& courseTitle, const std::vector<std::string>& prerequisites) 
        : courseId(id), title(courseTitle), prereqs(prerequisites) {}
};

// Comparison operators for Course
bool operator<(const Course &b1, const Course &b2);
bool operator>(const Course &b1, const Course &b2);
bool operator==(const Course &b1, const Course &b2);

#endif // COURSE_H
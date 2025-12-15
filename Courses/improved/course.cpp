//============================================================================
// Name        : course.cpp
// Author      : Keyvan Gharehtapeh
// Version     : 1.0
// Description : Course data structure implementation
//============================================================================

#include "course.h"

bool operator<(const Course &b1, const Course &b2) {
    return b1.courseId.compare(b2.courseId) < 0;
}

bool operator>(const Course &b1, const Course &b2) {
    return b1.courseId.compare(b2.courseId) > 0;
}

bool operator==(const Course &b1, const Course &b2) {
    return b1.courseId == b2.courseId;
}
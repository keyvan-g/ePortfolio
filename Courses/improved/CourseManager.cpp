//============================================================================
// Name        : ProjectTwo_final.cpp
// Author      : Keyvan Gharehtapeh
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Project 2 - Final version with modular data structures
//============================================================================

#include <iostream>
#include <memory>
#include <string>
#include <algorithm>
#include <iomanip>
#include "csv_parser.h"
#include "course.h"
#include "data_structure_interface.h"
#include "data_structure_factory.h"

/**
 * Display the course information to the console
 *
 * @param course struct containing the course info
 * @param showPrereqs whether to show prerequisites
 */
void displayCourse(const Course &course, bool showPrereqs = false) {
    std::cout << course.courseId << ", " << course.title << std::endl;

    if (showPrereqs && !course.prereqs.empty()) {
        std::cout << "Prerequisites: ";
        bool first = true;
        for (const auto& prereq : course.prereqs) {
            if (!first) std::cout << ", ";
            std::cout << prereq;
            first = false;
        }
        std::cout << std::endl;
    }
}

/**
 * Load a CSV file containing courses into the data structure
 *
 * @param csvPath the path to the CSV file to load
 * @param dataStructure pointer to the data structure to load courses into
 */
void loadCourses(const std::string& csvPath, IDataStructure* dataStructure) {
    std::cout << "Loading CSV file " << csvPath << std::endl;

    try {
        // initialize the CSV Parser using the given path
        csv::Parser file = csv::Parser(csvPath);

        // read and display header row - optional
        std::vector<std::string> header = file.getHeader();
        // for (const auto &c : header) {
        //     std::cout << c << " | ";
        // }
        // std::cout << std::endl;

        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            // Create a data structure and add to the collection of courses
            Course course;
            course.courseId = file[i][0];
            course.title = file[i][1];

            // Skip rows with empty course ID or title
            if (course.courseId.empty() || course.title.empty()) {
                std::cout << "Warning: Skipping row " << i + 1 << " due to missing course ID or title" << std::endl;
                continue;
            }

            // Convert course ID to uppercase for consistency
            std::transform(course.courseId.begin(), course.courseId.end(), course.courseId.begin(), ::toupper);

            for (size_t k = 2; k < file[i].size(); k++) {
                std::string prereq = file[i][k];
                if (!prereq.empty()) {
                    std::transform(prereq.begin(), prereq.end(), prereq.begin(), ::toupper);
                    course.prereqs.push_back(prereq);
                }
            }

            // insert course into data structure
            dataStructure->Insert(course);
        }
        std::cout << "Successfully loaded " << dataStructure->Size() << " courses." << std::endl;
    } catch (csv::Error &e) {
        std::cerr << "CSV Error: " << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cerr << "Error loading courses: " << e.what() << std::endl;
    }
}

/**
 * Display menu and get user choice for data structure type
 */
DataStructureType getDataStructureChoice() {
    std::cout << "\nSelect Data Structure Type:" << std::endl;
    std::cout << "  1. Binary Search Tree (BST)" << std::endl;
    std::cout << "  2. Vector (Linear Search)" << std::endl;
    std::cout << "  3. Directed Acyclic Graph (DAG)" << std::endl;
    std::cout << "Enter choice (1-3): ";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1:
            return DataStructureType::BINARY_SEARCH_TREE;
        case 2:
            return DataStructureType::VECTOR;
        case 3:
            return DataStructureType::DAG;
        default:
            std::cout << "Invalid choice, defaulting to Binary Search Tree." << std::endl;
            return DataStructureType::BINARY_SEARCH_TREE;
    }
}

/**
 * Display performance information about the chosen data structure
 */
void displayPerformanceInfo(DataStructureType type) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "Data Structure Performance Characteristics:" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    switch (type) {
        case DataStructureType::BINARY_SEARCH_TREE:
            std::cout << "Binary Search Tree:" << std::endl;
            std::cout << "  - Search: O(log n) average, O(n) worst case" << std::endl;
            std::cout << "  - Insert: O(log n) average, O(n) worst case" << std::endl;
            std::cout << "  - Remove: O(log n) average, O(n) worst case" << std::endl;
            std::cout << "  - Memory: O(n) with additional pointer overhead" << std::endl;
            std::cout << "  - Maintains sorted order automatically" << std::endl;
            break;
        case DataStructureType::VECTOR:
            std::cout << "Vector (Linear):" << std::endl;
            std::cout << "  - Search: O(n)" << std::endl;
            std::cout << "  - Insert: O(1) at end, O(n) for duplicates check" << std::endl;
            std::cout << "  - Remove: O(n)" << std::endl;
            std::cout << "  - Memory: O(n) with good cache locality" << std::endl;
            std::cout << "  - Simple implementation, good for small datasets" << std::endl;
            break;
        case DataStructureType::DAG:
            std::cout << "Directed Acyclic Graph (DAG):" << std::endl;
            std::cout << "  - Search: O(log n) average, O(n) worst case" << std::endl;
            std::cout << "  - Insert: O(log n) average, O(n) worst case" << std::endl;
            std::cout << "  - Remove: O(log n) average, O(n) worst case" << std::endl;
            std::cout << "  - Memory: O(n) with additional pointer overhead" << std::endl;
            std::cout << "  - Supports prerequisite relationships and topological ordering" << std::endl;
            break;
    }
    std::cout << std::string(50, '=') << std::endl;
}

/**
 * The main program
 */
int main(int argc, char *argv[]) {
    // process command line arguments
    std::string csvPath;
    switch (argc) {
        case 2:
            csvPath = argv[1];
            break;
        default:
            csvPath = "ABCU_Advising_Program_Input.csv";
    }

    std::cout << "Welcome to the Course Planner!" << std::endl;
    
    // Let user choose data structure type
    DataStructureType dsType = getDataStructureChoice();
    displayPerformanceInfo(dsType);

    // Create the data structure using the factory
    std::unique_ptr<IDataStructure> dataStructure;
    try {
        dataStructure = DataStructureFactory::Create(dsType);
        std::cout << "\nData structure created successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error creating data structure: " << e.what() << std::endl;
        return 1;
    }

    int choice = 0;
    while (choice != 9) {
        std::cout << std::endl;
        std::cout << "  1. Load Data Structure." << std::endl;
        std::cout << "  2. Print Course List." << std::endl;
        std::cout << "  3. Print Course." << std::endl;
        std::cout << "  4. Display Data Structure Info." << std::endl;
        std::cout << "  9. Exit" << std::endl;
        std::cout << std::endl << "What would you like to do? ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                loadCourses(csvPath, dataStructure.get());
                break;
            }

            case 2: {
                if (dataStructure->IsEmpty()) {
                    std::cout << "No courses loaded. Please load data first." << std::endl;
                } else {
                    std::cout << "Here is a sample schedule:" << std::endl << std::endl;
                    dataStructure->PrintAll();
                }
                break;
            }

            case 3: {
                if (dataStructure->IsEmpty()) {
                    std::cout << "No courses loaded. Please load data first." << std::endl;
                    break;
                }

                std::string courseId;
                std::cout << "What course do you want to know about? ";
                std::cin >> courseId;
                
                // Convert to uppercase for consistent searching
                std::transform(courseId.begin(), courseId.end(), courseId.begin(), ::toupper);

                if (courseId.empty()) {
                    std::cout << "Invalid course ID. Please enter a valid course ID." << std::endl;
                    break;
                }

                Course course = dataStructure->Search(courseId);

                if (!course.courseId.empty()) {
                    displayCourse(course, true);
                } else {
                    std::cout << "Course Id " << courseId << " not found." << std::endl;
                }
                break;
            }

            case 4: {
                displayPerformanceInfo(dsType);
                std::cout << "Current data structure statistics:" << std::endl;
                std::cout << "  - Number of courses: " << dataStructure->Size() << std::endl;
                std::cout << "  - Is empty: " << (dataStructure->IsEmpty() ? "Yes" : "No") << std::endl;
                break;
            }

            case 9: {
                std::cout << "Thank you for using the course planner!" << std::endl;
                break;
            }

            default: {
                std::cout << choice << " is not a valid option." << std::endl;
                break;
            }
        }
    }

    return 0;
}
//============================================================================
// Name        : data_structure_factory.cpp
// Author      : Keyvan Gharehtapeh
// Version     : 1.0
// Description : Factory for creating different data structure implementations
//============================================================================

#include "data_structure_factory.h"
#include "binary_search_tree.h"
#include "vector_data_structure.h"
#include "dag_data_structure.h"
#include <stdexcept>
#include <algorithm>

std::unique_ptr<IDataStructure> DataStructureFactory::Create(DataStructureType type) {
    switch (type) {
        case DataStructureType::BINARY_SEARCH_TREE:
            return std::make_unique<BinarySearchTree>();
        case DataStructureType::VECTOR:
            return std::make_unique<VectorDataStructure>();
        case DataStructureType::DAG:
            return std::make_unique<DAG>();
        default:
            throw std::invalid_argument("Unknown data structure type");
    }
}

std::unique_ptr<IDataStructure> DataStructureFactory::Create(const std::string& typeName) {
    std::string lowerTypeName = typeName;
    std::transform(lowerTypeName.begin(), lowerTypeName.end(), lowerTypeName.begin(), ::tolower);

    if (lowerTypeName == "bst" || lowerTypeName == "binary_search_tree" || lowerTypeName == "tree") {
        return Create(DataStructureType::BINARY_SEARCH_TREE);
    } else if (lowerTypeName == "vector" || lowerTypeName == "array") {
        return Create(DataStructureType::VECTOR);
    } else if (lowerTypeName == "dag" || lowerTypeName == "directed_acyclic_graph") {
        return Create(DataStructureType::DAG);
    } else {
        throw std::invalid_argument("Unknown data structure type: " + typeName);
    }
}

std::vector<std::string> DataStructureFactory::GetAvailableTypes() {
    return {
        "binary_search_tree",
        "vector",
        "dag"
    };
}
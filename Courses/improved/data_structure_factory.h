//============================================================================
// Name        : data_structure_factory.h
// Author      : Keyvan Gharehtapeh
// Version     : 1.0
// Description : Factory for creating different data structure implementations
//============================================================================

#ifndef DATA_STRUCTURE_FACTORY_H
#define DATA_STRUCTURE_FACTORY_H

#include "data_structure_interface.h"
#include <memory>
#include <string>

enum class DataStructureType {
    BINARY_SEARCH_TREE,
    VECTOR,
    DAG,
    // Future implementations could include:
    // HASH_TABLE,
    // AVL_TREE,
    // RED_BLACK_TREE
};

/**
 * Factory class for creating different data structure implementations
 * Allows easy swapping between different data structure types
 */
class DataStructureFactory {
public:
    /**
     * Create a data structure of the specified type
     * @param type The type of data structure to create
     * @return Unique pointer to the data structure implementation
     */
    static std::unique_ptr<IDataStructure> Create(DataStructureType type);

    /**
     * Create a data structure from string name (useful for configuration)
     * @param typeName String name of the data structure type
     * @return Unique pointer to the data structure implementation
     */
    static std::unique_ptr<IDataStructure> Create(const std::string& typeName);

    /**
     * Get available data structure types
     * @return Vector of available type names
     */
    static std::vector<std::string> GetAvailableTypes();
};

#endif // DATA_STRUCTURE_FACTORY_H
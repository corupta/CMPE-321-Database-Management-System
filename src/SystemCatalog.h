//
// Created by CorupTa on 2019-04-08.
//

#ifndef CMPE_321_DBMS_SYSTEMCATALOG_H
#define CMPE_321_DBMS_SYSTEMCATALOG_H

#include <fstream>
#include <map>
#include <string>
#include <cstdio>
#include <vector>
#include "Scheme.h"

/**
 * A class handling DDL operations.
 * Opens / Creates system catalog file and manages it
 */
class SystemCatalog {
    /**
     * file stream of the system catalog file where the type definitions exist
     */
    std::fstream catalogFile;

    /**
     * number of types (tables) in the catalog file
     * (header of catalog file)
     */
    unsigned int tableCount;

    /**
     * a mapping of type names to schemes of types (type definition instances)
     * Even if a type is deleted, it will still be present in that map and the system catalog
     * So, when creating a file checking if it already exists as a deleted type is mandatory
     */
    std::map<std::string, Scheme*> schemes;

public:
    /**
     * constructor class that opens / creates the catalog file,
     * reads all the type definition and creates respective schemes,
     * eventually filling up the schemes map
     */
    SystemCatalog();

    /**
     * given the name and the names of the fields of a new type, creates the file
     * checks if the type exists in the schemes, if it does and is deleted replaces it
     * otherwise creates a new type at the end of the system catalog.
     *
     * returns true on success
     * and false on failure (when there's a not deleted type with the same name)
     *
     * @param name
     * @param fields
     * @return bool
     */
    bool createType(std::string name, std::vector<std::string> fields);

    /**
     * given the name of a type, deletes the type and the respective table & index files.
     *
     * returns true on success
     * and false on failure (when such type does not exist or is already deleted)
     *
     * @param name
     * @return bool
     */
    bool deleteType(std::string name);

    /**
     * gets all the types (not deleted) (type name only) in the system,
     * sorts them according to their names and returns them.
     * @return vector<string>
     */
    std::vector<std::string> listType();

    /**
     * find and return the scheme of the type with the given type name
     *
     * if the scheme cannot be found, logs an error to the error stream (cerr)
     * and returns nullptr
     *
     * otherwise returns a pointer to the respective Scheme object
     *
     * @param name
     * @return Scheme*
     */
    Scheme* getScheme(std::string name);

    /**
     * delete all the Scheme instances in the schemes map, and clear the map.
     */
    ~SystemCatalog();
};


#endif //CMPE_321_DBMS_SYSTEMCATALOG_H

//
// Created by CorupTa on 2019-04-08.
//

#ifndef CMPE_321_DBMS_SCHEME_H
#define CMPE_321_DBMS_SCHEME_H

#include <vector>
#include <fstream>
#include <iostream>
#include "Helpers.h"

/**
 * Definitions of a type (all the info stored in system catalog for a specific type)
 */
class Scheme {
    /**
     * size of each record of this scheme
     */
    int recordSize;

public:
    /**
     * beginning position of this scheme in the system catalog
     */
    unsigned long long catalogPosition;

    /**
     * whether this type is deleted or not
     * (indicated by the MSB of the first byte of the type name in the catalog file)
     */
    bool deleted;

    /**
     * name of the type
     */
    std::string name;

    /**
     * names of each field
     */
    std::vector<std::string> fields;

    /**
     * Used when creating a new type when publicly called, do not forget to call write afterwards.
     * In order to read & construct an existing type's scheme, prefer the below factory method, read.
     * @param name
     * @param fields
     * @param catalogPosition
     */
    Scheme(std::string name, std::vector<std::string> fields, unsigned long long catalogPosition);

    /**
     * A factory method that reads a scheme information from the given system catalog file stream,
     * constructs a scheme with the read info (using the above constructor but not for creating a new type,
     * instead for constructing an existing type's scheme, so it doesn't call write in the end)
     *
     * input stream's read pointer must be at the beginning of a scheme.
     * it will be moved by the length of one scheme definition afterwards.
     * @param inp
     * @return
     */
    static Scheme* read(std::fstream& inp);

    /**
     * Write the current status of the scheme into the given system catalog file stream
     *
     * output stream's pointers can be in any position, the scheme knows it's position in the catalog.
     * output stream's write pointer will be moved to the end of the current scheme's
     * (begging of the next one) position in the system catalog file.
     * @param out
     */
    void write(std::fstream& out);

    /**
     * Return the size of each record of this type (size of one record)
     * @return
     */
    int getRecordSize();

    /**
     * Size of a scheme definition in the system catalog, currently decided as 64 bytes.
     * 9 bytes type name + 1 byte fieldCount + (up to 6 fields * 9 field name = 54 bytes)
     */
    static int binarySize;
};


#endif //CMPE_321_DBMS_SCHEME_H

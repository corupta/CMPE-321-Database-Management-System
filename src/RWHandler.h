//
// Created by CorupTa on 2019-04-08.
//

#ifndef CMPE_321_DBMS_RWHANDLER_H
#define CMPE_321_DBMS_RWHANDLER_H

#include "Scheme.h"
#include "Helpers.h"
#include <string>
#include <fstream>

/**
 * A class handling DML operations of a specific type including reading & writing data to that
 * type's table file and index file.
 */
class RWHandler {
    /**
     * file stream of the table file where the records of a type exist
     */
    std::fstream dataFile;
    /**
     * file stream of the index file where the indexes for the records of a type exist
     */
    std::fstream indexFile;
    /**
     * the type definition (scheme) of the type of this class instance
     */
    Scheme* scheme;
    /**
     * page count in the table file (dataFile)
     * (header of data file)
     */
    unsigned int pageCount;
    /**
     * index count in the index file (indexFile)
     * (header of index file)
     */
    unsigned int indexCount;

    /**
     * search primaryKey first in indexes to obtain the page number
     * then read that page from the data file, find the position of the record with the
     * given primaryKey inside that page, move both the read and write pointers of the dataFile stream
     * to the beginning of the record with the primaryKey
     *
     * on success returns true,
     * on failure (index not found) return false.
     *
     * Note that this function works regardless of whether the record is deleted.
     * (You may want to check whether the pointed record is deleted after using this function)
     * @param primaryKey
     * @return bool
     */
    bool moveToRecord(const long long& primaryKey);
    /**
     * try to create an empty record in the last page of the dataFile,
     * if the last page is full, create a new page and an empty record
     *
     * create a new index for that record in the end of the index file
     *
     * move the write pointer of the dataFile stream to the beginning of the new record
     *
     * This method handles updating pageCount, nofRecordsInPage, indexCount,
     * creating index,
     * creating an empty record at the end.
     * however it does not check whether this record existed previously
     * (use this method only after when moveToRecord returns false.
     *
     * also, it does not fill the created record afterwards, but instead moves the
     * write pointer of dataFile stream to the beginning of the record,
     * so you should fill it after calling this method
     *
     * @param primaryKey
     * @return
     */
    bool initNextRecord(const long long& primaryKey);
    /**
     * comparator function used to sort records by their primary key (first field's value)
     * returns true if the first record's primary key is smaller
     * @param a
     * @param b
     * @return bool
     */
    static bool compareRecord(std::vector<long long> a, std::vector<long long> b);
public:
    /**
     * find a record matching the primary key and return its fields
     * returns empty vector if the matching fails or the record is deleted
     * @param primaryKey
     * @return vector<long long>
     */
    std::vector<long long> findRecord(const long long& primaryKey);
    /**
     * given a record's fields, find a record matching the first field,
     * and update the rest of the fields of that record according to the given data.
     * returns true on success and false if the record does not exist or is deleted
     * @param fields
     * @return bool
     */
    bool updateRecord(std::vector<long long> fields);
    /**
     * given a record's primary key, delete the record (set deleted flag to true)
     * returns true on success and false if the record does not exist or is deleted
     * @param primaryKey
     * @return bool
     */
    bool deleteRecord(const long long& primaryKey);
    /**
     * given a list of fields, creates that record
     * @param fields
     * @return true
     */
    bool createRecord(std::vector<long long> fields);
    /**
     * find all records (not deleted ones) of the current type,
     * sort them according to their primary key and
     * return all fields of those records
     * @return vector<vector<long long> >
     */
    std::vector<std::vector<long long> > listRecord();
    /**
     * constructor that opens / creates dataFile and indexFile streams for the respective
     * scheme and initializes the header data (pageCount/indexCount)
     * @param scheme
     */
    explicit RWHandler(Scheme* scheme);
    /**
     * destructor function that closes dataFile and indexFile streams
     */
    ~RWHandler();
};


#endif //CMPE_321_DBMS_RWHANDLER_H

//
// Created by CorupTa on 2019-04-08.
//

#ifndef CMPE_321_DBMS_SYSTEM_H
#define CMPE_321_DBMS_SYSTEM_H

#include "SystemCatalog.h"
#include "RWHandler.h"

/**
 * A general system class, when given an input & output stream, handles queries
 * and makes necessary calls to other system elements.
 *
 * A singleton facade class in a design pattern view.
 */

class System {
    /**
     * singleton instance of the System Class
     */
    static System* singletonInstance;

    /**
     * system catalog of the DBMS
     */
    SystemCatalog *catalog;

    /**
     * constructor function that opens/creates the system catalog
     */
    System();


    /* ====== Below operations are each a query method ===== */

    /* ====== DDL OPERATION METHODS ======================== */
    /**
     * reads [typeName] [#ofFields] [firstFieldName] ... [lastFieldName]
     * from the input stream and creates such type.
     * @param input
     * @param output
     */
    void createType(std::istream& input, std::ostream& output);

    /**
     * reads [typeName] from the input stream and removes that type
     * @param input
     * @param output
     */
    void deleteType(std::istream& input, std::ostream& output);

    /**
     * lists all existing types in the system onto the output stream sorted by their name.
     * @param input
     * @param output
     */
    void listType(std::istream& input, std::ostream& output);

    /**
     * DML HELPER METHOD not a query method
     * each DML operation's third parameter is the type name
     * reads type name and returns the respective scheme
     * each DML operation method calls this method first.
     * reads [typeName] returns scheme
     * @param input
     * @param output
     * @return
     */
    Scheme* getTypeScheme(std::istream& input, std::ostream& output);

    // DML operations
    /**
     * reads [firstFieldValue] [secondFieldValue] ... [lastFieldValue]
     * from the input stream according to the related scheme,
     * creates a new record with the read data
     * @param input
     * @param output
     */
    void createRecord(std::istream& input, std::ostream& output);

    /**
     * reads [firstFieldValue] from the input stream
     * finds the record where the firstFieldValue matches and removes it
     * @param input
     * @param output
     */
    void deleteRecord(std::istream& input, std::ostream& output);

    /**
     * reads [firstFieldValue] [secondFieldValue] ... [lastFieldValue]
     * from the input stream according to the related scheme,
     * finds the record where the firstFieldValue matches,
     * replaces its other field values with the read data
     * @param input
     * @param output
     */
    void updateRecord(std::istream& input, std::ostream& output);

    /**
     * reads [firstFieldValue] from the input stream, finds the record
     * where the firstFieldValue matches and prints its fields' values onto the output stream.
     * @param input
     * @param output
     */
    void searchRecord(std::istream& input, std::ostream& output);

    /**
     * finds all records of a type, prints all the fields of those records onto the output stream,
     * sorted by their firstFieldValues.
     * @param input
     * @param output
     */
    void listRecord(std::istream& input, std::ostream& output);

public:
    /**
     * return singleton instance of the System
     * create if it does not exist
     * @return System*
     */
    static System* getInstance();

    /**
     * destructor method that destroys the system catalog instance inside.
     */
    ~System();

    /**
     * infinite running query system, reads the next operation from the input and runs the
     * respective DDL/DML query methods.
     *
     * As a bonus, accepts a "quit" query which quits the program.
     * @param input
     * @param output
     */
    void run(std::istream& input, std::ostream& output);
};


#endif //CMPE_321_DBMS_SYSTEM_H

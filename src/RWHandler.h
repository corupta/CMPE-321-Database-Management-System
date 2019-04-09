//
// Created by CorupTa on 2019-04-08.
//

#ifndef CMPE_321_DBMS_RWHANDLER_H
#define CMPE_321_DBMS_RWHANDLER_H

#include "Scheme.h"
#include <string>
#include <fstream>

class RWHandler {
    std::fstream dataFile;
    std::fstream indexFile;
    Scheme* scheme;
    unsigned int pageCount;
    unsigned int indexCount;

    bool moveToRecord(const long long& primaryKey);
    bool initNextRecord(const long long& primaryKey);
    static bool compareRecord(std::vector<long long> a, std::vector<long long> b);
public:
    std::vector<long long> findRecord(const long long& primaryKey);
    bool updateRecord(std::vector<long long> fields);
    bool deleteRecord(const long long& primaryKey);
    bool createRecord(std::vector<long long> fields);
    std::vector<std::vector<long long> > listRecord();
    RWHandler(Scheme* scheme);
    ~RWHandler();
    static void openFile(std::fstream &stream, std::string path);
};


#endif //CMPE_321_DBMS_RWHANDLER_H

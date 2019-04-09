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

class SystemCatalog {
    unsigned int tableCount;
    std::fstream catalogFile;
    std::map<std::string, Scheme*> schemes;
public:
    SystemCatalog();
    bool createType(std::string name, std::vector<std::string> fields);
    bool deleteType(std::string name);
    std::vector<std::string> listType();
    Scheme* getScheme(std::string name);
    static bool compareType(std::string a, std::string b);
    ~SystemCatalog();

};


#endif //CMPE_321_DBMS_SYSTEMCATALOG_H

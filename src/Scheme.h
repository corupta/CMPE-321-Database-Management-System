//
// Created by CorupTa on 2019-04-08.
//

#ifndef CMPE_321_DBMS_SCHEME_H
#define CMPE_321_DBMS_SCHEME_H

#include <vector>
#include <fstream>
#include <iostream>

class Scheme {
    int recordSize;
public:
    unsigned long long catalogPosition;
    char status; // 0 means deleted
    std::string name;
    std::vector<std::string> fields;
    explicit Scheme(char schemeDefinition[64], unsigned long long catalogPosition);
    Scheme(std::string name, std::vector<std::string> fields, unsigned long long catalogPosition);
    static Scheme* read(std::fstream& inp);
    void write(std::fstream& out);
    int getRecordSize();
    static int binarySize;
};


#endif //CMPE_321_DBMS_SCHEME_H

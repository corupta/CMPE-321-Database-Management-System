//
// Created by CorupTa on 2019-04-08.
//

#ifndef CMPE_321_DBMS_SYSTEM_H
#define CMPE_321_DBMS_SYSTEM_H

#include "SystemCatalog.h"
#include "RWHandler.h"

class System {
    SystemCatalog *catalog;
    System();
    static System* singletonInstance;

    void createType(std::istream& input, std::ostream& output);
    void deleteType(std::istream& input, std::ostream& output);
    void listType(std::istream& input, std::ostream& output);
    void createRecord(std::istream& input, std::ostream& output);
    void deleteRecord(std::istream& input, std::ostream& output);
    void updateRecord(std::istream& input, std::ostream& output);
    void searchRecord(std::istream& input, std::ostream& output);
    void listRecord(std::istream& input, std::ostream& output);
public:
    static System* getInstance();
    ~System();

    void run(std::istream& input, std::ostream& output);
};


#endif //CMPE_321_DBMS_SYSTEM_H

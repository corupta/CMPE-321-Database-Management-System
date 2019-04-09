//
// Created by CorupTa on 2019-04-08.
//

#include "System.h"
System::System() {
    this->catalog = new SystemCatalog();
}

System* System::singletonInstance = nullptr;

System* System::getInstance() {
    if (singletonInstance == nullptr) {
        singletonInstance = new System();
    }

    return singletonInstance;
}


// =========== DDL OPERATIONS ==========
void System::createType(std::istream &input, std::ostream &output) {
    std::string typeName, fieldName;
    int fieldCount;
    std::vector<std::string> fields;
    input >> typeName >> fieldCount;
    for (int i = 0; i < fieldCount; ++i) {
        input >> fieldName;
        fields.push_back(fieldName);
    }
    catalog->createType(typeName, fields);
}
void System::deleteType(std::istream &input, std::ostream &output) {
    std::string typeName;
    input >> typeName;
    catalog->deleteType(typeName);
}
void System::listType(std::istream &input, std::ostream &output) {
    std::vector<std::string> types = catalog->listType();
    for (auto it = types.begin(); it != types.end(); ++it) {
        output << *it << std::endl;
    }
}

// ============ DML OPERATIONS ===========
void System::createRecord(std::istream &input, std::ostream &output) {
    std::string typeName;
    std::vector<long long> fields;
    input >> typeName;
    int fieldCount = (int)catalog->getScheme(typeName)->fields.size();
    long long fieldValue;
    for (int i = 0; i < fieldCount; ++i) {
        input >> fieldValue;
        fields.push_back(fieldValue);
    }
    RWHandler handler(catalog->getScheme(typeName));
    handler.createRecord(fields);
}
void System::deleteRecord(std::istream &input, std::ostream &output) {
    std::string typeName;
    long long primaryKey;
    input >> typeName >> primaryKey;
    RWHandler handler(catalog->getScheme(typeName));
    handler.deleteRecord(primaryKey);
}
void System::updateRecord(std::istream &input, std::ostream &output) {
    std::string typeName;
    std::vector<long long> fields;
    input >> typeName;
    int fieldCount = (int)catalog->getScheme(typeName)->fields.size();
    long long fieldValue;
    for (int i = 0; i < fieldCount; ++i) {
        input >> fieldValue;
        fields.push_back(fieldValue);
    }
    RWHandler handler(catalog->getScheme(typeName));
    handler.updateRecord(fields);
}
void System::searchRecord(std::istream &input, std::ostream &output) {
    std::string typeName;
    long long primaryKey;
    input >> typeName >> primaryKey;
    RWHandler handler(catalog->getScheme(typeName));
    std::vector<long long> fields = handler.findRecord(primaryKey);
}
void System::listRecord(std::istream &input, std::ostream &output) {
    std::string typeName;
    input >> typeName;
    RWHandler handler(catalog->getScheme(typeName));
    std::vector<std::vector<long long> > records = handler.listRecord();
    for (auto record = records.begin(); record != records.end(); ++record) {
        for (auto field = record->begin(); field != record->end(); ++field) {
            output << *field << " ";
        }
        output << std::endl;
    }
}


void System::run(std::istream &input, std::ostream &output) {
    std::string operation, scope;
    input >> operation;
    while(input.good()) {
        input >> scope;
        if (scope == "type") {
            if (operation == "create") {
                this->createType(input, output);
            } else if (operation == "delete") {
                this->deleteType(input, output);
            } else if (operation == "list") {
                this->listType(input, output);
            } else {
                std::cerr << "unknown DDL operation, must be (create|delete|list)" << std::endl;
            }
        } else if (scope == "record") {
            if (operation == "create") {
                this->createRecord(input, output);
            } else if (operation == "delete") {
                this->deleteRecord(input, output);
            } else if (operation == "update") {
                this->updateRecord(input, output);
            } else if (operation == "search") {
                this->searchRecord(input, output);
            } else if (operation == "list") {
                this->listRecord(input, output);
            } else {
                std::cerr << "unknown DML operation, must be (create|delete|update|search|list)" << std::endl;
            }
        } else if (scope == "quit") {
            return;
        } else {
            std::cerr << "unknown scope, must be (type|record)" << std::endl;
        }
        // std::cerr << "Expections: " << std::strerror(errno) << std::endl;
        input >> operation;
    }
}

System::~System() {
    delete catalog;
}
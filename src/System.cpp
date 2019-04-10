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
    input >> typeName;
    input >> fieldCount;
    for (int i = 0; i < fieldCount; ++i) {
        input >> fieldName;
        fields.push_back(fieldName);
    }
#ifdef DEBUG
    if (!catalog->createType(typeName, fields)) {
        std::cerr << "Failed to create type!" << std::endl;
    }
#endif
}
void System::deleteType(std::istream &input, std::ostream &output) {
    std::string typeName;
    input >> typeName;
#ifdef DEBUG
    if (!catalog->deleteType(typeName)) {
        std::cerr << "Failed to delete type!" << std::endl;
    }
#endif
}
void System::listType(std::istream &input, std::ostream &output) {
    std::vector<std::string> types = catalog->listType();
    for (auto it = types.begin(); it != types.end(); ++it) {
        output << *it << std::endl;
    }
}

// ============ DML HELPER ===============

Scheme* System::getTypeScheme(std::istream &input, std::ostream &output) {
    std::string typeName;
    input >> typeName;
    return catalog->getScheme(typeName);
}

// ============ DML OPERATIONS ===========

void System::createRecord(std::istream &input, std::ostream &output) {
    Scheme* scheme = getTypeScheme(input, output);
    if (scheme != nullptr) {
        std::vector<long long> fields;
        int fieldCount = (int) scheme->fields.size();
        long long fieldValue;
        for (int i = 0; i < fieldCount; ++i) {
            input >> fieldValue;
            fields.push_back(fieldValue);
        }
        RWHandler handler(scheme);
#ifdef DEBUG
        if (!handler.createRecord(fields)) {
            std::cerr << "Failed to create record" << std::endl;
        }
#endif
    }
}
void System::deleteRecord(std::istream &input, std::ostream &output) {
    Scheme* scheme = getTypeScheme(input, output);
    if (scheme != nullptr) {
        long long primaryKey;
        input >> primaryKey;
        RWHandler handler(scheme);
#ifdef DEBUG
        if (!handler.deleteRecord(primaryKey)) {
            std::cerr << "Failed to delete record" << std::endl;
        }
#endif
    }
}
void System::updateRecord(std::istream &input, std::ostream &output) {
    Scheme* scheme = getTypeScheme(input, output);
    if (scheme != nullptr) {
        std::vector<long long> fields;
        int fieldCount = (int) scheme->fields.size();
        long long fieldValue;
        for (int i = 0; i < fieldCount; ++i) {
            input >> fieldValue;
            fields.push_back(fieldValue);
        }
        RWHandler handler(scheme);
#ifdef DEBUG
        if (!handler.updateRecord(fields)) {
            std::cerr << "Failed to update record" << std::endl;
        }
#endif
    }
}
void System::searchRecord(std::istream &input, std::ostream &output) {
    Scheme* scheme = getTypeScheme(input, output);
    if (scheme != nullptr) {
        long long primaryKey;
        input >> primaryKey;
        RWHandler handler(scheme);
        std::vector<long long> fields = handler.findRecord(primaryKey);
#ifdef DEBUG
        if (fields.size() == 0) {
            // std::cerr << "Failed to find record" << std::endl;
        }
#endif
        for (auto field = fields.begin(); field != fields.end(); ++field) {
            output << *field << " ";
        }
        output << std::endl;
    }
}
void System::listRecord(std::istream &input, std::ostream &output) {
    Scheme* scheme = getTypeScheme(input, output);
    if (scheme != nullptr) {
        RWHandler handler(scheme);
        std::vector<std::vector<long long> > records = handler.listRecord();
        for (auto record = records.begin(); record != records.end(); ++record) {
            for (auto field = record->begin(); field != record->end(); ++field) {
                output << *field << " ";
            }
            output << std::endl;
        }
    }
}


void System::run(std::istream &input, std::ostream &output) {
    std::string operation, scope;
    input >> operation;
    while(input.good()) {
        if (operation == "quit") {
            return;
        }
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
        } else {
            std::cerr << "unknown scope, must be (type|record)" << std::endl;
        }
#ifdef DEBUG
        // std::cerr << "Done." << std::endl;
#endif
        input >> operation;
    }
}

System::~System() {
    delete catalog;
}
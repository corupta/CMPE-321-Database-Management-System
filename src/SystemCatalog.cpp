//
// Created by CorupTa on 2019-04-08.
//

#include "SystemCatalog.h"


SystemCatalog::SystemCatalog() {
    if (Helpers::openFile(catalogFile, "SystemCatalog.txt")) {
        // NEW FILE
        tableCount = 0;
        Helpers::write(catalogFile, &tableCount, sizeof(tableCount));
    } else {
        // OLD FILE
        Helpers::read(catalogFile, &tableCount, sizeof(tableCount));
    }

    // table numbers start from 1
    for (unsigned int i = 1; i <= tableCount; ++i) {
        Scheme* scheme = Scheme::read(catalogFile);
        schemes[scheme->name] = scheme;
    }
}

bool SystemCatalog::createType(std::string name, std::vector<std::string> fields) {
    unsigned long long catalogPosition = 4 + tableCount * Scheme::binarySize;
    if (schemes[name]) {
        catalogPosition = schemes[name]->catalogPosition;
        if (!schemes[name]->deleted) {
            return false;
        }
        delete schemes[name];
    } else {
        ++tableCount;
        Helpers::seekWritePos(catalogFile, 0);
        Helpers::write(catalogFile, &tableCount, sizeof(tableCount));
    }
    schemes[name] = new Scheme(name, fields, catalogPosition);
    schemes[name]->write(catalogFile);
    return true;
}

bool SystemCatalog::deleteType(std::string name) {
    if (schemes[name]) {
        if (!schemes[name]->deleted) {
            schemes[name]->deleted = true;
            schemes[name]->write(catalogFile);
            Helpers::removeFile(name + ".table.txt");
            Helpers::removeFile(name + ".index.txt");
            return true;
        }
    }
    return false;
}

std::vector<std::string> SystemCatalog::listType() {
    std::vector<std::string> res;
    for (auto it = schemes.begin(); it != schemes.end(); ++it) {
        Scheme* scheme = it->second;
        if (!scheme->deleted) {
            res.push_back(scheme->name);
        }
    }
    std::sort(res.begin(), res.end());
    return res;
}

Scheme* SystemCatalog::getScheme(std::string name) {
    if (schemes[name]) {
        return schemes[name];
    } else {
        std::cerr << "Trying to access an undefined type's scheme" << std::endl;
        return nullptr;
    }
}

SystemCatalog::~SystemCatalog() {
    for (auto it = schemes.begin(); it != schemes.end(); ++it) {
        delete it->second;
    }
    schemes.clear();
}
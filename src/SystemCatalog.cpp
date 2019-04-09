//
// Created by CorupTa on 2019-04-08.
//

#include "SystemCatalog.h"


SystemCatalog::SystemCatalog() {
    RWHandler::openFile(catalogFile, "SystemCatalog.txt");
    std::cerr << "Read sys cat" << std::endl;
    if (!catalogFile) {
        std::cerr << "popopo" << std::endl;
    }
    catalogFile.seekg(0, catalogFile.end);
    long long length = catalogFile.tellg();
    catalogFile.seekg(0, catalogFile.beg);
    if (length < 4) {
        catalogFile.seekp(0, catalogFile.beg);
        tableCount = 0;
        catalogFile.write((char *) &tableCount, sizeof(tableCount));
        catalogFile.seekg(0, catalogFile.beg);
    }
    catalogFile.read((char *) &tableCount, sizeof(tableCount));
    if(!catalogFile) std:: cerr << "dipC" << std::endl;
    // table numbers start from 1
    for (unsigned int i = 1; i <= tableCount; ++i) {
        Scheme* scheme = Scheme::read(catalogFile);
        if(!catalogFile) std:: cerr << "dipB" << std::endl;
        schemes[scheme->name] = scheme;
    }
    if(!catalogFile) std:: cerr << "dipA" << std::endl;
}

bool SystemCatalog::createType(std::string name, std::vector<std::string> fields) {
    unsigned long long catalogPosition = 4 + tableCount * Scheme::binarySize;
    if (schemes[name]) {
        catalogPosition = schemes[name]->catalogPosition;
        delete schemes[name];
    } else {
        ++tableCount;
        catalogFile.seekp(0);
        catalogFile.write((char *)&tableCount, sizeof(tableCount));
    }
    if (!catalogFile) {
        std::cerr << "not even here?" << std::endl;
    }
    schemes[name] = new Scheme(name, fields, catalogPosition);
    schemes[name]->write(catalogFile);
    catalogFile.flush();
    return true;
}

bool SystemCatalog::deleteType(std::string name) {
    if (schemes[name]) {
        schemes[name]->status = 0;
        schemes[name]->write(catalogFile);
        std::string dataPath = "data/" + name + ".table.txt";
        std::remove(dataPath.c_str());
        std::string indexPath = "data/" + name + ".index.txt";
        std::remove(indexPath.c_str());
        catalogFile.flush();
        return true;
    }
    return false;
}

std::vector<std::string> SystemCatalog::listType() {
    std::vector<std::string> res;
    for (auto it = schemes.begin(); it != schemes.end(); ++it) {
        Scheme* scheme = it->second;
        if (scheme->status) {
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
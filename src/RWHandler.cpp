//
// Created by CorupTa on 2019-04-08.
//

#include "RWHandler.h"

RWHandler::RWHandler(Scheme *scheme): scheme(scheme) {
    RWHandler::openFile(dataFile, scheme->name + ".table.txt");
    RWHandler::openFile(indexFile, scheme->name + ".index.txt");
    if(!dataFile) std:: cerr << "dipKKKK" << dataFile.rdstate() << dataFile.good() << dataFile.bad() << dataFile.fail() << dataFile.eof() << std::endl;
    dataFile.read((char *) &pageCount, sizeof(pageCount));
    if(!dataFile) std:: cerr << "dipW" << dataFile.rdstate() << dataFile.good() << dataFile.bad() << dataFile.fail() << dataFile.eof() << std::endl;
    if (!dataFile) std::cerr << "not here either wow" << std::endl;

    if (!dataFile.good()) {
        dataFile.seekp(0);
        pageCount = 0;
        dataFile.write((char *) &pageCount, sizeof(pageCount));
        if (!dataFile) std::cerr << "not here either wow" << std::endl;

    }
    if (!dataFile) std::cerr << "not here either wow" << std::endl;

    indexFile.read((char *) &indexCount, sizeof(indexCount));
    if (!indexFile.good()) {
        indexFile.seekp(0);
        indexCount = 0;
        indexFile.write((char *) &indexCount, sizeof(indexCount));
    }

    if (!dataFile) std::cerr << "not here either wow" << std::endl;
}

bool RWHandler::moveToRecord(const long long &primaryKey) {
    long long pkey;
    unsigned int page = 0;
    this->indexFile.seekg(4);
    for (int i = 0; i < this->indexCount; ++i) {
        indexFile.read((char*) &pkey, sizeof(pkey));
        indexFile.read((char*) &page, sizeof(page));
        if (pkey == primaryKey) {
            break;
        }
    }
    std::vector<long long> res;
    if (page == 0) {
        return false;
    }
    // pages start from 1
    this->dataFile.seekg(4 + (page - 1) * 1024 + 2);
    // 1 for page header, +1 for del stat of record
    this->dataFile.seekp(4 + (page - 1) * 1024 + 1);
    // 1 for page header
    int nofRecordsPerPage = 1023 / scheme->getRecordSize();
    for (int i = 0; i < nofRecordsPerPage; ++i) {
        this->dataFile.seekg(scheme->getRecordSize(), std::ios_base::cur);
        this->dataFile.seekp(scheme->getRecordSize(), std::ios_base::cur);
        this->dataFile.read((char *) &pkey, sizeof(pkey));
        if (pkey == primaryKey) {
            this->dataFile.seekg(-sizeof(pkey) - 1, std::ios_base::cur);
            // -1 for del stat of record
            return true;
        }
    }
    return false;
}

bool RWHandler::initNextRecord(const long long &primaryKey) {
    unsigned int page = pageCount;
    int nofRecordsPerPage = 1023 / scheme->getRecordSize();
    char nofRecordsInPage;

    if (page > 0) {
        dataFile.seekg(4 + (page - 1) * 1024);
        dataFile.read(&nofRecordsInPage, sizeof(nofRecordsInPage));

        if ((int) nofRecordsInPage == nofRecordsPerPage) {
            ++page;
            nofRecordsInPage = 0;
        }
    } else {
        page = 1;
        nofRecordsInPage = 0;
    }

    ++nofRecordsInPage;
    dataFile.seekp(4 + (page - 1) * 1024);
    dataFile.write(&nofRecordsInPage, sizeof(nofRecordsInPage));
    dataFile.flush();

    ++indexCount;
    indexFile.seekp(4 + (indexCount - 1) * 12);
    indexFile.write((char *) &primaryKey, sizeof(primaryKey));
    indexFile.write((char *) &page, sizeof(page));
    indexFile.flush();
    return true;
}

bool RWHandler::compareRecord(std::vector<long long> a, std::vector<long long> b) {
    return (a[0] < b[0]);
}

std::vector<long long> RWHandler::findRecord(const long long &primaryKey) {
    std::vector<long long> res;
    if (this->moveToRecord(primaryKey)) {
        char deleted;
        dataFile.read(&deleted, sizeof(deleted));
        if (!deleted) {
            long long field;
            for (int i = 0; i < scheme->fields.size(); ++i) {
                dataFile.read((char *) &field, sizeof(field));
                res.push_back(field);
            }
        }
    }
    return res;
}

bool RWHandler::updateRecord(std::vector<long long> fields) {
    if (this->moveToRecord(fields[0])) {
        char deleted;
        dataFile.read(&deleted, sizeof(deleted));
        if (!deleted) {
            long long field;
            dataFile.seekp(9, std::ios_base::cur);
            for (int i = 1; i < scheme->fields.size(); ++i) {
                field = fields[i];
                dataFile.write((char *) &field, sizeof(field));
            }
            dataFile.flush();
            return true;
        }
    }
    return false;
}

bool RWHandler::deleteRecord(const long long &primaryKey) {
    if (this->moveToRecord(primaryKey)) {
        char deleted;
        dataFile.read(&deleted, sizeof(deleted));
        if (!deleted) {
            deleted = 1;
            dataFile.write(&deleted, sizeof(deleted));
            dataFile.flush();
            return true;
        }
    }
    return false;
}

bool RWHandler::createRecord(std::vector<long long> fields) {

    if (!dataFile) std::cerr << "not here either" << std::endl;
    if (!this->moveToRecord(fields[0])) {
        this->initNextRecord(fields[0]);
    }

    if (!dataFile) std::cerr << "not here either" << std::endl;
    char deleted = 0;
    dataFile.write(&deleted, sizeof(deleted));

    if (!dataFile) std::cerr << "not here either" << std::endl;
    long long field;
    for (int i = 0; i < fields.size(); ++i) {
        field = fields[i];
        dataFile.write((char *) &field, sizeof(field));
    }
    if (!dataFile) std::cerr << "not here either" << std::endl;

    dataFile.flush();
    return true;
}

std::vector<std::vector<long long> > RWHandler::listRecord() {
    std::vector<std::vector<long long> > res;
    for (unsigned int page = 1; page <= pageCount; ++page) {
        dataFile.seekg(4 + (page - 1) * 1024);
        // 4 for file header
        char nofRecords = 0;
        dataFile.read(&nofRecords, sizeof(nofRecords));
        // 1 byte page header
        for (int j = 0; j < nofRecords; ++j) {
            std::vector<long long> record;
            long long field;
            for (int k = 0; k < scheme->fields.size(); ++k) {
                dataFile.read((char *) &field, sizeof(field));
                record.push_back(field);
            }
            res.push_back(record);
        }
    }
    sort(res.begin(), res.end(), compareRecord);
    return res;
}

RWHandler::~RWHandler() {
    dataFile.close();
    indexFile.close();
}

void RWHandler::openFile(std::fstream &stream, std::string path) {
    stream.open(path.c_str(), std::ios::in | std::ios::binary | std::ios::out);
    while (!stream) {
        stream.open(path.c_str(), std::ios::out | std::ios::binary);
        stream.close();
        stream.open(path.c_str(), std::ios::in | std::ios::binary | std::ios::out);
    }
}
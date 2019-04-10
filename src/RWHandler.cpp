//
// Created by CorupTa on 2019-04-08.
//

#include "RWHandler.h"

RWHandler::RWHandler(Scheme *scheme): scheme(scheme) {
    if (Helpers::openFile(dataFile, scheme->name + ".table.txt")) {
        // NEW FILE
        Helpers::seekWritePos(dataFile, 0);
        pageCount = 0;
        Helpers::write(dataFile, &pageCount, sizeof(pageCount));
    } else {
        // OLD FILE
        Helpers::read(dataFile, &pageCount, sizeof(pageCount));
    }

    if (Helpers::openFile(indexFile, scheme->name + ".index.txt")) {
        // NEW FILE
        Helpers::seekWritePos(indexFile, 0);
        indexCount = 0;
        Helpers::write(indexFile, &indexCount, sizeof(indexCount));
    } else {
        // OLD FILE
        Helpers::read(indexFile, &indexCount, sizeof(indexCount));
    }
}

bool RWHandler::moveToRecord(const long long &primaryKey) {
    long long pkey;
    unsigned int page = 0;

    Helpers::seekReadPos(indexFile, 4);
    for (int i = 0; i < this->indexCount; ++i) {
        Helpers::read(indexFile, &pkey, sizeof(pkey));
        Helpers::read(indexFile, &page, sizeof(page));
        if (pkey == primaryKey) {
            break;
        }
    }
    std::vector<long long> res;
    if (page == 0) {
        return false;
    }
    // pages start from 1
    Helpers::seekReadPos(dataFile, 4 + (page - 1) * 1024);
    unsigned char nofRecordsInPage;
    Helpers::read(dataFile, &nofRecordsInPage, sizeof(nofRecordsInPage));
    Helpers::incReadPos(dataFile, 1);
    // +1 for del stat of record
    // 1 for page header
    for (int i = 0; i < nofRecordsInPage; ++i) {
        if (i > 0) {
            Helpers::incReadPos(dataFile, scheme->getRecordSize());
        }
        Helpers::read(dataFile, &pkey, sizeof(pkey));
        Helpers::incReadPos(dataFile, -sizeof(pkey));
        if (pkey == primaryKey) {
            Helpers::incReadPos(dataFile, -1);
            // -1 for del stat of record
            return true;
        }
    }
    return false;
}

bool RWHandler::initNextRecord(const long long &primaryKey) {
    int nofRecordsPerPage = 1023 / scheme->getRecordSize();
    char nofRecordsInPage = 0; // 0 only to prevent compiler warning
    // although, either pageCount > 0 or == 0 must be true since pageCount is unsigned
    // so, nofRecordsInPage will always be overwritten.

    if (pageCount > 0) {
        Helpers::seekReadPos(dataFile, 4 + (pageCount - 1) * 1024);
        Helpers::read(dataFile, &nofRecordsInPage, sizeof(nofRecordsInPage));
    }
    if (pageCount == 0 || nofRecordsInPage == (char)nofRecordsPerPage) {
        ++pageCount;
        nofRecordsInPage = 0;
        Helpers::seekWritePos(dataFile, 0);
        Helpers::write(dataFile, &pageCount, sizeof(pageCount));
    }

    ++nofRecordsInPage;
    Helpers::seekWritePos(dataFile, 4 + (pageCount - 1) * 1024);
    Helpers::write(dataFile, &nofRecordsInPage, sizeof(nofRecordsInPage));

    Helpers::incWritePos(dataFile, (nofRecordsInPage - 1) * scheme->getRecordSize());

    ++indexCount;
    Helpers::seekWritePos(indexFile, 0);
    Helpers::write(indexFile, &indexCount, sizeof(indexCount));
    Helpers::seekWritePos(indexFile, 4 + (indexCount - 1) * 12);
    Helpers::write(indexFile, &primaryKey, sizeof(primaryKey));
    Helpers::write(indexFile, &pageCount, sizeof(pageCount));
    return true;
}

bool RWHandler::compareRecord(std::vector<long long> a, std::vector<long long> b) {
    return (a[0] < b[0]);
}

std::vector<long long> RWHandler::findRecord(const long long &primaryKey) {
    std::vector<long long> res;
    if (this->moveToRecord(primaryKey)) {
        char deleted;
        Helpers::read(dataFile, &deleted, sizeof(deleted));
        if (!deleted) {
            long long field;
            for (int i = 0; i < scheme->fields.size(); ++i) {
                Helpers::read(dataFile, &field, sizeof(field));
                res.push_back(field);
            }
        }
    }
    return res;
}

bool RWHandler::updateRecord(std::vector<long long> fields) {
    if (this->moveToRecord(fields[0])) {
        char deleted;
        Helpers::read(dataFile, &deleted, sizeof(deleted));
        if (!deleted) {
            long long field;
            Helpers::incWritePos(dataFile, 8);
            // skip 8 for primary key
            for (int i = 1; i < scheme->fields.size(); ++i) {
                field = fields[i];
                Helpers::write(dataFile, &field, sizeof(field));
            }
            return true;
        }
    }
    return false;
}

bool RWHandler::deleteRecord(const long long &primaryKey) {
    if (this->moveToRecord(primaryKey)) {
        char deleted;
        Helpers::read(dataFile, &deleted, sizeof(deleted));
        if (!deleted) {
            deleted = 1;
            Helpers::incWritePos(dataFile, -sizeof(deleted));
            // go back to deleted
            Helpers::write(dataFile, &deleted, sizeof(deleted));
            return true;
        }
    }
    return false;
}

bool RWHandler::createRecord(std::vector<long long> fields) {

    if (!this->moveToRecord(fields[0])) {
        this->initNextRecord(fields[0]);
    }

    char deleted = 0;
    Helpers::write(dataFile, &deleted, sizeof(deleted));

    long long field;
    for (int i = 0; i < fields.size(); ++i) {
        field = fields[i];
        Helpers::write(dataFile, &field, sizeof(field));
    }

    return true;
}

std::vector<std::vector<long long> > RWHandler::listRecord() {
    std::vector<std::vector<long long> > res;
    for (unsigned int page = 1; page <= pageCount; ++page) {
        Helpers::seekReadPos(dataFile, 4 + (page - 1) * 1024);
        // 4 for file header
        char nofRecords = 0;
        Helpers::read(dataFile, &nofRecords, sizeof(nofRecords));
        // 1 byte page header
        for (int j = 0; j < nofRecords; ++j) {
            char deleted;
            Helpers::read(dataFile, &deleted, sizeof(deleted));
            if (!deleted) {
                std::vector<long long> record;
                long long field;
                for (int k = 0; k < scheme->fields.size(); ++k) {
                    Helpers::read(dataFile, &field, sizeof(field));
                    record.push_back(field);
                }
                res.push_back(record);
            }
        }
    }
    sort(res.begin(), res.end(), compareRecord);
    return res;
}

RWHandler::~RWHandler() {
    Helpers::close(dataFile);
    Helpers::close(indexFile);
}
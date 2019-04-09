//
// Created by CorupTa on 2019-04-08.
//

#include "Scheme.h"


Scheme::Scheme(std::string name, std::vector<std::string> fields, unsigned long long catalogPosition) :
fields(fields), catalogPosition(catalogPosition), deleted(false) {
    if (name[0] & (1 << 7)) {
        // this means the scheme is deleted
        deleted = true;
        name[0] &= 0x7F;
    }
    this->name = name;
    recordSize = 1 + (int)fields.size() * 8;
}

Scheme* Scheme::read(std::fstream &inp) {
    unsigned long long catalogPosition = Helpers::tellReadPos(inp);

    std::vector<std::string> fields;
    std::string name;

    char buffer[9], fieldCount;

    Helpers::read(inp, buffer, sizeof(buffer));
    name = Helpers::readString(buffer, 9);
    Helpers::read(inp, &fieldCount, sizeof(fieldCount));
    for (int i = 0; i < fieldCount; ++i) {
        Helpers::read(inp, buffer, sizeof(buffer));
        fields.push_back(Helpers::readString(buffer, 9));
    }
    return new Scheme(name, fields, catalogPosition);
}

void Scheme::write(std::fstream &out) {
    char schemeDefinition[64];
    memset(schemeDefinition, 0, sizeof(schemeDefinition));
    for (int i = 0; i < this -> name.length(); ++i) {
        schemeDefinition[i] = name[i];
    }
    if (deleted) {
        schemeDefinition[0] |= (1<<7);
    }
    schemeDefinition[9] = (char)this->fields.size();
    for (int i = 0; i < this-> fields.size(); ++i) {
        for (int j = 0; j < this->fields[i].length(); ++j) {
            schemeDefinition[9*i + 10 + j] = this->fields[i][j];
        }
    }
    Helpers::seekWritePos(out, catalogPosition);
    Helpers::write(out, schemeDefinition, sizeof(schemeDefinition));
}

int Scheme::getRecordSize() {
    return this->recordSize;
}

int Scheme::binarySize = 64;



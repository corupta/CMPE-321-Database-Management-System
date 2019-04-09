//
// Created by CorupTa on 2019-04-08.
//

#include "Scheme.h"


Scheme::Scheme(char schemeDefinition[64], unsigned long long catalogPosition) : status(schemeDefinition[0])
, name(schemeDefinition), recordSize(1), catalogPosition(catalogPosition) {
    int fieldCount = (int) schemeDefinition[9];
    for (int i = 0; i < fieldCount; ++i) {
        this -> fields.push_back(schemeDefinition + (9*i + 10));
        this -> recordSize += 8;
    }
}

Scheme::Scheme(std::string name, std::vector<std::string> fields, unsigned long long catalogPosition) :
status(name[0]), name(name), fields(fields), catalogPosition(catalogPosition) {
    status = name[0];
    recordSize = 1 + (int)fields.size() * 8;
}

Scheme* Scheme::read(std::fstream &inp) {
    char schemeDefinition[64];

    if(!inp) std:: cerr << "dipZ" << std::endl;
    unsigned long long catalogPosition = inp.tellg();

    if(!inp) std:: cerr << "dipQ" << std::endl;
    inp.read((char *) &schemeDefinition[0], sizeof(schemeDefinition));


    if(!inp) std:: cerr << "dipW" << inp.rdstate() << inp.good() << inp.bad() << inp.fail() << inp.eof() << std::endl;
    return new Scheme(schemeDefinition, catalogPosition);
}

void Scheme::write(std::fstream &out) {
    char schemeDefinition[64];
    memset(schemeDefinition, 0, sizeof(schemeDefinition));
    for (int i = 0; i < this -> name.length(); ++i) {
        schemeDefinition[i] = name[i];
    }
    if (this->status == 0) {
        schemeDefinition[0] = 0;
    }
    schemeDefinition[9] = (char)this->fields.size();
    for (int i = 0; i < this-> fields.size(); ++i) {
        for (int j = 0; j < this->fields[i].length(); ++j) {
            schemeDefinition[9*i + 10 + j] = this->fields[i][j];
        }
    }
    if (!out) {
        std::cerr << "out not set??" << std::endl;
    }
    out.seekp(catalogPosition, std::ios::beg);
    out.write(&schemeDefinition[0], sizeof(schemeDefinition));
    if (!out) {
        std::cerr << "out is broken" << std::endl;
    }
}

int Scheme::getRecordSize() {
    return this->recordSize;
}

int Scheme::binarySize = 64;



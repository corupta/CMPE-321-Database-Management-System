//
// Created by CorupTa on 2019-04-09.
//

#ifndef CMPE_321_DBMS_HELPERS_H
#define CMPE_321_DBMS_HELPERS_H

#include <string>
#include <fstream>
#include <iostream>

#define DEBUG

class Helpers {
    static std::ostream& logStreamStatus(std::fstream& stream, std::ostream& logStream = std::cout, bool endline = true);
    static std::ostream& logBytes(char* ptr, unsigned int length, std::ostream& logStream = std::cout, bool endline = true);
public:
    static std::ostream& logError(std::string operation, std::ostream& errorStream = std::cerr, bool endline = false);
    // read string but at most maxLength (string may be padded with 0 in the end)
    static std::string readString(char* stringPointer, unsigned int maxLength);
    // return true if opened file is new, false if it existed
    static bool openFile(std::fstream &stream, std::string path);
    // absolute position (relative to begin)
    static void seekReadPos(std::fstream &stream, unsigned long long position);
    static void seekWritePos(std::fstream &stream, unsigned long long position);
    // relative position (relative to current cursor)
    static void incReadPos(std::fstream &stream, long long offset);
    static void incWritePos(std::fstream &stream, long long offset);
    static unsigned long long tellReadPos(std::fstream &stream);
    static unsigned long long tellWritePos(std::fstream &stream);
    static void read(std::fstream &stream, const void* ptr, unsigned int length);
    static void write(std::fstream &stream, const void* ptr, unsigned int length);
    static void close(std::fstream &stream);
    static void removeFile(std::string filePath);
};


#endif //CMPE_321_DBMS_HELPERS_H

//
// Created by CorupTa on 2019-04-09.
//

#include "Helpers.h"

using namespace std;

ostream& Helpers::logError(string operation, ostream& errorStream, bool endline) {
    errorStream << "Error in " << operation << "! Details: ";
    if (endline) errorStream << endl;
    return errorStream;
}

ostream& Helpers::logStreamStatus(fstream& stream, ostream& logStream, bool endline) {
    logStream << " Stream{ Good: " << stream.good() << ", Bad: " << stream.bad()
    << ", Fail: " << stream.fail() << ", Eof: " << stream.eof() << " }";
    if (endline) logStream << endl;
    return logStream;
}

std::ostream& Helpers::logBytes(char *ptr, unsigned int length, std::ostream &logStream, bool endline) {
    logStream << "Bytes (" << length << ") = ";
    for (unsigned int i = 0; i < length; ++i) {
        logStream << "(" << i << ":" << (unsigned int)ptr[i] << ":" << ptr[i] << ") ";
    }
    if (endline) logStream << endl;
    return logStream;
}

bool Helpers::openFile(fstream &stream, string path) {
    bool isNew = false;
    stream.open(path.c_str(), ios::in | ios::binary | ios::out);
    if (!stream) {
        isNew = true;
        stream.open(path.c_str(), ios::out | ios::binary);
        stream.close();
        stream.open(path.c_str(), ios::in | ios::binary | ios::out);
    }
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("openFile") << path << " isNew: " << isNew) << endl;
    }
#endif
    return isNew;
}

string Helpers::readString(char *stringPointer, unsigned int maxLength) {
    unsigned int length = 0;
    while(length < maxLength && stringPointer[length]) ++length;
    return string(stringPointer, length);
}

void Helpers::seekReadPos(std::fstream &stream, unsigned long long position) {
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("seekReadPos") << "before seek");
    }
#endif DEBUG
    stream.seekg(position, ios::beg);
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("seekReadPos") << position);
    }
#endif
}

void Helpers::seekWritePos(std::fstream &stream, unsigned long long position) {
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("seekWritePos") << "before seek");
    }
#endif
    stream.seekp(position, ios::beg);
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("seekWritePos") << position);
    }
#endif
}

void Helpers::incReadPos(std::fstream &stream, long long offset) {
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("incReadPos") << "before inc");
    }
#endif
    stream.seekg(offset, ios::cur);
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("incReadPos") << offset);
    }
#endif
}

void Helpers::incWritePos(std::fstream &stream, long long offset) {
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("incWritePos") << "before inc");
    }
#endif
    stream.seekg(offset, ios::cur);
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("incWritePos") << offset);
    }
#endif
}

unsigned long long Helpers::tellReadPos(std::fstream &stream) {
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("tellReadPos") << "before tell");
    }
#endif
    long long position = stream.tellg();
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("tellReadPos") << position);
    }
    if (position < 0) {
        logError("tellReadPos") << "position < 0 " << position << endl;
    }
#endif
    return (unsigned long long)position;
}

unsigned long long Helpers::tellWritePos(std::fstream &stream) {
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("tellWritePos") << "before tell");
    }
#endif
    long long position = stream.tellp();
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("tellWritePos") << position);
    }
    if (position < 0) {
        logError("tellWritePos") << "position < 0 " << position << endl;
    }
#endif
    return (unsigned long long)position;
}

void Helpers::read(std::fstream &stream, const void *ptr, unsigned int length) {
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("read") << "before read");
    }
#endif
    stream.read((char*) ptr, length);
#ifdef DEBUG
    if (!stream) {
        logBytes((char*) ptr, length, logStreamStatus(stream, logError("read")) << "Read ");
    }
#endif
}

void Helpers::write(std::fstream &stream, const void *ptr, unsigned int length) {
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("write") << "before write");
    }
#endif
    stream.write((char*) ptr, length);
#ifdef DEBUG
    if (!stream) {
        logBytes((char*) ptr, length, logStreamStatus(stream, logError("write")) << "Writing ");
    }
#endif
    stream.flush();
#ifdef DEBUG
    if (!stream) {
        logBytes((char*) ptr, length, logStreamStatus(stream, logError("write")) << "Flushing ");
    }
#endif
}

void Helpers::close(std::fstream &stream) {
#ifdef DEBUG
    if (!stream) {
        logStreamStatus(stream, logError("close") << "before close");
    }
#endif
    stream.close();
}

void Helpers::removeFile(std::string filePath) {
    int error = remove(filePath.c_str());
#ifdef DEBUG
    if (error) {
        logError("removeFile") << filePath << " ErrorCode: " << error << endl;
    }
#endif
}
//
// Created by CorupTa on 2019-04-09.
//

#ifndef CMPE_321_DBMS_HELPERS_H
#define CMPE_321_DBMS_HELPERS_H

#include <string>
#include <fstream>
#include <iostream>

// #define DEBUG

/**
 * Helper methods regarding file operations and debugging
 */
class Helpers {
    /**
     * Log The current status of a file stream (for debug purposes)
     * @param stream
     * @param logStream
     * @param endline
     * @return logStream
     */
    static std::ostream& logStreamStatus(std::fstream& stream, std::ostream& logStream = std::cout, bool endline = true);

    /**
     * Log the bytes of a given binary buffer (for debug purposes)
     * @param ptr
     * @param length
     * @param logStream
     * @param endline
     * @return logStream
     */
    static std::ostream& logBytes(char* ptr, unsigned int length, std::ostream& logStream = std::cout, bool endline = true);

public:
    /**
     * Log that an error happened when running the given operation (for debug purposes)
     * @param operation
     * @param errorStream
     * @param endline
     * @return errorStream
     */
    static std::ostream& logError(std::string operation, std::ostream& errorStream = std::cerr, bool endline = false);

    /**
     * Read a c-style (0 padded) string or a string at most maxLength from the given char pointer
     * (String data may not be 0 padded if all bytes are occupied)
     * @param stringPointer
     * @param maxLength
     * @return std::string
     */
    static std::string readString(char* stringPointer, unsigned int maxLength);

    /**
     * Open a file stream with the given path in (in|out|binary) mode
     * Create the file if it does not exist, but do not truncate or delete if it exists
     * Return true if the file is newly created, false otherwise (it already exists)
     * @param stream
     * @param path
     * @return bool
     */
    static bool openFile(std::fstream &stream, std::string path);

    /**
     * Move given file stream's read pointer to the given absolute position (relative to beginning)
     * @param stream
     * @param position
     */
    static void seekReadPos(std::fstream &stream, unsigned long long position);

    /**
     * Move given file stream's write pointer to the given absolute position (relative to beginning)
     * @param stream
     * @param position
     */
    static void seekWritePos(std::fstream &stream, unsigned long long position);

    /**
     * Move given file stream's read pointer by given offset relative to the current position
     * @param stream
     * @param offset
     */
    static void incReadPos(std::fstream &stream, long long offset);

    /**
     * Move given file stream's write pointer by given offset relative to the current position
     * @param stream
     * @param offset
     */
    static void incWritePos(std::fstream &stream, long long offset);

    /**
     * Tell the given file stream's read pointer's current absolute position
     * @param stream
     * @return
     */
    static unsigned long long tellReadPos(std::fstream &stream);

    /**
     * Tell the given file stream's write pointer's current absolute position
     * @param stream
     * @return
     */
    static unsigned long long tellWritePos(std::fstream &stream);

    /**
     * Read length bytes from the given file stream onto the memory starting from the given pointer, ptr
     * @param stream
     * @param ptr
     * @param length
     */
    static void read(std::fstream &stream, const void* ptr, unsigned int length);

    /**
     * Write length bytes from the memory starting from the given pointer, ptr onto the given file stream.
     * @param stream
     * @param ptr
     * @param length
     */
    static void write(std::fstream &stream, const void* ptr, unsigned int length);

    /**
     * Close the given file stream
     * @param stream
     */
    static void close(std::fstream &stream);

    /**
     * Remove the file in the given path.
     * @param filePath
     */
    static void removeFile(std::string filePath);
};


#endif //CMPE_321_DBMS_HELPERS_H

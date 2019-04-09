#include <iostream>
#include <fstream>
#include "System.h"

int main(int argc, char *argv[]) {
    System* system;
    if (argc == 1) {
        std::cout << "Starting the program in test environment." << std::endl;
        std::cout << "Reading from stdin and printing to stdout." << std::endl;
        system = System::getInstance();
        system->run(std::cin, std::cout);
    } else if (argc == 3){
        std::cout << "Starting the program in production environment." << std::endl;
        std::cout << "Reading from " << argv[1] << " and printing to " << argv[2] << std::endl;
        std::ifstream inputFile;
        std::ofstream outputFile;
        inputFile.open(argv[1], std::ifstream::in);
        outputFile.open(argv[2], std::ofstream::out);
        system = System::getInstance();
        system->run(inputFile, outputFile);
    } else {
        std::cerr << "Incorrect number of arguments. Quitting." << std::endl;
        std::cerr << "Run the program as './storageManager' for "
                     "test environment, (stdin & stdout)." << std::endl;
        std::cerr << "Run the program as './storageManager [input] [output]'"
                     "for production environment." << std::endl;
        exit(1);
    }

    return 0;
}
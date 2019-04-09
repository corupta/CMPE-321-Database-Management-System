cmake_minimum_required(VERSION 3.13)
project(cmpe_321_dbms)

set(CMAKE_CXX_STANDARD 11)

add_executable(cmpe_321_dbms src/main.cpp src/SystemCatalog.cpp src/SystemCatalog.h src/RWHandler.cpp src/RWHandler.h src/Scheme.cpp src/Scheme.h src/System.cpp src/System.h src/Helpers.cpp src/Helpers.h)
cmake_minimum_required(VERSION 4.2.1) # Minimum Version of CMake

project(proto-engine) #Project Name

add_executable(proto-engine) #Name of the Executable

target_sources(proto-engine PRIVATE #The source files to build. Relative path to the Root

src/main.cpp



PUBLIC #The Include Directories
FILE_SET HEADERS
BASE_DIRS
  include
FILES
include/*.h
)




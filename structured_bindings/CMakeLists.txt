cmake_minimum_required(VERSION 3.20)

# set the project name
project(Cpp17StructuredBindings VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(struct_bind01 struct_bind01.cpp)
add_executable(struct_bind02 struct_bind02.cpp)
add_executable(struct_bind03 struct_bind03.cpp)
add_executable(struct_bind04 struct_bind04.cpp)

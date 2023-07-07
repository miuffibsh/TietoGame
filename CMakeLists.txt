cmake_minimum_required(VERSION 3.21)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

# Set the output directory for the executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build)

# Include subdirectories
add_subdirectory(Mediator)
add_subdirectory(Tietogra)
add_subdirectory(PlayerOne)

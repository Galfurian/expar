# -----------------------------------------------------------------------------
# @brief  : Root cmake file.
# @author : Enrico Fraccaroli
# @create : 23/03/2021
# @update : 23/03/2021
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Set the minimum CMake version, the project name and default build type.
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1...3.18)

# Set the project name.
project(expar)

# Set the default build type to Debug.
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# -----------------------------------------------------------------------------
# Documentation target.
# -----------------------------------------------------------------------------
#add_subdirectory(doc)

# -----------------------------------------------------------------------------
# Dependencies.
# -----------------------------------------------------------------------------
set(CMAKE_PROGRAM_PATH ${CMAKE_SOURCE_DIR}/third_party)
# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)
# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)
# add external build for antlrcpp
include(ExternalAntlr4Cpp)
# add antrl4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS})
# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)

# -----------------------------------------------------------------------------
# Set the compilation flags.
# -----------------------------------------------------------------------------
# Set C++ Standard.
set(CMAKE_CXX_STANDARD 17)

# Set the actual flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")

    message(STATUS "Disabling optimizations.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

elseif (CMAKE_BUILD_TYPE STREQUAL "Release")

    message(STATUS "Enabling optimizations.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

# -----------------------------------------------------------------------------
# Add the C++ code generation target.
# -----------------------------------------------------------------------------
# Call macro to add lexer and grammar to your build dependencies.
antlr_target(
    ExparLexer ${CMAKE_SOURCE_DIR}/grammar/ExparLexer.g4 LEXER
    PACKAGE expar::parser
)
antlr_target(
    ExparParser ${CMAKE_SOURCE_DIR}/grammar/ExparParser.g4 PARSER
    PACKAGE expar::parser
    DEPENDS_ANTLR ExparLexer
    COMPILE_FLAGS -lib ${ANTLR_ExparLexer_OUTPUT_DIR}
    VISITOR
)

# -----------------------------------------------------------------------------
# Add the C++ Executable.
# -----------------------------------------------------------------------------
add_library(
    expar
    ${CMAKE_SOURCE_DIR}/src/expar/parser.cpp
    ${CMAKE_SOURCE_DIR}/src/expar/enums.cpp
    ${CMAKE_SOURCE_DIR}/src/expar/core.cpp
    ${ANTLR_ExparLexer_CXX_OUTPUTS}
    ${ANTLR_ExparParser_CXX_OUTPUTS}
)

# -----------------------------------------------------------------------------
# Inlcude header directories and set the library.
# -----------------------------------------------------------------------------
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    # Include project directories.
    ${CMAKE_SOURCE_DIR}/include/
    # include generated files in project environment
    ${ANTLR_ExparLexer_OUTPUT_DIR}
    ${ANTLR_ExparParser_OUTPUT_DIR}
)

# -----------------------------------------------------------------------------
# Add executable libraries (if required).
# -----------------------------------------------------------------------------
#target_link_directories(
#  expar PRIVATE
#)
target_link_libraries( 
    ${PROJECT_NAME}
    antlr4_static
)

# -----------------------------------------------------------------------------
# Add tests.
# -----------------------------------------------------------------------------
# CMake has support for adding tests to a project:
enable_testing()
# Add the subdirectory containing the tests (which imports also their target).
add_subdirectory(test)
cmake_minimum_required (VERSION 3.20.2)
set(CMAKE_CXX_STANDARD 20)

project (LoLDamageCalc)

add_compile_definitions(REPO_DIR="${CMAKE_CURRENT_LIST_DIR}" WITH_REPO_DIR)

option(WITH_TEST_TOOLS "compile test-tools" ON)

add_subdirectory(calculator_engine)
add_subdirectory(cmd_interface)

if(WITH_TEST_TOOLS)
    add_subdirectory(test_tools)
endif()

# ======== Packages ========

# Boost
#if necessary, define Boost_INCLUDE_DIR and Boost_LIBRARY_DIR
find_package(Boost 1.80.0)
if(!Boost_FOUND)
    message(SEND_ERROR "Could not find vital package Boost")
    message(STATUS "Boost_INCLUDE_DIR: " ${Boost_INCLUDE_DIR})
    message(STATUS "Boost_LIBRARY_DIR: " ${Boost_LIBRARY_DIR})
    message(FATAL_ERROR "aborting!")
endif()

# ======== Custom libraries (as git submodules) ========

add_subdirectory(common_lib_c)



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

add_subdirectory(common_lib_c)

cmake_minimum_required(VERSION 3.20.2)
set(CMAKE_CXX_STANDARD 20)

add_library (environment environment.cpp)
target_include_directories (environment PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(environment PUBLIC ${Boost_INCLUDE_DIR})

add_subdirectory(champions)
add_subdirectory(engine_signal_system)


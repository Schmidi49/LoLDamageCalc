cmake_minimum_required(VERSION 3.20.2)
set(CMAKE_CXX_STANDARD 20)

#include_directories(../../common_lib_c/json/single_include/nlohmann)

add_library (lib_champion Attacker_Champion.cpp Defender_Champion.cpp Base_Champion.cpp)

target_link_libraries(lib_champion PRIVATE lib_json)

target_include_directories (lib_champion PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
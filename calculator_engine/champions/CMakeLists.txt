cmake_minimum_required(VERSION 3.20.2)
set(CMAKE_CXX_STANDARD 20)

#include_directories(../../common_lib_c/json/single_include/nlohmann)

add_library (champions Base_Champion.cpp Attacker_Champion.cpp Defender_Champion.cpp)

target_link_libraries(champions PRIVATE lib_json)
target_link_libraries(champions PRIVATE engine_signal_system)

target_include_directories (champions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


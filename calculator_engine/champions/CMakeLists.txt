cmake_minimum_required(VERSION 3.20.2)
set(CMAKE_CXX_STANDARD 20)

add_library (champions Stats.hpp Generic_Damage_Spell.cpp Base_Champion.cpp Attacker_Champion.cpp Defender_Champion.cpp)


target_link_libraries(champions PRIVATE lib_json)
target_link_libraries(champions PRIVATE engine_signal_system)
target_link_libraries(champions PUBLIC environment)

target_include_directories (champions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CHAMP_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
message(${CHAMP_SRC_DIR})

target_sources(champions
        PUBLIC ${CHAMP_SRC_DIR}/Jinx.cpp)
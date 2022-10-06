cmake_minimum_required(VERSION 3.20.2)
set(CMAKE_CXX_STANDARD 20)

add_library (engine_signal_system engine_signal_system.hpp)
target_include_directories(engine_signal_system PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories (engine_signal_system PUBLIC ${CMAKE_CURRENT_LIST_DIR})
set_target_properties(engine_signal_system PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 20)
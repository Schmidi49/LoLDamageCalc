cmake_minimum_required(VERSION 3.20.2)
set(CMAKE_CXX_STANDARD 20)

add_executable(test_tool_main main.cpp)
target_link_libraries(test_tool_main PRIVATE champions)
target_link_libraries(test_tool_main PRIVATE lib_json)
target_link_libraries(test_tool_main PRIVATE engine_signal_system)

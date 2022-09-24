cmake_minimum_required(VERSION 3.20.2)
set(CMAKE_CXX_STANDARD 20)

# nlohmann_json

add_subdirectory(json)
add_library(lib_json json/single_include/nlohmann/json.hpp)
target_link_libraries(lib_json PRIVATE nlohmann_json::nlohmann_json)
target_include_directories (lib_json PUBLIC ../common_lib_c/json/single_include/nlohmann)
set_target_properties(lib_json PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 11)


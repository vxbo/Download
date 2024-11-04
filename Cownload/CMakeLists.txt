cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(Cownload LANGUAGES CXX VERSION 1.0.0)

include_directories(include)
add_library(${PROJECT_NAME} STATIC "src/lib.cpp")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

include("cmake/vcpkg.cmake")

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

find_package(CURL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl)

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>$<INSTALL_INTERFACE:include>")

add_subdirectory(tests)
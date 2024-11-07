cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(Cownloadpp VERSION 1.0.2 LANGUAGES CXX)

include_directories("include")
add_library(${PROJECT_NAME} STATIC "src/Cownloadpp.cpp")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

include("cmake/vcpkg.cmake")

find_package(unofficial-curlpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::curlpp::curlpp)

find_package(Boost REQUIRED COMPONENTS filesystem system)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::filesystem Boost::system)

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>$<INSTALL_INTERFACE:include>")

add_subdirectory(tests)
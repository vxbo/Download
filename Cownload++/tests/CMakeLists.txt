# tests/CMakeLists.txt

enable_testing()

add_executable(TestRunner main.cpp)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(TestRunner PRIVATE Cownloadpp GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(NAME GTests COMMAND TestRunner)
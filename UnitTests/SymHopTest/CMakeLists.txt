cmake_minimum_required(VERSION 3.0)
project(HopsanCoreTests)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_DEBUG_POSTFIX _d)

set(test_name tst_symhoptest)

add_executable(${test_name} ${test_name}.cpp)
target_compile_definitions(${test_name} PRIVATE TEST_DATA_ROOT=\"${CMAKE_CURRENT_LIST_DIR}/\")
target_link_libraries(${test_name} symhop Qt5::Test)
add_test(NAME ${test_name} COMMAND ${test_name})

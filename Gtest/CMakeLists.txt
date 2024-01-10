# 'Google_test' is the subproject name
project(Gtest)

# 'lib' is the folder with Google Test sources
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
# 'test1.cpp test2.cpp' are source files with tests
add_executable(Gtest_run TestRational.cpp TestModulo.cpp TestMonomialDegree.cpp TestMonomial.cpp)
target_link_libraries(Gtest_run src)
target_link_libraries(Gtest_run gtest gtest_main)
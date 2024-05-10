project(Gtest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Gtest_run TestRational.cpp TestModulo.cpp TestMonomial.cpp TestMonomialCompare.cpp
        TestPolynomial.cpp TestGroebnerAlgorithm.cpp TestPolySystem.cpp TestVariableOrder.cpp)
target_link_libraries(Gtest_run src)
target_link_libraries(Gtest_run gtest gtest_main)
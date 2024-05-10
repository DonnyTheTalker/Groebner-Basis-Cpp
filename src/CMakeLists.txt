project(Groebner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(HEADER_FILES
        Rational.h
        Modulo.h
        FieldFwd.h
        MonomialCompare.h
        Monomial.h
        ComparatorFwd.h
        Polynomial.h
        PolySystem.h
        GroebnerAlgorithm.h
        ListFwd.h
        VariableOrder.h
        Printer.h
)

set(SOURCE_FILES
        Rational.cpp
        Monomial.cpp
        MonomialCompare.cpp
        Modulo.cpp
        VariableOrder.cpp
        Printer.cpp
)

add_library(src STATIC ${SOURCE_FILES})
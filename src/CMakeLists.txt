project(Groebner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(HEADER_FILES
        Rational.h
        Modulo.h
        FieldFwd.h
        MonomialCompare.h
        MonomialDegree.h
        ComparatorFwd.h
        Polynomial.h
        PolySystem.h
        GroebnerBuilder.h
)

set(SOURCE_FILES
        Rational.cpp
        MonomialDegree.cpp
        Polynomial.cpp
        PolySystem.cpp
        GroebnerBuilder.cpp
)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
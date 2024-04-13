project(Groebner)

set(HEADER_FILES
        Rational.h
        Modulo.h
        FieldFwd.h
        Monomial.h
        MonomialCompare.h
        MonomialDegree.h
        ComparatorFwd.h
        Polynomial.h
        PolySystem.h
        GroebnerBuilder.h
)

set(SOURCE_FILES
        Rational.cpp
        Monomial.cpp
        MonomialCompare.cpp
        MonomialDegree.cpp
        Polynomial.cpp
        PolySystem.cpp
        GroebnerBuilder.cpp
)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
project(Groebner)

set(HEADER_FILES
        Rational.h
        Modulo.h
        FieldFwd.h
        Monomial.h
        MonomialCompare.h
        MonomialDegree.h
        ComparatorFwd.h
)

set(SOURCE_FILES
        Rational.cpp
        Modulo.cpp
        Monomial.cpp
        MonomialCompare.cpp
        MonomialDegree.cpp
)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
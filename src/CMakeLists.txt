project(Groebner)

set(HEADER_FILES
        Rational.h
        Modulo.h
)

set(SOURCE_FILES
        Rational.cpp
        Modulo.cpp
)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
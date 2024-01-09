project(Groebner)

set(HEADER_FILES
        Rational.h
)

set(SOURCE_FILES
        Rational.cpp
)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
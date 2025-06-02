add_library(Field
        Field.cpp
        Field.h
        BitMatrix/BitMatrix.h
)

target_include_directories(Field PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(MyBitMap)

add_library(BitMapDraw
        BitMapDraw.cpp
        BitMapDraw.h
)

target_link_libraries(BitMapDraw
        MyBitMap
        Field
)

target_include_directories(BitMapDraw PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

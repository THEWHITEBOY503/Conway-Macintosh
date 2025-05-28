add_library(DirectDraw
        DirectDraw.cpp
        DirectDraw.h
)

target_link_libraries(DirectDraw
        Field
)

target_include_directories(DirectDraw PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

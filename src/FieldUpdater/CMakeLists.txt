add_library(FieldUpdater
        FieldUpdater.cpp
        FieldUpdater.h
)

target_link_libraries(FieldUpdater
        Field
        GridDraw
)

target_include_directories(FieldUpdater PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

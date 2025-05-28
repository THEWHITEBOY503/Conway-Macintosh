add_subdirectory(BitMapDraw)
add_subdirectory(DirectDraw)

add_library(GridDraw INTERFACE)

target_link_libraries(GridDraw INTERFACE
        BitMapDraw
        DirectDraw
)

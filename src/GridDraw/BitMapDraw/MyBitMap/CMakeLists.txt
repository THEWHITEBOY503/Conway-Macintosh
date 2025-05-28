add_library(MyBitMap INTERFACE
        MyBitMap.h
)

target_include_directories(MyBitMap INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

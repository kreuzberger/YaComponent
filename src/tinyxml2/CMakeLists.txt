project(
    tinyxml2
    VERSION 9.0.0
    LANGUAGES CXX
)

add_library(tinyxml2 STATIC "")

target_sources(
    tinyxml2
    PRIVATE tinyxml2.cpp
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2.h ${CMAKE_CURRENT_SOURCE_DIR}/tixml2cx.h
           ${CMAKE_CURRENT_SOURCE_DIR}/tixml2ex.h
)

target_include_directories(tinyxml2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(tinyxml2::tinyxml2 ALIAS tinyxml2)

project(YaComponentCore)

add_library(YaComponentCore STATIC)
target_sources(YaComponentCore  PRIVATE   ${YaComponentCore_SOURCE_DIR}/src/YaComponentCore.cpp
    ${YaComponentCore_SOURCE_DIR}/include/core/YaComponentCore.h
    )

target_include_directories(YaComponentCore PUBLIC ${YaComponentCore_SOURCE_DIR}/include)

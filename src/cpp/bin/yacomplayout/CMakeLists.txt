project(YaCompLayoutGenerator)

add_executable(yacomplayout)
target_sources(yacomplayout PRIVATE main.cpp YaCompLayoutParser.cpp YaCompLayoutParser.h)
target_link_libraries(yacomplayout PRIVATE YaComponentCore tinyxml2)

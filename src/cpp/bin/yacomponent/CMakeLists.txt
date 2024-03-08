project(YaComponentGenerator)

add_executable(yacomponent)

target_sources(yacomponent PRIVATE main.cpp YaComponentParser.cpp YaComponentParser.h YaComponentCodeGen.cpp YaComponentCodeGen.h)

target_link_libraries( yacomponent YaComponentCore tinyxml2)



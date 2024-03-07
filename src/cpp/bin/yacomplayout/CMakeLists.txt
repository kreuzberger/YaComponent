project(YaCompLayoutGenerator)

add_executable(yacomplayout)
target_sources(yacomplayout PRIVATE main.cpp YaCompLayoutParser.cpp YaCompLayoutParser.h YaCompLayoutCodeGen.cpp YaCompLayoutCodeGen.h)
target_link_libraries(yacomplayout PRIVATE YaComponentCore tinyxml2)

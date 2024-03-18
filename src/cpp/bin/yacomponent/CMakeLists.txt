project(YaComponentGenerator)

add_executable(yacomponent)

target_sources(yacomponent PRIVATE main.cpp YaComponentParser.cpp YaComponentParser.h YaComponentCodeGen.cpp YaComponentCodeGen.h
                                    YaComponentDocGen.cpp YaComponentDocGen.h
)

target_link_libraries( yacomponent YaComponentCore tinyxml2)

install(
    TARGETS yacomponent
    EXPORT YaComponentTargets
    EXCLUDE_FROM_ALL
    DESTINATION "yacomponent/bin"
    COMPONENT "yacomponent"
)

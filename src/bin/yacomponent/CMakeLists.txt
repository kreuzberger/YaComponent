project(YaComponentGenerator)

add_executable(yacomponent)

target_sources(
    yacomponent
    PRIVATE main.cpp
            YaComponentParser.cpp
            YaComponentParser.h
            YaComponentCodeGen.cpp
            YaComponentCodeGen.h
            YaComponentPyCodeGen.cpp
            YaComponentPyCodeGen.h
            YaComponentDocGen.cpp
            YaComponentDocGen.h
)

target_link_libraries(yacomponent YaComponentCore tinyxml2)

install(
    TARGETS yacomponent
    EXPORT YaComponentTargets
    DESTINATION "bin"
    COMPONENT "yacomponent"
)

add_executable(YaComponent::yacomponent ALIAS yacomponent)

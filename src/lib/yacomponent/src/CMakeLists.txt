add_library( YaComponent STATIC )

set( YaComponentLib_PUBLIC_HEADERS
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaComponentThread.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaProxyBase.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaPUBImpl.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaStubBase.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaSUBImpl.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaBuffer.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaComponent.h
)

target_sources( YaComponent PRIVATE
    YaBuffer.cpp
    YaComponent.cpp
    YaComponentThread.cpp
    YaProxyBase.cpp
    YaPUBImpl.cpp
    YaStubBase.cpp
    YaSUBImpl.cpp
    ${YaComponentLib_PUBLIC_HEADERS}
)

#target_link_libraries( YaComponentLib PUBLIC Qt5::Core libzmq-static protobuf::libprotobuf absl::log absl::log_internal_check_op absl::node_hash_set absl::status absl::statusor absl::cord absl::strings)
target_link_libraries( YaComponent PUBLIC Qt5::Core libzmq-static protobuf::libprotobuf)
target_include_directories( YaComponent
    PUBLIC # where top-level project will look for the library's public headers
           $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
           # where external projects will look for the library's public headers
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


install(
    TARGETS YaComponent
    EXPORT YaComponentTargets
    DESTINATION "lib"
    COMPONENT "yacomponent"
)

install(
    DIRECTORY "${YaComponentLib_SOURCE_DIR}/include/yacomponent"
    DESTINATION "include"
    COMPONENT "yacomponent"
)

add_library(YaComponent::YaComponent ALIAS YaComponent)

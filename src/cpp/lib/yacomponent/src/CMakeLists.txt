add_library( YaComponentLib STATIC )

set( YaComponentLib_PUBLIC_HEADERS
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaComponentThread.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaProxyBase.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaPUBImpl.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaStubBase.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaSUBImpl.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaBuffer.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaComponent.h
)

target_sources( YaComponentLib PRIVATE
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
target_link_libraries( YaComponentLib PUBLIC Qt5::Core libzmq-static libprotobuf)
target_include_directories( YaComponentLib
    PUBLIC # where top-level project will look for the library's public headers
           $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
           # where external projects will look for the library's public headers
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


install(
    TARGETS YaComponentLib
    EXPORT YaComponentTargets
    EXCLUDE_FROM_ALL
    DESTINATION "yacomponent/lib"
    COMPONENT "yacomponent"
)

install(
    DIRECTORY "${YaComponentLib_SOURCE_DIR}/include/yacomponent" EXCLUDE_FROM_ALL
    DESTINATION "yacomponent/include"
    COMPONENT "yacomponent"
)


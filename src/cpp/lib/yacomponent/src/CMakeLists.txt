add_library( YaComponentLib STATIC   )

target_sources( YaComponentLib PRIVATE
    YaBuffer.cpp
    YaComponent.cpp
    YaComponentThread.cpp
    YaProxyBase.cpp
    YaPUBImpl.cpp
    YaStubBase.cpp
    YaSUBImpl.cpp

    PUBLIC
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaComponentThread.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaProxyBase.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaPUBImpl.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaStubBase.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaSUBImpl.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaBuffer.h
    ${YaComponentLib_SOURCE_DIR}/include/yacomponent/YaComponent.h
)

#target_link_libraries( YaComponentLib PUBLIC Qt5::Core libzmq-static protobuf::libprotobuf absl::log absl::log_internal_check_op absl::node_hash_set absl::status absl::statusor absl::cord absl::strings)
target_link_libraries( YaComponentLib PUBLIC Qt5::Core libzmq-static libprotobuf)
target_include_directories( YaComponentLib PUBLIC "${YaComponentLib_SOURCE_DIR}/include")

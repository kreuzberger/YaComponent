project( YaComponentLib )


set( YaComponentLib_SRCS
  YaBuffer.cpp
  YaComponent.cpp
  YaComponentThread.cpp
  YaProxyBase.cpp
  YaPUBImpl.cpp
  YaStubBase.cpp
  YaSUBImpl.cpp
)

set( YaComponentLib_HDRS
  YaBuffer.h
  YaComponent.h
  YaComponentThread.h
  YaProxyBase.h
  YaPUBImpl.h
  YaStubBase.h
  YaSUBImpl.h
)

qt5_wrap_cpp( YaComponentLib_MOCS
  YaStubBase.h
  YaComponentThread.h
  YaProxyBase.h
)


add_library( YaComponentLib ${YaComponentLib_SRCS} ${YaComponentLib_HDRS} ${YaComponentLib_MOCS} )

target_link_libraries( YaComponentLib Qt5::Core libzmq::static protobuf::lib)

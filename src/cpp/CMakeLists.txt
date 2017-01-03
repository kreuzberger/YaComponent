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

qt4_wrap_cpp( YaComponentLib_MOCS
  YaStubBase.h
  YaComponentThread.h
  YaProxyBase.h
)


add_library( YaComponentLib ${YaComponentLib_SRCS} ${YaComponentLib_HDRS} ${YaComponentLib_MOCS} )

if( WITH_ZMQ )
  add_dependencies( YaComponentLib zeromq)
endif()

if( WITH_PROTOBUF )
  add_dependencies( YaComponentLib protobuf )
endif()

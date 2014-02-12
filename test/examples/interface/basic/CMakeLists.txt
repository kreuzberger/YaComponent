project ( BasicInterface)
set( DESIRED_QT_VERSION 4 )
include(${QT_USE_FILE})

PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders BasicInterface.proto)

add_library( basicinterface ${ProtoSources} ${ProtoHeaders})





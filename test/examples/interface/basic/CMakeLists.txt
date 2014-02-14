project ( BasicInterface)
set( DESIRED_QT_VERSION 4 )
include(${QT_USE_FILE})

PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders BasicInterface.proto)

YACOMPONENT_IFC_GENERATE(GENERATED_IFC_GEN_SRC BasicInterface.xml )

add_library( basicinterface ${ProtoSources} ${ProtoHeaders} ${GENERATED_IFC_GEN_SRC})




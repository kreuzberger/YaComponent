project ( BasicInterface)
set( DESIRED_QT_VERSION 4 )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
include(${QT_USE_FILE})
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )
#IFC_GENERATE(GENERATED_IFC_SRC BasicInterface.xml)
#QT4_WRAP_CPP( GENERATED_FSM_SRC_MOC_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/FullFSM/code/FSMTimer.h)

PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders BasicInterface.proto)

add_library( basicinterface ${ProtoSources} ${ProtoHeaders})





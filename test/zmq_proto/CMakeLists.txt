project ( PubSubProto )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
include(${QT_USE_FILE})
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )
#QT4_WRAP_CPP( GENERATED_FSM_SRC_MOC_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/FullFSM/code/FSMTimer.h)
#add_library( pubsubtcp  ) # ${GENERATED_IFC_SRC_MOC_HEADERS})

PROTOBUF_GENERATE_CPP(TextSources TextHeaders TextGenerator.proto)

YACOMPONENT_IFC_GENERATE(GENERATED_IFC_GEN_SRC TextGenerator.xml )

add_executable(pubproto psenvpub.cpp psenvbase.cpp psenvbase.h ${GENERATED_IFC_GEN_SRC} ${TextSources} ${TextHeaders})
add_executable(subproto psenvsub.cpp psenvbase.cpp psenvbase.h ${GENERATED_IFC_GEN_SRC} ${TextSources} ${TextHeaders})


target_link_libraries(pubproto ${ZEROMQ_LIBRARIES} ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES})
target_link_libraries(subproto ${ZEROMQ_LIBRARIES} ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES})




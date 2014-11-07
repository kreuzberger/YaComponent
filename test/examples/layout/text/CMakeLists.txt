project ( TextExample)

set( QT_USE_QTCORE true )
set( QT_USE_QTGUI true )

include(${INSTALL_QT_USE_FILE})

include_directories( ${QT_INCLUDE_DIR} )


PROTOBUF_GENERATE_CPP(ProtoSourcesTextGen ProtoHeadersTextGen textgen/TextGenIfc.proto)

YACOMPONENT_IFC_GENERATE(GENERATED_IFC_SRC textgen/TextGenIfc.xml textout/TextOutIfc.xml)
YACOMPONENT_IFC_GENERATE(GENERATED_IFC_SRC_OUT textout/TextOutIfc.xml)


YACOMPONENT_GENERATE(GENERATED_COMP_GEN_SRC textgen/TextGenComp.xml)
YACOMPONENT_GENERATE(GENERATED_COMP_OUT_SRC textout/TextOutComp.xml)

YACOMPONENT_LAYOUT_GENERATE(GENERATED_LAYOUT_SRC TextExampleMPMT.xml)

set( textgen_SRCS
  textgenMPMT.cpp
  textgen/textgencomp.cpp
  ${GENERATED_IFC_SRC}
  ${GENERATED_COMP_GEN_SRC}
  ${ProtoSourcesTextGen}
  ${GENERATED_LAYOUT_SRC}
)

set( textout_SRCS
  textoutMPMT.cpp
  textout/textoutcomp.cpp
  ${GENERATED_IFC_SRC}
  ${GENERATED_IFC_SRC_OUT}
  ${GENERATED_COMP_GEN_SRC}
  ${GENERATED_COMP_OUT_SRC}
  ${ProtoSourcesTextGen}
  ${GENERATED_LAYOUT_SRC}

)

add_executable( textgenMPMT  ${textgen_SRCS} )
add_executable( textoutMPMT  ${textout_SRCS} )

target_link_libraries(textgenMPMT ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES} ${ZEROMQ_LIBRARIES})
target_link_libraries(textoutMPMT ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES} ${ZEROMQ_LIBRARIES})



install( TARGETS textgenMPMT textoutMPMT RUNTIME DESTINATION "examples/text" )
install( FILES ${QT_INSTALL_FILES} DESTINATION "examples/text" )


#add_test( NAME texttest
#          COMMAND ${CMAKE_COMMAND} -DTEXTGEN_EXE=$<TARGET_FILE:textgenMPMT> -DTEXTOUT_EXE=$<TARGET_FILE:textoutMPMT> -DTESTDIR_PATH=${CMAKE_CURRENT_BINARY_PATH} -P ${CMAKE_CURRENT_LIST_DIR}/TextTest.cmake
#          )

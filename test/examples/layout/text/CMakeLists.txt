project ( TextExample)

set( QT_USE_QTCORE true )
set( QT_USE_QTGUI true )
set( QT_USE_QTTEST true )

include(${INSTALL_QT_USE_FILE})

include_directories( ${QT_INCLUDE_DIR} )


PROTOBUF_GENERATE_CPP(ProtoSourcesTextGen ProtoHeadersTextGen textgen/TextGenIfc.proto)

YACOMPONENT_IFC_GENERATE(GENERATED_IFC_SRC textgen/TextGenIfc.xml textout/TextOutIfc.xml)
YACOMPONENT_IFC_GENERATE(GENERATED_IFC_SRC_OUT textout/TextOutIfc.xml)


YACOMPONENT_GENERATE(GENERATED_COMP_GEN_SRC textgen/TextGenComp.xml)
YACOMPONENT_GENERATE(GENERATED_COMP_OUT_SRC textout/TextOutComp.xml)

YACOMPONENT_LAYOUT_GENERATE(GENERATED_LAYOUT_SRC TextExampleMPMT.xml)
YACOMPONENT_LAYOUT_GENERATE(GENERATED_LAYOUT_SRC_SPMT TextExampleSPMT.xml)

set( textgen_SRCS
  ${GENERATED_IFC_SRC}
  ${GENERATED_COMP_GEN_SRC}
  ${ProtoSourcesTextGen}
  ${GENERATED_LAYOUT_SRC}
  textgenMPMT.cpp
  textgen/textgencomp.cpp
)

set( textout_SRCS
  ${GENERATED_IFC_SRC}
  ${GENERATED_IFC_SRC_OUT}
  ${GENERATED_COMP_GEN_SRC}
  ${GENERATED_COMP_OUT_SRC}
  ${ProtoSourcesTextGen}
  ${GENERATED_LAYOUT_SRC}
  textoutMPMT.cpp
  textout/textoutcomp.cpp
)

QT4_WRAP_CPP( UNITTEST_MOCS unittest/TextUnittest.h )

set( textunittest_SRCS
  ${GENERATED_IFC_SRC}
  ${GENERATED_IFC_SRC_OUT}
  ${GENERATED_COMP_GEN_SRC}
  ${GENERATED_COMP_OUT_SRC}
  ${ProtoSourcesTextGen}
  ${GENERATED_LAYOUT_SRC_SPMT}
  ${UNITTEST_MOCS}
  unittest/textoutcompunittest.cpp
  unittest/textgencompunittest.cpp
  unittest/TextUnittest.cpp
  unittest/TextUnittest.h
)

add_executable( textgenMPMT  ${textgen_SRCS} )
add_executable( textoutMPMT  ${textout_SRCS} )
add_executable( textunittest ${textunittest_SRCS} )


target_link_libraries(textgenMPMT ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES} ${ZEROMQ_LIBRARIES})
target_link_libraries(textoutMPMT ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES} ${ZEROMQ_LIBRARIES})
target_link_libraries(textunittest ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES} ${ZEROMQ_LIBRARIES})


install( TARGETS textgenMPMT textoutMPMT RUNTIME DESTINATION "examples/text" )
install( TARGETS textunittest RUNTIME DESTINATION "examples/text/unittest" )
#install( FILES ${QT_INSTALL_FILES} DESTINATION "examples/text" )


#add_test( NAME texttest
#          COMMAND ${CMAKE_COMMAND} -DTEXTGEN_EXE=$<TARGET_FILE:textgenMPMT> -DTEXTOUT_EXE=$<TARGET_FILE:textoutMPMT> -DTESTDIR_PATH=${CMAKE_CURRENT_BINARY_PATH} -P ${CMAKE_CURRENT_LIST_DIR}/TextTest.cmake
#          )

add_test( NAME textunittest COMMAND textunittest )

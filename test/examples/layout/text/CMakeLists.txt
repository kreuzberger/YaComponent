project ( TextExample)
set( DESIRED_QT_VERSION 4 )
find_package ( Qt4 REQUIRED )

#INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
include(${QT_USE_FILE})
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )


PROTOBUF_GENERATE_CPP(ProtoSourcesTextGen ProtoHeadersTextGen textgen/TextGenIfc.proto)

YACOMPONENT_IFC_GENERATE(GENERATED_IFC_SRC textgen/TextGenIfc.xml textout/TextOutIfc.xml)
YACOMPONENT_IFC_GENERATE(GENERATED_IFC_SRC_OUT textout/TextOutIfc.xml)



YACOMPONENT_GENERATE(GENERATED_COMP_GEN_SRC textgen/TextGenComp.xml)
YACOMPONENT_GENERATE(GENERATED_COMP_OUT_SRC textout/TextOutComp.xml)

YACOMPONENT_LAYOUT_GENERATE(GENERATED_LAYOUT_SRC TextExampleMPMT.xml)

set( textgen_SRCS ${GENERATED_LAYOUT_SRC}
  textgenMPMT.cpp
  textgen/textgencomp.cpp
  ${GENERATED_IFC_SRC}
  ${GENERATED_COMP_GEN_SRC}
  ${ProtoSourcesTextGen}
)

set( textout_SRCS
  ${GENERATED_LAYOUT_SRC}
  textoutMPMT.cpp
  textout/textoutcomp.cpp
  ${GENERATED_IFC_SRC}
  ${GENERATED_IFC_SRC_OUT}
  ${GENERATED_COMP_OUT_SRC}
)

add_executable( textgenMPMT  ${textgen_SRCS} )
add_executable( textoutMPMT  ${textout_SRCS} )

target_link_libraries(textgenMPMT ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES} ${ZEROMQ_LIBRARIES})
target_link_libraries(textoutMPMT ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES} ${ZEROMQ_LIBRARIES})




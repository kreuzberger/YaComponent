project ( TextExample)

PROTOBUF_GENERATE(OUT_VAR ProtoSourcesTextGen LANGUAGE cpp PROTOS textgen/TextGenIfc.proto APPEND_PATH ${CMAKE_CURRENT_SOURCE_DIR})

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
  textgencompunittest.cpp
  TextUnittestMP_PUB.cpp
  TextUnittestMP_PUB.h
  ${UNITTESTMPPUB_MOCS}
)

set( textout_SRCS
  ${GENERATED_IFC_SRC}
  ${GENERATED_IFC_SRC_OUT}
  ${GENERATED_COMP_GEN_SRC}
  ${GENERATED_COMP_OUT_SRC}
  ${ProtoSourcesTextGen}
  ${GENERATED_LAYOUT_SRC}
  textoutcompunittest.cpp
  TextUnittestMP_SUB.cpp
  TextUnittestMP_SUB.h
  ${UNITTESTMPSUB_MOCS}
)

set( textunittest_SRCS
  ${GENERATED_IFC_SRC}
  ${GENERATED_IFC_SRC_OUT}
  ${GENERATED_COMP_GEN_SRC}
  ${GENERATED_COMP_OUT_SRC}
  ${ProtoSourcesTextGen}
  ${GENERATED_LAYOUT_SRC_SPMT}
  textoutcompunittest.cpp
  textgencompunittest.cpp
  TextUnittest.cpp
  TextUnittest.h
  ${UNITTEST_MOCS}
)

set( textunittestmp_SRCS
  TextUnittestMP.cpp
  TextUnittestMP.h
  ${UNITTESTMP_MOCS}
)




add_executable( textgenMPMT  ${textgen_SRCS} )
add_executable( textoutMPMT  ${textout_SRCS} )
add_executable( textunittest ${textunittest_SRCS} )
add_executable( textunittestmp ${textunittestmp_SRCS} )


target_link_libraries(textgenMPMT PRIVATE YaComponent Qt5::Test)
target_link_libraries(textoutMPMT PRIVATE YaComponent Qt5::Test)
target_link_libraries(textunittest PRIVATE YaComponent Qt5::Test)
target_link_libraries(textunittestmp PRIVATE YaComponent Qt5::Test)

add_test( NAME textunittest COMMAND textunittest )
add_test( NAME textunittestmp COMMAND textunittestmp )

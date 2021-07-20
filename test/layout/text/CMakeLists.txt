project ( TextExample)

PROTOBUF_GENERATE_CPP(ProtoSourcesTextGen ProtoHeadersTextGen textgen/TextGenIfc.proto)

YACOMPONENT_IFC_GENERATE(GENERATED_IFC_SRC textgen/TextGenIfc.xml textout/TextOutIfc.xml)
YACOMPONENT_IFC_GENERATE(GENERATED_IFC_SRC_OUT textout/TextOutIfc.xml)


YACOMPONENT_GENERATE(GENERATED_COMP_GEN_SRC textgen/TextGenComp.xml)
YACOMPONENT_GENERATE(GENERATED_COMP_OUT_SRC textout/TextOutComp.xml)

YACOMPONENT_LAYOUT_GENERATE(GENERATED_LAYOUT_SRC TextExampleMPMT.xml)
YACOMPONENT_LAYOUT_GENERATE(GENERATED_LAYOUT_SRC_SPMT TextExampleSPMT.xml)

QT5_WRAP_CPP( UNITTESTMPSUB_MOCS TextUnittestMP_SUB.h )
QT5_WRAP_CPP( UNITTESTMPPUB_MOCS TextUnittestMP_PUB.h )

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

QT5_WRAP_CPP( UNITTEST_MOCS TextUnittest.h )

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

QT5_WRAP_CPP( UNITTESTMP_MOCS TextUnittestMP.h )

set( textunittestmp_SRCS
  TextUnittestMP.cpp
  TextUnittestMP.h
  ${UNITTESTMP_MOCS}
)




add_executable( textgenMPMT  ${textgen_SRCS} )
add_executable( textoutMPMT  ${textout_SRCS} )
add_executable( textunittest ${textunittest_SRCS} )
add_executable( textunittestmp ${textunittestmp_SRCS} )


target_link_libraries(textgenMPMT Qt5::Core Qt5::Test ${PROTOBUF_LIBRARIES} libzmq::static)
target_link_libraries(textoutMPMT Qt5::Core Qt5::Test ${PROTOBUF_LIBRARIES} libzmq::static)
target_link_libraries(textunittest Qt5::Core Qt5::Test ${PROTOBUF_LIBRARIES} libzmq::static)
target_link_libraries(textunittestmp Qt5::Core Qt5::Test ${PROTOBUF_LIBRARIES} libzmq::static)

install( TARGETS textgenMPMT textoutMPMT RUNTIME DESTINATION "unittest/text" )
install( TARGETS textunittest RUNTIME DESTINATION "unittest/text" )
install( TARGETS textunittestmp RUNTIME DESTINATION "unittest/text" )


add_test( NAME textunittest COMMAND textunittest )
add_test( NAME textunittestmp COMMAND textunittestmp )

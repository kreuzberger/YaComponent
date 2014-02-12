project ( BasicLayout)
set( DESIRED_QT_VERSION 4 )
find_package ( Qt4 REQUIRED )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
include(${QT_USE_FILE})
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )

YACOMPONENT_GENERATE(GENERATED_IFC_GEN_SRC TextGenComp.xml)
YACOMPONENT_GENERATE(GENERATED_IFC_OUT_SRC TextOutComp.xml)

YACOMPONENT_LAYOUT_GENERATE(GENERATED_LAYOUT_SRC BasicLayout.xml)

PROTOBUF_GENERATE_CPP(ProtoSourcesTextGen ProtoHeadersTextGen TextGenIfc.proto)

add_executable( process1  ${GENERATED_LAYOUT_SRC} main1.cpp  textgencomp.cpp ${GENERATED_IFC_GEN_SRC} ${ProtoSourcesTextGen})
add_executable( process2  ${GENERATED_LAYOUT_SRC} main2.cpp  textoutcomp.cpp ${GENERATED_IFC_OUT_SRC})

target_link_libraries(process1 ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES})
target_link_libraries(process2 ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES})




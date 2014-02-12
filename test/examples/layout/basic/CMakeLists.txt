project ( BasicLayout)
set( DESIRED_QT_VERSION 4 )
find_package ( Qt4 REQUIRED )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
include(${QT_USE_FILE})
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )

IFC_GENERATE(GENERATED_IFC_GEN_SRC TextGenComp.xml)
IFC_GENERATE(GENERATED_IFC_OUT_SRC TextOutComp.xml)

COMPLAYOUT_GENERATE(GENERATED_LAYOUT_SRC BasicLayout.xml)

add_executable( process1  ${GENERATED_LAYOUT_SRC} main1.cpp  textgencomp.cpp ${GENERATED_IFC_GEN_SRC})
add_executable( process2  ${GENERATED_LAYOUT_SRC} main2.cpp  textoutcomp.cpp ${GENERATED_IFC_OUT_SRC})

target_link_libraries(process1 ${QT_LIBRARIES})
target_link_libraries(process2 ${QT_LIBRARIES})




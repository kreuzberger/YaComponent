project ( PubSubTcp)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
include(${QT_USE_FILE})
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )
#QT4_WRAP_CPP( GENERATED_FSM_SRC_MOC_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/FullFSM/code/FSMTimer.h)
#add_library( pubsubtcp  ) # ${GENERATED_IFC_SRC_MOC_HEADERS})

add_executable(pubtcp psenvpubtcp.cpp psenvbase.cpp psenvbase.h YaPUBImpl.cpp YaPUBImpl.h)
add_executable(subtcp psenvsubtcp.cpp psenvbase.cpp psenvbase.h YaSUBImpl.cpp YaSUBImpl.h)
add_executable(pubipc psenvpubipc.cpp psenvbase.cpp psenvbase.h YaPUBImpl.cpp YaPUBImpl.h)
add_executable(subipc psenvsubipc.cpp psenvbase.cpp psenvbase.h YaSUBImpl.cpp YaSUBImpl.h)


target_link_libraries(pubtcp ${ZEROMQ_LIBRARIES} ${QT_LIBRARIES})
target_link_libraries(subtcp ${ZEROMQ_LIBRARIES} ${QT_LIBRARIES})
target_link_libraries(pubipc ${ZEROMQ_LIBRARIES} ${QT_LIBRARIES})
target_link_libraries(subipc ${ZEROMQ_LIBRARIES} ${QT_LIBRARIES})




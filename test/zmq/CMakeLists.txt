project ( PubSubTcp)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
include(${QT_USE_FILE})
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )
#QT4_WRAP_CPP( GENERATED_FSM_SRC_MOC_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/FullFSM/code/FSMTimer.h)
#add_library( pubsubtcp  ) # ${GENERATED_IFC_SRC_MOC_HEADERS})

add_executable(pub psenvpub.cpp psenvbase.cpp psenvbase.h YaPUBImpl.cpp YaPUBImpl.h YaComponent.h YaComponent.cpp)
add_executable(sub psenvsub.cpp psenvbase.cpp psenvbase.h YaSUBImpl.cpp YaSUBImpl.h YaComponent.h YaComponent.cpp)


target_link_libraries(pub ${ZEROMQ_LIBRARIES} ${QT_LIBRARIES})
target_link_libraries(sub ${ZEROMQ_LIBRARIES} ${QT_LIBRARIES})




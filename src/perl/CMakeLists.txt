#add_subdirectory(images)
project(YaComponent_GENERATOR)

#ADD_CUSTOM_COMMAND(
#  OUTPUT FSM
#  COMMAND echo bla > ${CMAKE_CURRENT_SOURCE_DIR}/test.h
#CMAKE_CURRENT_BINARY_DIR
#)

#set( FSM_SRC test.xml)

#ADD_CUSTOM_TARGET(Generate ALL DEPENDS FSM)
#if ( FSM_SRC )
macro (COMPONENT_GENERATE outfiles)
  foreach( it ${ARGN})
   get_filename_component( it ${it} ABSOLUTE )
   get_filename_component( component ${it} NAME_WE )
#   set(outfile ${CMAKE_CURRENT_BINARY_DIR}/code/I${outfile}.h ${CMAKE_CURRENT_BINARY_DIR}/code/${outfile}.h ${CMAKE_CURRENT_BINARY_DIR}/code/${outfile}StateBase.h ${CMAKE_CURRENT_BINARY_DIR}/code/I${outfile}.h${CMAKE_CURRENT_BINARY_DIR}/code/I${outfile}State.h)
   set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/I${component}Comp.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Comp.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}CompImpl.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}CompImpl.cpp
   )

   set(fileCode )

#   set(fileCode YaCompTimer.h
#                YaCompTimer.cpp
#                YaCompEvent.h
#                YaCompEvent.cpp
#   )
   set(fileIfc )

#   set(fileIfc  IYaCompTimer.h
#                IYaCompTimerCB.h
#                IYaCompEvent.h
#                IYaCompEventCB.h
#   )

  add_custom_command( OUTPUT ${outfile}
      #COMMAND perl -I${YaFSM_GENERATOR_SOURCE_DIR} -f ${YaFSM_GENERATOR_SOURCE_DIR}/YaFsm.pl --fsm=${it}  --gendot --gendsc --gencode --verbose --outdot=${CMAKE_CURRENT_BINARY_DIR}/${fsm}/dot --outdsc=${CMAKE_CURRENT_BINARY_DIR}/${fsm}/dsc --outcode=${CMAKE_CURRENT_BINARY_DIR}/${fsm}/code
      COMMAND perl -I${YaComponent_GENERATOR_SOURCE_DIR} -f ${YaComponent_GENERATOR_SOURCE_DIR}/YaComponent.pl --comp=${it}  --genview --gencode --outview=${CMAKE_CURRENT_BINARY_DIR}/${component}/view --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code
      #ARGS -o ${outfile} ${it}
      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} )
      #COMMENT "Generating FSM ${FSM_SRC}"
    foreach ( file ${fileCode} )
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${file}
      COMMAND ${CMAKE_COMMAND} -E copy ${YaComponent_GENERATOR_SOURCE_DIR}/codeimpl/cpp/qt4/${file}  ${CMAKE_CURRENT_BINARY_DIR}/${component}/code
      DEPENDS ${YaComponent_GENERATOR_SOURCE_DIR}/codeimpl/cpp/qt4/${file}
    )
    set( ${outfiles} ${${outfiles}} ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${file})
    endforeach( file )

    foreach ( fileI ${fileIfc} )
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${fileI}
      COMMAND ${CMAKE_COMMAND} -E copy ${YaComponent_GENERATOR_SOURCE_DIR}/codeimpl/cpp/inc/${fileI}  ${CMAKE_CURRENT_BINARY_DIR}/${component}/code
      DEPENDS ${YaComponent_GENERATOR_SOURCE_DIR}/codeimpl/cpp/inc/${fileI}
    )
    set( ${outfiles} ${${outfiles}} ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${fileI})
    endforeach( fileI )

  set( ${outfiles} ${${outfiles}} ${outfile})
  INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${component}/code )
#  QT4_WRAP_CPP( GENERATED_FSM_SRC_MOC_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/YaCompTimer.h ${CMAKE_CURRENT_BINARY_DIR}/${cmponent}/code/YaCompEvent.h)
 endforeach( it )
endmacro( COMPONENT_GENERATE )
#endif( FSM_SRC )


#add_custom_target( GenerateFSM ALL
#  DEPENDS ${FSM}
#  )

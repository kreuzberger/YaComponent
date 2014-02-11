#add_subdirectory(images)
project(YaComponent_GENERATOR)


macro (IFC_GENERATE outfiles)
  foreach( it ${ARGN})
    get_filename_component( it ${it} ABSOLUTE )
    get_filename_component( component ${it} NAME_WE )
#   set(outfile ${CMAKE_CURRENT_BINARY_DIR}/code/I${outfile}.h ${CMAKE_CURRENT_BINARY_DIR}/code/${outfile}.h ${CMAKE_CURRENT_BINARY_DIR}/code/${outfile}StateBase.h ${CMAKE_CURRENT_BINARY_DIR}/code/I${outfile}.h${CMAKE_CURRENT_BINARY_DIR}/code/I${outfile}State.h)

    set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/I${component}Comp.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}CompProxy.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}CompProxy.cpp
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}CompStub.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}CompStub.cpp
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Impl.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Impl.cpp
    )

    add_custom_command( OUTPUT ${outfile}
      #COMMAND echo perl -I${YaComponent_GENERATOR_SOURCE_DIR} -f ${YaComponent_GENERATOR_SOURCE_DIR}/YaComponent.pl --ifc=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code
      COMMAND perl -I${YaComponent_GENERATOR_SOURCE_DIR} -f ${YaComponent_GENERATOR_SOURCE_DIR}/YaComponent.pl --ifc=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code --verbose
      #ARGS -o ${outfile} ${it}
      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} )

      QT4_WRAP_CPP(blablub ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Impl.h)


#      QT4_AUTOMOC( ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Impl.cpp)
#    set( ${outfiles} ${${outfiles}} ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${file})

    set( ${outfiles} ${${outfiles}} ${outfile})
    INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${component}/code )
  endforeach( it )
endmacro( IFC_GENERATE )


macro (COMPLAYOUT_GENERATE outfiles)
  foreach( it ${ARGN})
   get_filename_component( it ${it} ABSOLUTE )
   get_filename_component( component ${it} NAME_WE )

   set(outfile)
   set(outfile #${CMAKE_CURRENT_BINARY_DIR}/${component}/code/I${component}CompLayout.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}CompLayout.h
               #${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}CompLayout.cpp
   )

   set (fileCode )

#   set(fileCode  MainPre.cpp
#                 MainPost.cpp
#   )

   set(fileIfc  YaComponent.h
#                IYaCompTimerCB.h
#                IYaCompEvent.h
#                IYaCompEventCB.h
   )

  add_custom_command( OUTPUT ${outfile}
      #COMMAND echo perl -I${YaComponent_GENERATOR_SOURCE_DIR} -f ${YaComponent_GENERATOR_SOURCE_DIR}/YaComponent.pl --ifc=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code
      COMMAND perl -I${YaComponent_GENERATOR_SOURCE_DIR} -f ${YaComponent_GENERATOR_SOURCE_DIR}/YaCompLayout.pl --comp=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code --verbose
      #ARGS -o ${outfile} ${it}
      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} )
      #COMMENT "Generating FSM ${FSM_SRC}"
    foreach ( file ${fileCode} )
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${file}
      COMMAND ${CMAKE_COMMAND} -E copy ${YaComponent_GENERATOR_SOURCE_DIR}/code/cpp/${file}  ${CMAKE_CURRENT_BINARY_DIR}/${component}/code
      DEPENDS ${YaComponent_GENERATOR_SOURCE_DIR}/code/cpp/${file}
    )
    set( ${outfiles} ${${outfiles}} ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${file})
    endforeach( file )

    foreach ( fileI ${fileIfc} )
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${fileI}
      COMMAND ${CMAKE_COMMAND} -E copy ${YaComponent_GENERATOR_SOURCE_DIR}/code/inc/${fileI}  ${CMAKE_CURRENT_BINARY_DIR}/${component}/code
      DEPENDS ${YaComponent_GENERATOR_SOURCE_DIR}/code/inc/${fileI}
    )
    set( ${outfiles} ${${outfiles}} ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${fileI})
    endforeach( fileI )

    set( ${outfiles} ${${outfiles}} ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${fileI})

  set( ${outfiles} ${${outfiles}} ${outfile})
  INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${component}/code )
  #QT4_WRAP_CPP( GENERATED_COMPLAYOUT_SRC_MOC_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/YaComponent.h )
 endforeach( it )
endmacro( COMPLAYOUT_GENERATE )



#add_subdirectory(images)
project(YaComponent_GENERATOR)

macro (YACOMPONENT_IFC_GENERATE outfiles)
  foreach( it ${ARGN})
    get_filename_component( it ${it} ABSOLUTE )
    get_filename_component( component ${it} NAME_WE )

    set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Proxy.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Proxy.cpp
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Stub.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Stub.cpp
    )

    add_custom_command( OUTPUT ${outfile}
      COMMAND perl -I${YaComponent_GENERATOR_SOURCE_DIR} -f ${YaComponent_GENERATOR_SOURCE_DIR}/YaComponent.pl --ifc=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code --rootpath=${CMAKE_SOURCE_DIR} --verbose
      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR}
    )

    QT4_WRAP_CPP(blablub ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Stub.h
                         ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Proxy.h
    )

    set( ${outfiles} ${${outfiles}} ${outfile})
    INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${component}/code )
  endforeach( it )
endmacro( YACOMPONENT_IFC_GENERATE )



macro (YACOMPONENT_GENERATE outfiles)
  foreach( it ${ARGN})
    get_filename_component( it ${it} ABSOLUTE )
    get_filename_component( component ${it} NAME_WE )

    set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/I${component}Comp.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Proxy.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Proxy.cpp
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Stub.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Stub.cpp
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Impl.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Impl.cpp
    )

    add_custom_command( OUTPUT ${outfile}
      COMMAND perl -I${YaComponent_GENERATOR_SOURCE_DIR} -f ${YaComponent_GENERATOR_SOURCE_DIR}/YaComponent.pl --comp=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code --rootpath=${CMAKE_SOURCE_DIR} --verbose
      #ARGS -o ${outfile} ${it}
      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR}
    )

    QT4_WRAP_CPP(blablub ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Stub.h
                         ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Proxy.h
                         ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Impl.h
    )

    set( ${outfiles} ${${outfiles}} ${outfile})
    INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${component}/code )
  endforeach( it )
endmacro( YACOMPONENT_GENERATE )


macro (YACOMPONENT_LAYOUT_GENERATE outfiles)
  foreach( it ${ARGN})
   get_filename_component( it ${it} ABSOLUTE )
   get_filename_component( component ${it} NAME_WE )

   set(outfile #${CMAKE_CURRENT_BINARY_DIR}/${component}/code/I${component}CompLayout.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}CompLayout.h
               #${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}CompLayout.cpp
   )


  add_custom_command( OUTPUT ${outfile}
      COMMAND perl -I${YaComponent_GENERATOR_SOURCE_DIR} -f ${YaComponent_GENERATOR_SOURCE_DIR}/YaCompLayout.pl --layout=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code
      #ARGS -o ${outfile} ${it}
      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR}
  )
      #COMMENT "Generating FSM ${FSM_SRC}"
#    foreach ( file ${fileCode} )
#    add_custom_command(
#      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${file}
#      COMMAND ${CMAKE_COMMAND} -E copy ${YaComponent_GENERATOR_SOURCE_DIR}/code/cpp/${file}  ${CMAKE_CURRENT_BINARY_DIR}/${component}/code
#      DEPENDS ${YaComponent_GENERATOR_SOURCE_DIR}/code/cpp/${file}
#    )
#    set( ${outfiles} ${${outfiles}} ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${file})
#    endforeach( file )

#    foreach ( fileI ${fileIfc} )
#      add_custom_command(
#      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${fileI}
#      COMMAND ${CMAKE_COMMAND} -E copy ${YaComponent_GENERATOR_SOURCE_DIR}/code/inc/${fileI}  ${CMAKE_CURRENT_BINARY_DIR}/${component}/code
#      DEPENDS ${YaComponent_GENERATOR_SOURCE_DIR}/code/inc/${fileI}
#    )
#    set( ${outfiles} ${${outfiles}} ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${fileI})
#    endforeach( fileI )

#    set( ${outfiles} ${${outfiles}} ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${fileI})

  set( ${outfiles} ${${outfiles}} ${outfile})
  INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${component}/code )
  #QT4_WRAP_CPP( GENERATED_COMPLAYOUT_SRC_MOC_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/YaComponent.h )
 endforeach( it )
endmacro( YACOMPONENT_LAYOUT_GENERATE )



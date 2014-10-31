#add_subdirectory(images)
project(YaComponent_GENERATOR)

macro (YACOMPONENT_IFC_GENERATE outfiles)
  set( outfiles )
  foreach( it ${ARGN})
    get_filename_component( it ${it} ABSOLUTE )
    get_filename_component( component ${it} NAME_WE )

    set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Proxy.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Proxy.cpp
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Stub.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Stub.cpp
    )

    add_custom_command( OUTPUT ${outfile}
      COMMAND perl -I${YaComponent_GENERATOR_SOURCE_DIR} -f ${YaComponent_GENERATOR_SOURCE_DIR}/YaComponent.pl --ifc=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code --outdoc=${CMAKE_CURRENT_BINARY_DIR}/${component}/doc --rootpath=${CMAKE_SOURCE_DIR} --verbose
      COMMAND ${Java_JAVA_EXECUTABLE} -Dplantuml.include.path=${CMAKE_CURRENT_BINARY_DIR} -jar ${PLANTUML_JAR} -tpng ${CMAKE_CURRENT_BINARY_DIR}/${component}/doc/${component}.txt

      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR}
    )

    set( ${outfiles} ${${outfiles}} ${outfile}  )

    QT4_WRAP_CPP(outfile_mocs ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Stub.h
                             ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Proxy.h
    )

    set( ${outfiles} ${${outfiles}} ${outfile_mocs}  )

    #message(" resulting outfiles in if generation ${${outfiles}}")
    INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${component}/code )
    INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )
  endforeach( it )
endmacro( YACOMPONENT_IFC_GENERATE )



macro (YACOMPONENT_GENERATE outfiles)
  foreach( it ${ARGN})
    get_filename_component( it ${it} ABSOLUTE )
    get_filename_component( component ${it} NAME_WE )

    set(outfile #${CMAKE_CURRENT_BINARY_DIR}/${component}/code/I${component}Comp.h
              # ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Proxy.h
              # ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Proxy.cpp
              # ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Stub.h
              # ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Stub.cpp
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Impl.h
               ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Impl.cpp
    )

    add_custom_command( OUTPUT ${outfile}
      COMMAND perl -I${YaComponent_GENERATOR_SOURCE_DIR} -f ${YaComponent_GENERATOR_SOURCE_DIR}/YaComponent.pl --comp=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code --outdoc=${CMAKE_CURRENT_BINARY_DIR}/${component}/doc --rootpath=${CMAKE_SOURCE_DIR} --verbose
      COMMAND echo ${Java_JAVA_EXECUTABLE} -Dplantuml.include.path=${CMAKE_CURRENT_BINARY_DIR} -jar ${PLANTUML_JAR} -tpng ${CMAKE_CURRENT_BINARY_DIR}/${component}/doc/${component}.txt
      COMMAND ${Java_JAVA_EXECUTABLE} -Dplantuml.include.path=${CMAKE_CURRENT_BINARY_DIR} -jar ${PLANTUML_JAR} -tpng ${CMAKE_CURRENT_BINARY_DIR}/${component}/doc/${component}.txt
      #ARGS -o ${outfile} ${it}
      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR}
    )

    QT4_WRAP_CPP(blablub #${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Stub.h
                         #${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Proxy.h
                         ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Impl.h
    )

    set( ${outfiles} ${${outfiles}} ${outfile})
    INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${component}/code )
  endforeach( it )

  set(yacodefiles YaComponent.h
                  YaComponent.cpp
                  YaPUBImpl.h
                  YaPUBImpl.cpp
                  YaSUBImpl.h
                  YaSUBImpl.cpp
                  YaBuffer.h
                  YaBuffer.cpp
  )

  foreach( file ${yacodefiles} )
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${file}
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/cpp/${file} ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${file}
      DEPENDS ${CMAKE_SOURCE_DIR}/src/cpp/${file}
    )
    set( ${outfiles} ${${outfiles}} ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${file})
  endforeach()
endmacro( YACOMPONENT_GENERATE )


macro (YACOMPONENT_LAYOUT_GENERATE outfiles)
  foreach( it ${ARGN})
    get_filename_component( it ${it} ABSOLUTE )
    get_filename_component( layout ${it} NAME_WE )

    set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${layout}/code/${layout}.h
    )

    get_property(inc_dirs DIRECTORY ${YaComponent_GENERATOR_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    message("include dirs ${inc_dirs}")

    add_custom_command( OUTPUT ${outfile}
      COMMAND perl -I${YaComponent_GENERATOR_SOURCE_DIR} -f ${YaComponent_GENERATOR_SOURCE_DIR}/YaCompLayout.pl --layout=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${layout}/code --outdoc=${CMAKE_CURRENT_BINARY_DIR}/${layout}/doc
      COMMAND echo ${Java_JAVA_EXECUTABLE} -Dplantuml.include.path=${CMAKE_CURRENT_BINARY_DIR} -jar ${PLANTUML_JAR} -tpng ${CMAKE_CURRENT_BINARY_DIR}/${layout}/doc/${layout}.txt
      COMMAND ${Java_JAVA_EXECUTABLE} -Dplantuml.include.path=${CMAKE_CURRENT_BINARY_DIR} -jar ${PLANTUML_JAR} -tpng ${CMAKE_CURRENT_BINARY_DIR}/${layout}/doc/${layout}.txt
      #ARGS -o ${outfile} ${it}
      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR}
    )

   set( ${outfiles} ${${outfiles}} ${outfile})
   INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${layout}/code )
   #QT4_WRAP_CPP( GENERATED_COMPLAYOUT_SRC_MOC_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/YaComponent.h )
 endforeach( it )
endmacro( YACOMPONENT_LAYOUT_GENERATE )



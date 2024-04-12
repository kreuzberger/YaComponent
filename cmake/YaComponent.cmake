#[=======================================================================[.rst:

YaComponent
************

Script to add Yet Another Component xml definitions to the build process.

.. command:: yacomponent_ifc_generate

  Macro to generate source code files from component xml definition

  ::

    yacomponent_ifc_generate( outfiles
                        <xml input files>
    )

  The options are:

    ``outfiles``
      The generated cpp files that could be added to a target.

    ``Component xml``
      Full path to the XML file describing the Component(s)

#]=======================================================================]


include(CMakeParseArguments)

macro (YACOMPONENT_IFC_GENERATE outfiles)

   set( outfiles )

  foreach( it ${ARGN})
    get_filename_component( it ${it} ABSOLUTE )
    get_filename_component( component ${it} NAME_WE )

    set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Proxy.h
                ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Proxy.cpp
                ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/I${component}ProxyHandler.h
                ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Stub.h
                ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Stub.cpp
    )
   # message(" adding outfile in if generation ${outfile}")


    add_custom_command( OUTPUT ${outfile}
      COMMAND $<TARGET_FILE:YaComponent::yacomponent> --ifc=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code --outdoc=${CMAKE_CURRENT_BINARY_DIR}/${component}/doc --verbose
      #COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${component}/codePerl
      #COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${component}/docPerl
      #COMMAND perl -I${YaComponent_GENERATOR_SOURCE_DIR} -f ${YaComponent_GENERATOR_SOURCE_DIR}/YaComponent.pl --ifc=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/codePerl --outdoc=${CMAKE_CURRENT_BINARY_DIR}/${component}/docPerl --rootpath=${CMAKE_SOURCE_DIR} --verbose
      COMMAND ${Java_JAVA_EXECUTABLE} -Dplantuml.include.path=${CMAKE_CURRENT_BINARY_DIR} -jar ${PLANTUML_JAR} -tpng ${CMAKE_CURRENT_BINARY_DIR}/${component}/doc/${component}.txt

      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} $<TARGET_FILE:YaComponent::yacomponent>
    )

    set( ${outfiles} ${${outfiles}} ${outfile} )

    INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${component}/code )
  endforeach( it )

  INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )
  INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/code )

  set( ${outfiles} ${${outfiles}}  )

  set_property( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${${outfiles}}" )

endmacro( YACOMPONENT_IFC_GENERATE )



macro (YACOMPONENT_GENERATE outfiles)
  foreach( it ${ARGN})
    get_filename_component( it ${it} ABSOLUTE )
    get_filename_component( component ${it} NAME_WE )

    set(outfile "${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Impl.h"
               "${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Impl.cpp"
    )

    add_custom_command( OUTPUT ${outfile}
      COMMAND $<TARGET_FILE:YaComponent::yacomponent> --component=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code --outdoc=${CMAKE_CURRENT_BINARY_DIR}/${component}/doc --verbose
      #COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${component}/docPerl
      #COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${component}/codePerl
      #COMMAND perl -I${YaComponent_GENERATOR_SOURCE_DIR} -f ${YaComponent_GENERATOR_SOURCE_DIR}/YaComponent.pl --comp=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/codePerl --outdoc=${CMAKE_CURRENT_BINARY_DIR}/${component}/docPerl --rootpath=${CMAKE_SOURCE_DIR} --verbose
      COMMAND echo ${Java_JAVA_EXECUTABLE} -Dplantuml.include.path=${CMAKE_CURRENT_BINARY_DIR} -jar ${PLANTUML_JAR} -tpng ${CMAKE_CURRENT_BINARY_DIR}/${component}/doc/${component}.txt
      COMMAND ${Java_JAVA_EXECUTABLE} -Dplantuml.include.path=${CMAKE_CURRENT_BINARY_DIR} -jar ${PLANTUML_JAR} -tpng ${CMAKE_CURRENT_BINARY_DIR}/${component}/doc/${component}.txt
      #ARGS -o ${outfile} ${it}
      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} $<TARGET_FILE:YaComponent::yacomponent>
    )

    set( ${outfiles} ${${outfiles}} ${outfile})

    INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${component}/code )
  endforeach( it )

  set_property( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${${outfiles}}" )
endmacro( YACOMPONENT_GENERATE )


macro (YACOMPONENT_LAYOUT_GENERATE outfiles)
  foreach( it ${ARGN})
    get_filename_component( it ${it} ABSOLUTE )
    get_filename_component( layout ${it} NAME_WE )

    set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${layout}/doc/${layout}.txt
    )

    get_property(inc_dirs DIRECTORY ${YaComponent_GENERATOR_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#    message("include dirs ${inc_dirs}")

    add_custom_command( OUTPUT ${outfile}
      COMMAND $<TARGET_FILE:YaComponent::yacomplayout> --layout=${it} --outcode=${CMAKE_CURRENT_BINARY_DIR}/${layout}/code
      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} $<TARGET_FILE:YaComponent::yacomplayout>
    )

   set( ${outfiles} ${${outfiles}} ${outfile})
   INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${layout}/code )
  endforeach( it )

  set( ${outfiles} ${${outfiles}}  )

  set_property( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${${outfiles}}" )

endmacro( YACOMPONENT_LAYOUT_GENERATE )



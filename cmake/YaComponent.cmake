#[=======================================================================[.rst:

YaComponent
************

Script to generate example source code for layouts

.. command:: yacomponent_layout_generate

  Macro to generate source code files from layout xml definition

  ::

    yacomponent_layout_generate( outfiles
                        <xml input files>
    )

  The options are:

    ``Layout xml``
      Full path to the XML file describing the Layout(s)

#]=======================================================================]


include(CMakeParseArguments)

macro (YACOMPONENT_LAYOUT_GENERATE)
  foreach( it ${ARGN})
    get_filename_component( it ${it} ABSOLUTE )
    get_filename_component( layout ${it} NAME_WE )

    set(_outfile ${CMAKE_CURRENT_BINARY_DIR}/${layout}/code/${layout}.cpp
    )

    get_property(inc_dirs DIRECTORY ${YaComponent_GENERATOR_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#    message("include dirs ${inc_dirs}")

    add_custom_command( OUTPUT ${_outfile}
      COMMAND $<TARGET_FILE:YaComponent::yacomplayout> --layout=${it} --outcode=${CMAKE_CURRENT_BINARY_DIR}/${layout}/code
      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} $<TARGET_FILE:YaComponent::yacomplayout>
    )

    list( APPEND _outfiles ${_outfile})
    INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${layout}/code )
  endforeach( it )

  add_custom_target(layouts_${PROJECT_NAME} ALL DEPENDS ${_outfiles}
                    COMMENT "Generating layout for ${PROJECT_NAME}")

  set_property( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${_outfiles}" )

endmacro( YACOMPONENT_LAYOUT_GENERATE )


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
                ${CMAKE_CURRENT_BINARY_DIR}/${component}/doc/${component}.uml
    )
   # message(" adding outfile in if generation ${outfile}")


    add_custom_command( OUTPUT ${outfile}
      COMMAND $<TARGET_FILE:YaComponent::yacomponent> --ifc=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code --outdoc=${CMAKE_CURRENT_BINARY_DIR}/${component}/doc --verbose

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
               "${CMAKE_CURRENT_BINARY_DIR}/${component}/doc/${component}.uml"
    )

    add_custom_command( OUTPUT ${outfile}
      COMMAND $<TARGET_FILE:YaComponent::yacomponent> --component=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code --outdoc=${CMAKE_CURRENT_BINARY_DIR}/${component}/doc --verbose
      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} $<TARGET_FILE:YaComponent::yacomponent>
    )

    set( ${outfiles} ${${outfiles}} ${outfile})

    INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${component}/code )
  endforeach( it )

  set_property( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${${outfiles}}" )
endmacro( YACOMPONENT_GENERATE )




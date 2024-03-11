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

  set( fileCodeIfc
       YaPUBImpl.h
       YaPUBImpl.cpp
       YaSUBImpl.h
       YaSUBImpl.cpp
       YaComponent.cpp
       YaProxyBase.h
       YaProxyBase.cpp
       YaStubBase.h
       YaStubBase.cpp
       YaBuffer.h
       YaBuffer.cpp
       YaComponent.h
       YaComponent.cpp
#      IYaConnection.h
 )

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
      COMMAND $<TARGET_FILE:yacomponent> --ifc=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code --verbose
      #COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${component}/codePerl
      #COMMAND perl -I${YaComponent_GENERATOR_SOURCE_DIR} -f ${YaComponent_GENERATOR_SOURCE_DIR}/YaComponent.pl --ifc=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/codePerl --outdoc=${CMAKE_CURRENT_BINARY_DIR}/${component}/doc --rootpath=${CMAKE_SOURCE_DIR} --verbose
      COMMAND ${Java_JAVA_EXECUTABLE} -Dplantuml.include.path=${CMAKE_CURRENT_BINARY_DIR} -jar ${PLANTUML_JAR} -tpng ${CMAKE_CURRENT_BINARY_DIR}/${component}/doc/${component}.txt

      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} $<TARGET_FILE:yacomponent>
    )

    set( ${outfiles} ${${outfiles}} ${outfile} )

    #QT5_WRAP_CPP(outfile_component_mocs ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Stub.h )
    #set( ${outfiles} ${${outfiles}} ${outfile_component_mocs} )
    set( ${outfiles} ${${outfiles}} )



    #message(" resulting outfiles in if generation ${${outfiles}}")
    INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${component}/code )
  endforeach( it )

  INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )
  INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/code )

  foreach ( file ${fileCodeIfc} )
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/code/${file}
      COMMAND ${CMAKE_COMMAND} -E copy ${YaComponentLib_SOURCE_DIR}/${file}  ${CMAKE_CURRENT_BINARY_DIR}/code/
      DEPENDS ${YaComponentLib_SOURCE_DIR}/${file}
    )
    set( ${outfiles} ${${outfiles}} ${CMAKE_CURRENT_BINARY_DIR}/code/${file} )
  endforeach( file )

  #QT5_WRAP_CPP(outfile_mocs ${CMAKE_CURRENT_BINARY_DIR}/code/YaProxyBase.h ${CMAKE_CURRENT_BINARY_DIR}/code/YaStubBase.h)
  #set( ${outfiles} ${${outfiles}} ${outfile_mocs}  )
  set( ${outfiles} ${${outfiles}}  )

  set_property( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${${outfiles}}" )

endmacro( YACOMPONENT_IFC_GENERATE )



macro (YACOMPONENT_GENERATE outfiles)
  set( fileCodeComp
   #    YaComponentBase.h
   #    YaComponentBase.cpp
  )
  foreach( it ${ARGN})
    get_filename_component( it ${it} ABSOLUTE )
    get_filename_component( component ${it} NAME_WE )

    set(outfile "${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Impl.h"
               "${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Impl.cpp"
    )

    add_custom_command( OUTPUT ${outfile}
      COMMAND $<TARGET_FILE:yacomponent> --component=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/code --verbose
      #COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${component}/codePerl
      #COMMAND perl -I${YaComponent_GENERATOR_SOURCE_DIR} -f ${YaComponent_GENERATOR_SOURCE_DIR}/YaComponent.pl --comp=${it}  --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/codePerl --outdoc=${CMAKE_CURRENT_BINARY_DIR}/${component}/doc --rootpath=${CMAKE_SOURCE_DIR} --verbose
      COMMAND echo ${Java_JAVA_EXECUTABLE} -Dplantuml.include.path=${CMAKE_CURRENT_BINARY_DIR} -jar ${PLANTUML_JAR} -tpng ${CMAKE_CURRENT_BINARY_DIR}/${component}/doc/${component}.txt
      COMMAND ${Java_JAVA_EXECUTABLE} -Dplantuml.include.path=${CMAKE_CURRENT_BINARY_DIR} -jar ${PLANTUML_JAR} -tpng ${CMAKE_CURRENT_BINARY_DIR}/${component}/doc/${component}.txt
      #ARGS -o ${outfile} ${it}
      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} $<TARGET_FILE:yacomponent>
    )

    set( ${outfiles} ${${outfiles}} ${outfile})

    #QT5_WRAP_CPP(outfiles_moc
    #                     ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/${component}Impl.h
    #)

    #set( ${outfiles} ${${outfiles}} ${outfiles_moc})
    INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${component}/code )
  endforeach( it )

  foreach ( file ${fileCodeComp} )
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/code/${file}
      COMMAND ${CMAKE_COMMAND} -E copy ${YaComponentLib_SOURCE_DIR}/${file}  ${CMAKE_CURRENT_BINARY_DIR}/code/
      DEPENDS ${YaComponentLib_SOURCE_DIR}/${file}
    )
    set( ${outfiles} ${${outfiles}} ${CMAKE_CURRENT_BINARY_DIR}/code/${file} )
  endforeach( file )

    #  QT5_WRAP_CPP(outfile_mocs ${CMAKE_CURRENT_BINARY_DIR}/code/YaComponentBase.h )
    #set( ${outfiles} ${${outfiles}} ${outfile_mocs}  )
    set( ${outfiles} ${${outfiles}} )

  set_property( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${${outfiles}}" )




endmacro( YACOMPONENT_GENERATE )


macro (YACOMPONENT_LAYOUT_GENERATE outfiles)
  set( fileCodeLayout
       YaComponentThread.h
       YaComponentThread.cpp
  )


  foreach( it ${ARGN})
    get_filename_component( it ${it} ABSOLUTE )
    get_filename_component( layout ${it} NAME_WE )

    set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${layout}/doc/${layout}.txt
    )

    get_property(inc_dirs DIRECTORY ${YaComponent_GENERATOR_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#    message("include dirs ${inc_dirs}")

    add_custom_command( OUTPUT ${outfile}
      COMMAND $<TARGET_FILE:yacomplayout> --layout=${it} --outcode=${CMAKE_CURRENT_BINARY_DIR}/${layout}/code
      DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} $<TARGET_FILE:yacomplayout>
    )

   set( ${outfiles} ${${outfiles}} ${outfile})
   INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/${layout}/code )
   #QT4_WRAP_CPP( GENERATED_COMPLAYOUT_SRC_MOC_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${component}/code/YaComponent.h )
  endforeach( it )

  foreach( file ${fileCodeLayout} )
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/code/${file}
      COMMAND ${CMAKE_COMMAND} -E copy ${YaComponentLib_SOURCE_DIR}/${file}  ${CMAKE_CURRENT_BINARY_DIR}/code/
      DEPENDS ${YaComponentLib_SOURCE_DIR}/${file}
    )
    set( ${outfiles} ${${outfiles}} ${CMAKE_CURRENT_BINARY_DIR}/code/${file} )
  endforeach( file )

  #QT5_WRAP_CPP(outfile_mocs ${CMAKE_CURRENT_BINARY_DIR}/code/YaComponentThread.h)
  #set( ${outfiles} ${${outfiles}} ${outfile_mocs}  )
  set( ${outfiles} ${${outfiles}}  )

  set_property( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${${outfiles}}" )

endmacro( YACOMPONENT_LAYOUT_GENERATE )



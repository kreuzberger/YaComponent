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

macro(YACOMPONENT_LAYOUT_GENERATE)
    foreach(it ${ARGN})
        get_filename_component(it ${it} ABSOLUTE)
        get_filename_component(layout ${it} NAME_WE)

        set(_outfile ${CMAKE_CURRENT_BINARY_DIR}/${layout}/cpp/${layout}.cpp)

        get_property(
            inc_dirs
            DIRECTORY ${YaComponent_GENERATOR_SOURCE_DIR}
            PROPERTY INCLUDE_DIRECTORIES
        )
        # message("include dirs ${inc_dirs}")

        add_custom_command(
            OUTPUT ${_outfile}
            COMMAND $<TARGET_FILE:YaComponent::yacomplayout> --layout=${it}
                    --outcode=${CMAKE_CURRENT_BINARY_DIR}/${layout}/cpp
            COMMAND $<TARGET_FILE:YaComponent::yacomplayout> --layout=${it} --language=python
                    --outcode=${CMAKE_CURRENT_BINARY_DIR}/${layout}/py
            DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} $<TARGET_FILE:YaComponent::yacomplayout>
        )

        list(APPEND _outfiles ${_outfile})
        include_directories(${CMAKE_CURRENT_BINARY_DIR}/${layout}/cpp)
    endforeach(it)

    add_custom_target(
        layouts_${PROJECT_NAME} ALL
        DEPENDS ${_outfiles}
        COMMENT "Generating layout for ${PROJECT_NAME}"
    )

    set_property(
        DIRECTORY
        APPEND
        PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${_outfiles}"
    )

endmacro(YACOMPONENT_LAYOUT_GENERATE)

macro(YACOMPONENT_IFC_GENERATE outfiles)

    set(options)
    set(oneValueArgs LANGUAGE)
    set(multiValueArgs)
    cmake_parse_arguments(args "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(outfiles)

    # message("args_UNPARSED_ARGUMENTS ${args_UNPARSED_ARGUMENTS}") message("args_LANGUAGE ${args_LANGUAGE}")

    foreach(it ${args_UNPARSED_ARGUMENTS})
        get_filename_component(it ${it} ABSOLUTE)
        get_filename_component(component ${it} NAME_WE)

        if("${args_LANGUAGE}" STREQUAL "python")
            set(outfile
                ${CMAKE_CURRENT_BINARY_DIR}/${component}/py/${component}Proxy.py
                # ${CMAKE_CURRENT_BINARY_DIR}/${component}/py/I${component}ProxyHandler.h
                ${CMAKE_CURRENT_BINARY_DIR}/${component}/py/${component}Stub.py
                ${CMAKE_CURRENT_BINARY_DIR}/${component}/doc/${component}.uml
            )
            # message(" adding outfile in if generation ${outfile}")
            add_custom_command(
                OUTPUT ${outfile}
                COMMAND
                    $<TARGET_FILE:YaComponent::yacomponent> --ifc=${it} --language=python
                    --outdoc=${CMAKE_CURRENT_BINARY_DIR}/${component}/doc --verbose
                    --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/py
                DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} $<TARGET_FILE:YaComponent::yacomponent>
            )

        else()
            set(outfile
                ${CMAKE_CURRENT_BINARY_DIR}/${component}/cpp/${component}Proxy.h
                ${CMAKE_CURRENT_BINARY_DIR}/${component}/cpp/${component}Proxy.cpp
                ${CMAKE_CURRENT_BINARY_DIR}/${component}/cpp/I${component}ProxyHandler.h
                ${CMAKE_CURRENT_BINARY_DIR}/${component}/cpp/${component}Stub.h
                ${CMAKE_CURRENT_BINARY_DIR}/${component}/cpp/${component}Stub.cpp
                ${CMAKE_CURRENT_BINARY_DIR}/${component}/doc/${component}.uml
            )
            # message(" adding outfile in if generation ${outfile}")

            add_custom_command(
                OUTPUT ${outfile}
                COMMAND
                    $<TARGET_FILE:YaComponent::yacomponent> --ifc=${it}
                    --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/cpp
                    --outdoc=${CMAKE_CURRENT_BINARY_DIR}/${component}/doc --verbose
                DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} $<TARGET_FILE:YaComponent::yacomponent>
            )
            include_directories(${CMAKE_CURRENT_BINARY_DIR}/${component}/cpp)
        endif()
        set(${outfiles} ${${outfiles}} ${outfile})

    endforeach(it)

    include_directories(${CMAKE_CURRENT_BINARY_DIR})
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/cpp)

    set(${outfiles} ${${outfiles}})

    set_property(
        DIRECTORY
        APPEND
        PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${${outfiles}}"
    )

endmacro(YACOMPONENT_IFC_GENERATE)

macro(YACOMPONENT_GENERATE outfiles)
    set(options)
    set(oneValueArgs LANGUAGE)
    set(multiValueArgs)
    cmake_parse_arguments(args "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    foreach(it ${args_UNPARSED_ARGUMENTS})
        get_filename_component(it ${it} ABSOLUTE)
        get_filename_component(component ${it} NAME_WE)

        if("${args_LANGUAGE}" STREQUAL "python")
            set(outfile "${CMAKE_CURRENT_BINARY_DIR}/${component}/py/${component}Impl.py"
                        "${CMAKE_CURRENT_BINARY_DIR}/${component}/doc/${component}.uml"
            )
            add_custom_command(
                OUTPUT ${outfile}
                COMMAND
                    $<TARGET_FILE:YaComponent::yacomponent> --component=${it}
                    --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/py --language=python
                    --outdoc=${CMAKE_CURRENT_BINARY_DIR}/${component}/doc --verbose
                DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} $<TARGET_FILE:YaComponent::yacomponent>
            )
        else()
            set(outfile
                "${CMAKE_CURRENT_BINARY_DIR}/${component}/cpp/${component}Impl.h"
                "${CMAKE_CURRENT_BINARY_DIR}/${component}/cpp/${component}Impl.cpp"
                "${CMAKE_CURRENT_BINARY_DIR}/${component}/doc/${component}.uml"
            )
            add_custom_command(
                OUTPUT ${outfile}
                COMMAND
                    $<TARGET_FILE:YaComponent::yacomponent> --component=${it}
                    --outcode=${CMAKE_CURRENT_BINARY_DIR}/${component}/cpp
                    --outdoc=${CMAKE_CURRENT_BINARY_DIR}/${component}/doc --verbose
                DEPENDS ${it} ${YaComponent_GENERATOR_SOURCE_DIR} $<TARGET_FILE:YaComponent::yacomponent>
            )
            include_directories(${CMAKE_CURRENT_BINARY_DIR}/${component}/cpp)
        endif()

        set(${outfiles} ${${outfiles}} ${outfile})

    endforeach(it)

    set_property(
        DIRECTORY
        APPEND
        PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${${outfiles}}"
    )
endmacro(YACOMPONENT_GENERATE)

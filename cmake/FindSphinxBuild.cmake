# - Find sphinx-build program
#
#  USE_DOC_SPHINX               - have the sphinx-build command found


set(USE_DOC_SPHINX true)
find_program(SPHINX_BUILD_EXECUTABLE sphinx-build PATH_SUFFIXES bin)
if (NOT SPHINX_BUILD_EXECUTABLE)
  message(STATUS "sphinx-build was not found, the documentation will not be generated.")
  set(USE_DOC_SPHINX false)
endif()
function(sphinx_environment SPHINX_ENV)
    # environment settings for the targets above - TODO: use generator-expressions for path handling (shell)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs PYTHONPATH DEPEND_PATH)

    cmake_parse_arguments(ARGUMENTS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # if( NOT TARGET Python::Interpreter OR NOT TARGET Python::Python ) message( WARNING "missing required python
    # targets") set( ${SPHINX_ENV} PARENT_SCOPE ) return() endif()

    # get_target_property(PYTHON_EXECUTABLE_DIR Python::Interpreter IMPORTED_LOCATION)
    # get_filename_component(PYTHON_EXECUTABLE_DIR "${PYTHON_EXECUTABLE_DIR}" DIRECTORY)

    # set(INSTALL_PATH "${PYTHON_EXECUTABLE_DIR}") if(UNIX) get_filename_component(INSTALL_PATH "${INSTALL_PATH}"
    # DIRECTORY)  # strip /bin endif()

    # get_target_property(DEPEND_PATH Python::Python IMPORTED_LOCATION) get_filename_component(DEPEND_PATH
    # "${DEPEND_PATH}" DIRECTORY)
    list(APPEND DEPEND_PATH ${ARGUMENTS_DEPEND_PATH})

    set(LATEXMK_OPTIONS "-interaction=batchmode -quiet -silent")

    if(UNIX)
        string(REPLACE ";" ":" DEPEND_PATH "${DEPEND_PATH}")
        set(DEPEND_PATH ".:${DEPEND_PATH}")
        string(REPLACE ";" ":" PYTHONPATH "${ARGUMENTS_PYTHONPATH}")
        # "PYTHONHOME=${SPHINX_VENV_DIR}"
        set(${SPHINX_ENV}
            "LD_LIBRARY_PATH=${DEPEND_PATH}" "PATH=${SPHINX_VENV_DIR}/bin:$ENV{PATH}" "PYTHONPATH=${PYTHONPATH}"
            "LATEXMKOPTS=${LATEXMK_OPTIONS}"
            PARENT_SCOPE
        )
    else()
        set(DEPEND_PATH "${DEPEND_PATH};$ENV{PATH}")
        set(PYTHONPATH "${ARGUMENTS_PYTHONPATH}")
        set(SPHINX_SCRIPTS_DIR "${SPHINX_VENV_DIR}/Scripts")

        string(REGEX REPLACE "/" "\\\\" DEPEND_PATH "${DEPEND_PATH}")
        string(REPLACE "\\;" ";" DEPEND_PATH "${DEPEND_PATH}")
        string(REPLACE ";" "\\\;" DEPEND_PATH "${DEPEND_PATH}")
        string(REGEX REPLACE "\\\\\\\\" "\\\\" DEPEND_PATH "${DEPEND_PATH}")

        string(REGEX REPLACE "/" "\\\\" PYTHONPATH "${PYTHONPATH}")
        string(REPLACE "\\;" ";" PYTHONPATH "${PYTHONPATH}")
        string(REPLACE ";" "\\\;" PYTHONPATH "${PYTHONPATH}")

        string(REGEX REPLACE "/" "\\\\" SPHINX_SCRIPTS_DIR "${SPHINX_SCRIPTS_DIR}")
        string(REPLACE "\\;" ";" SPHINX_SCRIPTS_DIR "${SPHINX_SCRIPTS_DIR}")
        string(REPLACE ";" "\\\;" SPHINX_SCRIPTS_DIR "${SPHINX_SCRIPTS_DIR}")

        set(${SPHINX_ENV}
            "PATH=${PYTHONPATH}\;${SPHINX_SCRIPTS_DIR}\;${DEPEND_PATH}" "PYTHONPATH=${PYTHONPATH}"
            "LATEXMKOPTS=${LATEXMK_OPTIONS}"
            PARENT_SCOPE
        )
    endif()
endfunction()

macro(sphinx_doc_generate outtarget)
    set(options DOCTEST EXCLUDE_FROM_ALL)
    set(oneValueArgs CONFIGDIR WORKING_DIRECTORY PROJECT OUTPUT_DIRECTORY)
    set(multiValueArgs
        OPTIONS
        DEPENDS
        PYTHONPATH
        DEPEND_PATH
        RST_FILES
        BUILDERS
        BYPRODUCTS
    )

    cmake_parse_arguments(SPHINX_DOC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT "${SPHINX_DOC_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(
            FATAL_ERROR
                "unparsed arguments in call to sphinx_doc_generate: ${SPHINX_DOC_UNPARSED_ARGUMENTS} from ${CMAKE_CURRENT_LIST_FILE}"
        )
    endif()

    set(SPHINX_CLEAN_DIRECTORIES ON)

    if("${SPHINX_DOC_CONFIGDIR}" STREQUAL "")
        set(SPHINX_DOC_CONFIGDIR "${CMAKE_CURRENT_SOURCE_DIR}")
    endif()

    if("${SPHINX_DOC_WORKING_DIRECTORY}" STREQUAL "")
        set(SPHINX_DOC_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    endif()

    if("${SPHINX_DOC_OUTPUT_DIRECTORY}" STREQUAL "")
        set(SPHINX_DOC_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
    endif()

    if("${SPHINX_DOC_BUILDERS}" STREQUAL "")
        # set( SPHINX_DOC_BUILDERS "HTML" "QTHELP" )
    endif()

    get_filename_component(_outtarget_name ${outtarget} NAME_WE)
    # get_filename_component( _outtarget_dir ${outtarget} DIRECTORY )

    if("${SPHINX_DOC_PROJECT}" STREQUAL "")
        get_filename_component(_project_name ${outtarget} NAME_WE)
    else()
        get_filename_component(_project_name ${SPHINX_DOC_PROJECT} NAME_WE)
    endif()

    sphinx_environment(SPHINX_ENV PYTHONPATH ${SPHINX_DOC_PYTHONPATH} DEPEND_PATH ${SPHINX_DOC_DEPEND_PATH})

    set(_sphinx_options -q -E -W -a)

    if("HTML" IN_LIST SPHINX_DOC_BUILDERS)
        if(SPHINX_CLEAN_DIRECTORIES)
            set(SPHINX_CLEAN_FILES "${SPHINX_DOC_OUTPUT_DIRECTORY}/html")
        endif()

        add_custom_command(
            OUTPUT ${SPHINX_DOC_OUTPUT_DIRECTORY}/html/${_project_name}.html
            COMMAND
                ${CMAKE_COMMAND} -E env ${SPHINX_ENV} ${SPHINX_BUILD_EXECUTABLE}  -c
                "${SPHINX_DOC_CONFIGDIR}" ${_sphinx_options} -b html ${SPHINX_DOC_WORKING_DIRECTORY}
                ${SPHINX_DOC_OUTPUT_DIRECTORY}/html
            DEPENDS ${SPHINX_DOC_DEPENDS} ${SPHINX_DOC_RST_FILES}
            VERBATIM
            COMMENT "Build sphinx html documentation for ${SPHINX_DOC_WORKING_DIRECTORY}"
        )

        list(APPEND SPHINX_DEPENDENCIES ${SPHINX_DOC_OUTPUT_DIRECTORY}/html/${_project_name}.html ${SPHINX_DOC_DEPENDS}
             ${SPHINX_DOC_RST_FILES}
        )
    endif()

    if("SINGLEHTML" IN_LIST SPHINX_DOC_BUILDERS)
        if(SPHINX_CLEAN_DIRECTORIES)
            set(SPHINX_CLEAN_FILES "${SPHINX_DOC_OUTPUT_DIRECTORY}/singlehtml")
        endif()

        add_custom_command(
            OUTPUT ${SPHINX_DOC_OUTPUT_DIRECTORY}/singlehtml/${_project_name}.html
            COMMAND
                ${CMAKE_COMMAND} -E env ${SPHINX_ENV} ${SPHINX_BUILD_EXECUTABLE}  -c
                "${SPHINX_DOC_CONFIGDIR}" ${_sphinx_options} -b singlehtml ${SPHINX_DOC_WORKING_DIRECTORY}
                ${SPHINX_DOC_OUTPUT_DIRECTORY}/singlehtml
            DEPENDS ${SPHINX_DOC_DEPENDS} ${SPHINX_DOC_RST_FILES}
            VERBATIM
            COMMENT "Build sphinx single html documentation for ${SPHINX_DOC_WORKING_DIRECTORY}"
        )

        list(APPEND SPHINX_DEPENDENCIES ${SPHINX_DOC_OUTPUT_DIRECTORY}/singlehtml/${_project_name}.html
             ${SPHINX_DOC_DEPENDS} ${SPHINX_DOC_RST_FILES}
        )
    endif()

    if("REVEALJS" IN_LIST SPHINX_DOC_BUILDERS)
        if(SPHINX_CLEAN_DIRECTORIES)
            set(SPHINX_CLEAN_FILES "${SPHINX_DOC_OUTPUT_DIRECTORY}/revealjs")
        endif()

        add_custom_command(
            OUTPUT ${SPHINX_DOC_OUTPUT_DIRECTORY}/revealjs/${_project_name}.html
            COMMAND
                ${CMAKE_COMMAND} -E env ${SPHINX_ENV} ${SPHINX_BUILD_EXECUTABLE}  -c
                "${SPHINX_DOC_CONFIGDIR}" ${_sphinx_options} -b revealjs ${SPHINX_DOC_WORKING_DIRECTORY}
                ${SPHINX_DOC_OUTPUT_DIRECTORY}/revealjs
            DEPENDS ${SPHINX_DOC_DEPENDS} ${SPHINX_DOC_RST_FILES}
            VERBATIM
            COMMENT "Build sphinx revealjs documentation for ${SPHINX_DOC_WORKING_DIRECTORY}"
        )

        list(APPEND SPHINX_DEPENDENCIES ${SPHINX_DOC_OUTPUT_DIRECTORY}/revealjs/${_project_name}.html
             ${SPHINX_DOC_DEPENDS} ${SPHINX_DOC_RST_FILES}
        )
    endif()

    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${SPHINX_CLEAN_FILES}")

    add_custom_target(
        doc_${_outtarget_name} ALL
        DEPENDS ${SPHINX_DOC_DEPENDS} ${SPHINX_DEPENDENCIES}
        SOURCES ${SPHINX_DOC_RST_FILES}
    )
    if(SPHINX_DOC_EXCLUDE_FROM_ALL)
        set_target_properties(doc_${_outtarget_name} PROPERTIES EXCLUDE_FROM_ALL TRUE)
    endif()

    set_target_properties( doc_${_outtarget_name} PROPERTIES OUTPUT_DIRECTORY ${SPHINX_DOC_OUTPUT_DIRECTORY})

    foreach(_builder ${SPHINX_DOC_BUILDERS})
        if(SPHINX_INSTALL_DIR_${_builder})
            set_target_properties(
                doc_${_outtarget_name} PROPERTIES INSTALL_DIR_${_builder} ${SPHINX_INSTALL_DIR_${_builder}}
            )
        endif()
        if(SPHINX_INSTALL_FILES_${_builder})
            set_target_properties(
                doc_${_outtarget_name} PROPERTIES INSTALL_FILES_${_builder} ${SPHINX_INSTALL_FILES_${_builder}}
            )
        endif()
        if(SPHINX_TARGET_FILE_${_builder})
            set_target_properties(
                doc_${_outtarget_name} PROPERTIES TARGET_FILE_${_builder} ${SPHINX_TARGET_FILE_${_builder}}
            )
        endif()
    endforeach()

endmacro(sphinx_doc_generate)

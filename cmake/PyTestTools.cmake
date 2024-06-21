include(PyEnv)

set(PYTHON_VENV_DIR "${yacomponent_BINARY_DIR}/pypackages/venv")
if(WIN32)
    set(PYTHON_VENV_BIN_DIR ${PYTHON_VENV_DIR}/Scripts)
else()
    set(PYTHON_VENV_BIN_DIR ${PYTHON_VENV_DIR}/bin)
endif()

function(yc_add_pytest)
    set(options NO_ASAN NO_CAPTURE)
    set(oneValueArgs NAME WORKING_DIRECTORY)
    set(multiValueArgs WHAT PYTHONPATH DEPEND_PATHS)
    cmake_parse_arguments(args "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(WITH_UBSAN OR args_NO_CAPTURE)
        list(APPEND pytest_args "--capture=no") # don't hide UBSAN output in tests or if wanted.
    endif()

    yc_add_test(
        NAME
        ${args_NAME}
        COMMAND
        ${PYTHON_VENV_BIN_DIR}/pytest
        ${pytest_args}
        ${args_WHAT}
        WORKING_DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}"
        PYTHONPATH
        "${PRO_PYPACKAGES_DIR}"
        ${args_PYTHONPATH}
        ${PYTHONPATH} # add global/common PYTHONPATH to test-specific one
        DEPEND_PATHS
        ${args_DEPEND_PATHS}
    )

    if(args_WORKING_DIRECTORY)
        set_tests_properties(${args_NAME} PROPERTIES WORKING_DIRECTORY ${args_WORKING_DIRECTORY})
    endif()

    if(NOT args_NO_ASAN)
        yc_preload_asan_in_test(${args_NAME})
    endif()
endfunction()

function(yc_preload_asan_in_test)
    # pass any number of tests as argument to get libasan preload if enabled example: running a test with Python
    # libpython is not compiled with asan and it gets loaded before our pymodules -> preload asan
    if(LIBASAN)
        set_property(
            TEST ${ARGN}
            APPEND
            PROPERTY ENVIRONMENT "LD_PRELOAD=${LIBASAN}"
        )
    endif()
endfunction()

function(yc_add_test)
    set(options)
    set(oneValueArgs NAME WORKING_DIRECTORY)
    set(multiValueArgs COMMAND DEPEND_PATHS CONFIGURATIONS PYTHONPATH)

    cmake_parse_arguments(test_arg "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT test_arg_NAME)
        message(FATAL_ERROR "use yc_add_test( NAME .. COMMAND ..) syntax.\nSee add_test() cmake documentation")
    endif()

    set(extra_args)
    if(test_arg_CONFIGURATIONS)
        list(APPEND extra_args CONFIGURATIONS ${test_arg_CONFIGURATIONS})
    endif()
    if(test_arg_WORKING_DIRECTORY)
        list(APPEND extra_args WORKING_DIRECTORY ${test_arg_WORKING_DIRECTORY})
    endif()

    add_test(NAME "${test_arg_NAME}" COMMAND ${test_arg_COMMAND} ${extra_args})

    set(required_files)
    set(environment)

    _py_add_test_set_environment_mods()

    set_tests_properties(
        ${test_arg_NAME} PROPERTIES ENVIRONMENT_MODIFICATION "${environment}" REQUIRED_FILES "${required_files}"
    )
endfunction()

macro(_py_add_test_set_environment_mods)
    set(environment)

    set(runpath ${test_arg_DEPEND_PATHS} ${PRO_RUNPATH})
    set(pythonpath)

    # Qt
    list(APPEND environment "QT_QPA_PLATFORM_PLUGIN_PATH=set:${CONAN_QT_ROOT}/plugins")

    # Python
    if(TARGET Python::Interpreter)
        get_target_property(pythonhome Python::Interpreter LOCATION)
        get_filename_component(pythonhome "${pythonhome}" DIRECTORY)
        if(UNIX)
            get_filename_component(pythonhome "${pythonhome}" DIRECTORY) # strip /bin
        endif()
        list(APPEND environment "PYTHONHOME=set:$<SHELL_PATH:${pythonhome}>" "OPENBLAS_NUM_THREADS=set:1")
        _py_add_test_set_pythonpath()
    endif()

    # PySide2
    if(EXISTS "${PySide2_IMPORT_PREFIX}")
        list(APPEND pythonpath "${PySide2_IMPORT_PREFIX}")
        list(APPEND environment "PYSIDE_DISABLE_INTERNAL_QT_CONF=set:1")
        list(APPEND environment "PYTEST_QT_API=set:pyside2")
    endif()

    if(WIN32)
        string(REPLACE ";" "\\\;" runpath "${runpath}")
        string(REPLACE ";" "\\\;" pythonpath "${pythonpath}")
    endif()

    list(APPEND environment
         "$<IF:$<PLATFORM_ID:Windows>,PATH,LD_LIBRARY_PATH>=path_list_prepend:$<SHELL_PATH:${runpath}>"
    )
    if(pythonpath)
        list(APPEND environment "PYTHONPATH=set:$<SHELL_PATH:${pythonpath}>")
    endif()
    if(WITH_ASAN)
        list(APPEND environment "LSAN_OPTIONS=set:suppressions=${CMAKE_CURRENT_FUNCTION_LIST_DIR}/sanitize/lsan.supp")
        list(APPEND environment "ASAN_OPTIONS=set:suppressions=${CMAKE_CURRENT_FUNCTION_LIST_DIR}/sanitize/asan.supp"
        )# :fast_unwind_on_malloc=0
        list(APPEND environment "QT_XCB_GL_INTEGRATION=set:none")
    endif()

    # list(APPEND environment "PYTEST_ADDOPTS=set:-p no:pytest-qt")
endmacro()

macro(_py_add_test_set_pythonpath)
    foreach(item IN LISTS test_arg_PYTHONPATH)
        if(TARGET "${item}")
            get_target_property(package_dir ${item} PYPACKAGE_PATH)
            if(package_dir)
                set(item "${package_dir}")
                # maybe add __init__.py to required files
            else()
                list(APPEND required_files "$<TARGET_FILE:${item}>")
                set(item "$<TARGET_FILE_DIR:${item}>")
            endif()
        endif()
        list(APPEND pythonpath "${item}")
    endforeach()
endmacro()

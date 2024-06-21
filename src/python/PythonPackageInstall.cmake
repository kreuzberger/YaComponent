# ----------------------------------------------------------------------------------
# Firma     PROCITEC GmbH    (C) Copyright PROCITEC GmbH 2022
# ----------------------------------------------------------------------------------
# Dieses Computerprogramm ist urheberrechtlich geschuetzt(Paragraphen 69a ff UrhG).
# Urheber ist die PROCITEC GmbH.
#
# Die Nutzung, Weitergabe und Vervielfaeltigung dieses Computerprogramms,
# sowie Verwertung und Mitteilung seines Inhaltes ist durch den Vertrag
# BWB E/UR1A/4A104/L5305 geregelt.
#
# Anderweitige Nutzung, Weitergabe und Vervielfaeltigung dieses Computer-
# programms, sowie Verwertung und Mitteilung seines Inhaltes bedarf der
# ausdruecklichen Zustimmung der PROCITEC GmbH.
#
# Zuwiderhandlung verpflichtet zum Schadensersatz.
# ----------------------------------------------------------------------------------

message(" using PYTHONHOME ${PYTHONHOME}")
message(" using PYTHON_INTERPRETER ${PYTHON_INTERPRETER}")
message(" using PACKAGE_DIR ${PACKAGE_DIR}")
message(" using VENV_DIR ${VENV_DIR}")
message(" using OS_CENTOS ${OS_CENTOS}")
message(" using CONAN_PYZMQ_ROOT ${CONAN_PYZMQ_ROOT}")

set(res_var)
set(environment)

get_filename_component(pythonhome "${PYTHON_INTERPRETER}" DIRECTORY)
set(runpath ${pythonhome})
if(UNIX)
    get_filename_component(pythonhome "${pythonhome}" DIRECTORY) # strip /bin
endif()

list(APPEND environment "PYTHONHOME=${pythonhome}")

if(WIN32)
    string(REPLACE ";" "\\\;" runpath "${runpath}")
    string(REPLACE ";" "\\\;" pythonpath "${pythonpath}")
endif()

list(APPEND environment "$<IF:$<PLATFORM_ID:Windows>,PATH,LD_LIBRARY_PATH>=${runpath}")

execute_process(COMMAND ${CMAKE_COMMAND} -E env ${environment} ${PYTHON_INTERPRETER} -s -m venv ${VENV_DIR})

# execute_process( COMMAND ${CMAKE_COMMAND} -E env ${environment} ${PYTHON_INTERPRETER} -s -m ensurepip )
# execute_process( COMMAND ${CMAKE_COMMAND} -E env ${environment} ${PYTHON_INTERPRETER} -s -m pip install -t
# ${PYPACKAGE_DIR} --upgrade --no-index --no-cache-dir --find-links=${PACKAGE_DIR} pip setuptools wheel poetry )

# list( APPEND OUTDATED_PACKAGES "sphinxcontrib-needs" )

# foreach( _line IN LISTS OUTDATED_PACKAGES ) execute_process( COMMAND ${CMAKE_COMMAND} -E env ${environment}
# ${PYTHON_INTERPRETER} -s -m pip uninstall --yes ${_line} ) endforeach()

if(WIN32)
    file(STRINGS ${CMAKE_CURRENT_LIST_DIR}/site-packages/requirements_windows.txt REQUIREMENTS)
    set(VENV_PYTHON_EXECUTABLE ${VENV_DIR}/Scripts/python)
else()
    set(VENV_PYTHON_EXECUTABLE ${VENV_DIR}/bin/python)
    if(OS_CENTOS)
        file(STRINGS ${CMAKE_CURRENT_LIST_DIR}/site-packages/requirements_centos.txt REQUIREMENTS)
    else()
        file(STRINGS ${CMAKE_CURRENT_LIST_DIR}/site-packages/requirements_linux.txt REQUIREMENTS)
    endif()
endif()

foreach(_line ${REQUIREMENTS})
    string(FIND "${_line}" "#" _hashtag)

    if(${_hashtag} GREATER 0)
        string(SUBSTRING "${_line}" 0 ${_hashtag} _requirement)
        string(SUBSTRING "${_line}" ${_hashtag} -1 _comment)
    else()
        set(_comment)
        set(_requirement "${_line}")
    endif()
    string(REGEX REPLACE "#.*" "" _requirement "${_line}")
    string(REGEX REPLACE "@.*" "" _requirement "${_requirement}")
    # string(REGEX REPLACE ".*#" "" _comment "${_line}") string(REPLACE "via -r requirements.in" "" _comment
    # "${_comment}" )

    string(STRIP "${_requirement}" _requirement)
    # string(STRIP "${_comment}" _comment )

    if(NOT "${_requirement}" STREQUAL "")
        message("installing requirement ${_requirement}")
        execute_process(
            COMMAND
                ${VENV_PYTHON_EXECUTABLE} -s -m pip install --upgrade --no-index --no-cache-dir
                --find-links=${PACKAGE_DIR} --find-links=${CONAN_PYZMQ_ROOT} --log install.log
                ${_requirement} # COMMAND_ECHO STDOUT
            RESULT_VARIABLE res_var
        )

        if(NOT "${res_var}" STREQUAL "0")
            message(FATAL_ERROR "package install step failed. command returned: ${res_var}")
        endif()
    endif()
endforeach()

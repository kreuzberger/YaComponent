project(pyyacomponent)

set(PYTHON_SITE_VERSION "1")

set(PACKAGE_DIR "${CMAKE_CURRENT_LIST_DIR}/site-packages/wheels")
set(PYTHON_VENV_DIR "${yacomponent_BINARY_DIR}/pypackages/venv")
set(PYTHON_UPDATE_DEPENDENCY "${PYTHON_VENV_DIR}/python_update-${PYTHON_SITE_VERSION}.txt")

add_custom_command(
    OUTPUT "${PYTHON_UPDATE_DEPENDENCY}"
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${PYTHON_VENV_DIR}
    COMMAND
        ${CMAKE_COMMAND} -D"PYTHONHOME=${CONAN_PYTHON_ROOT}" -D"PYTHON_INTERPRETER=$<TARGET_FILE:Python::Interpreter>"
        -D"PACKAGE_DIR=${PACKAGE_DIR}" -D"VENV_DIR=${PYTHON_VENV_DIR}"
        # -D"CONAN_PYZMQ_ROOT=${CONAN_PYZMQ_ROOT}"
        -D"OS_CENTOS=${OS_CENTOS}" -P ${CMAKE_CURRENT_SOURCE_DIR}/PythonPackageInstall.cmake
    COMMAND ${CMAKE_COMMAND} -E touch "${PYTHON_UPDATE_DEPENDENCY}"
    COMMENT "update python venv ${PYTHON_VENV_DIR}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

add_custom_target(
    python-update
    DEPENDS "${PYTHON_UPDATE_DEPENDENCY}"
    COMMENT "install python site-packages"
)
install(
    DIRECTORY "${pyyacomponent_SOURCE_DIR}/yacomponent"
    # DESTINATION "lib/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}"
    DESTINATION "lib/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}/site-packages"
    COMPONENT "yacomponent"
    PATTERN "__pycache__" EXCLUDE
)

if(ruff)
    set(ruff_files
        ${CMAKE_CURRENT_SOURCE_DIR}/yacomponent/__init__.py
        ${CMAKE_CURRENT_SOURCE_DIR}/yacomponent/ctx.py
        ${CMAKE_CURRENT_SOURCE_DIR}/yacomponent/proxy.py
        ${CMAKE_CURRENT_SOURCE_DIR}/yacomponent/publisher.py
        ${CMAKE_CURRENT_SOURCE_DIR}/yacomponent/stub.py
        ${CMAKE_CURRENT_SOURCE_DIR}/yacomponent/subscriber.py
        ${CMAKE_CURRENT_SOURCE_DIR}/yacomponent/variables.py
    )

    add_test(NAME ruff-check-yacomponent COMMAND ${ruff} check ${ruff_files})
    add_test(NAME ruff-check-format-yacomponent COMMAND ${ruff} format --diff ${ruff_files})
endif()

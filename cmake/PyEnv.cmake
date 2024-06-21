set(PRO_RUNPATH
    # Qt
    "$<TARGET_FILE_DIR:Qt5::Core>"
    # Python and bindings (may be missing)
    "$<$<TARGET_EXISTS:Python::Python>:$<TARGET_FILE_DIR:Python::Python>>"
)

function(_yc_create_runenv_python dll_directories)
    list(FILTER dll_directories EXCLUDE REGEX "Marble|Python::|PythonQt::")
    list(TRANSFORM dll_directories REPLACE "^(.+)$" "$<$<BOOL:\\1>:os.add_dll_directory('\\1')  # type: ignore>")
    file(
        GENERATE
        OUTPUT "${PRO_PYPACKAGES_DIR}/_prorunenv.py"
        CONTENT
            "\
import os

if hasattr(os, 'add_dll_directory'):
    $<JOIN:${dll_directories},\n    >
    os.environ['QT_QPA_PLATFORM_PLUGIN_PATH'] = r'$<SHELL_PATH:$<TARGET_FILE_DIR:Qt5::Core>/../plugins>'
    os.environ['PYSIDE_DISABLE_INTERNAL_QT_CONF'] = '1'"
    )

    file(
        GENERATE
        OUTPUT "${PRO_PYPACKAGES_DIR}/sx/__init__.py"
        CONTENT "import _prorunenv"
    )
endfunction()

function(_yc_create_runenv_shell runpath binpath pythonpath)
    if(WIN32)
        string(REPLACE ";" "\;" runpath "${runpath}")
        string(REPLACE ";" "\;" binpath "${binpath}")
        string(REPLACE ";" "\;" pythonpath "${pythonpath}")
    endif()

    set(content "#!/bin/bash\n\n")

    set(RUNPATH "$<IF:$<PLATFORM_ID:Windows>,PATH,LD_LIBRARY_PATH>")
    string(APPEND content "export ${RUNPATH}=\"$<SHELL_PATH:${runpath}>\"\${${RUNPATH}+:\$${RUNPATH}}\n")
    string(APPEND content "$<$<BOOL:${binpath}>:export PATH=\"$<SHELL_PATH:${binpath}>\"\${PATH+:\$PATH}\n>")
    string(
        APPEND
        content
        "$<$<TARGET_EXISTS:Python::Interpreter>:export PYTHONPATH=\"$<SHELL_PATH:${pythonpath}>\"\${PYTHONPATH+:\$PYTHONPATH}\n>"
    )

    file(
        GENERATE
        OUTPUT "prorunenv.sh"
        CONTENT "${content}"
    )
endfunction()

set(PRO_PYPACKAGES_DIR
    "${CMAKE_BINARY_DIR}/pypackages"
    CACHE PATH "Where Python modules and packages are build"
)

_yc_create_runenv_python("${PRO_RUNPATH}")
_yc_create_runenv_shell(
    "${PRO_RUNPATH}" "$<$<TARGET_EXISTS:Python::Interpreter>:$<TARGET_FILE_DIR:Python::Interpreter>>"
    "${PRO_PYPACKAGES_DIR};$<$<TARGET_EXISTS:PySide2::libshiboken>:$<TARGET_FILE_DIR:PySide2::libshiboken>/..>"
)

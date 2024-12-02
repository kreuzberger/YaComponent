project(TextExample)

protobuf_generate(
    OUT_VAR
    ProtoSourcesTextGen
    LANGUAGE
    cpp
    PROTOS
    textgen/TextGenIfc.proto
    APPEND_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}
)
protobuf_generate(
    OUT_VAR
    PyProtoSourcesTextGen
    LANGUAGE
    python
    PROTOS
    textgen/TextGenIfc.proto
    APPEND_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}
)

yacomponent_ifc_generate(GENERATED_IFC_SRC textgen/TextGenIfc.xml textout/TextOutIfc.xml)
yacomponent_ifc_generate(PYGENERATED_IFC_SRC LANGUAGE python NO_DOC textgen/TextGenIfc.xml textout/TextOutIfc.xml)
yacomponent_ifc_generate(GENERATED_IFC_SRC_OUT textout/TextOutIfc.xml)
yacomponent_ifc_generate(PYGENERATED_IFC_SRC_OUT LANGUAGE python NO_DOC textout/TextOutIfc.xml)

yacomponent_generate(GENERATED_COMP_GEN_SRC textgen/TextGenComp.xml)
yacomponent_generate(PYGENERATED_COMP_GEN_SRC LANGUAGE python NO_DOC textgen/TextGenComp.xml)
yacomponent_generate(GENERATED_COMP_OUT_SRC textout/TextOutComp.xml)
yacomponent_generate(PYGENERATED_COMP_OUT_SRC LANGUAGE python NO_DOC textout/TextOutComp.xml)

set(textgen_SRCS
    ${GENERATED_IFC_SRC}
    ${PYGENERATED_IFC_SRC}
    ${GENERATED_COMP_GEN_SRC}
    ${PYGENERATED_COMP_GEN_SRC}
    ${ProtoSourcesTextGen}
    ${PyProtoSourcesTextGen}
    textgencompunittest.cpp
    TextUnittestMP_PUB.cpp
    TextUnittestMP_PUB.h
    ${UNITTESTMPPUB_MOCS}
)

set(textout_SRCS
    ${GENERATED_IFC_SRC}
    ${PYGENERATED_IFC_SRC}
    ${GENERATED_IFC_SRC_OUT}
    ${PYGENERATED_IFC_SRC_OUT}
    ${GENERATED_COMP_GEN_SRC}
    ${PYGENERATED_COMP_GEN_SRC}
    ${GENERATED_COMP_OUT_SRC}
    ${PYGENERATED_COMP_OUT_SRC}
    ${ProtoSourcesTextGen}
    ${PyProtoSourcesTextGen}
    textoutcompunittest.cpp
    TextUnittestMP_SUB.cpp
    TextUnittestMP_SUB.h
    ${UNITTESTMPSUB_MOCS}
)

set(textunittest_SRCS
    ${GENERATED_IFC_SRC}
    ${PYGENERATED_IFC_SRC}
    ${GENERATED_IFC_SRC_OUT}
    ${PYGENERATED_IFC_SRC_OUT}
    ${GENERATED_COMP_GEN_SRC}
    ${PYGENERATED_COMP_GEN_SRC}
    ${GENERATED_COMP_OUT_SRC}
    ${PYGENERATED_COMP_OUT_SRC}
    ${ProtoSourcesTextGen}
    ${PyProtoSourcesTextGen}
    textoutcompunittest.cpp
    textgencompunittest.cpp
    TextUnittest.cpp
    TextUnittest.h
    ${UNITTEST_MOCS}
)

set(textunittestmp_SRCS TextUnittestMP.cpp TextUnittestMP.h ${UNITTESTMP_MOCS})

add_executable(textgenMPMT ${textgen_SRCS})
add_executable(textoutMPMT ${textout_SRCS})
add_executable(textunittest ${textunittest_SRCS})
add_executable(textunittestmp ${textunittestmp_SRCS})

target_link_libraries(textgenMPMT PRIVATE YaComponent Qt5::Test)
target_link_libraries(textoutMPMT PRIVATE YaComponent Qt5::Test)
target_link_libraries(textunittest PRIVATE YaComponent Qt5::Test)
target_link_libraries(textunittestmp PRIVATE YaComponent Qt5::Test)

yc_add_test(NAME textunittest COMMAND textunittest)
yc_add_test(NAME textunittestmp COMMAND textunittestmp)

yc_add_pytest(
    NAME
    pytextunittestsp
    WHAT
    textunittestsp.py
    --log-cli-level=Info
    PYTHONPATH
    "${CMAKE_CURRENT_BINARY_DIR};${pyyacomponent_SOURCE_DIR};${TextExample_BINARY_DIR}/TextGenIfc/py;${TextExample_BINARY_DIR}/TextGenComp/py;${TextExample_BINARY_DIR}/TextOutIfc/py;${TextExample_BINARY_DIR}/TextOutComp/py;"
)

yc_add_pytest(
    NAME
    pytextunittestguisp
    WHAT
    textunittestguisp.py
    --log-cli-level=Info
    PYTHONPATH
    "${CMAKE_CURRENT_BINARY_DIR};${pyyacomponent_SOURCE_DIR};${TextExample_BINARY_DIR}/TextGenIfc/py;${TextExample_BINARY_DIR}/TextGenComp/py;${TextExample_BINARY_DIR}/TextOutIfc/py;${TextExample_BINARY_DIR}/TextOutComp/py;"
)



if(clang_format)
    set(clang_files)
    foreach(_file IN LISTS GENERATED_IFC_SRC GENERATED_IFC_SRC_OUT GENERATED_COMP_GEN_SRC GENERATED_COMP_OUT_SRC)
        get_filename_component(_ext ${_file} LAST_EXT)
        if("${_ext}" STREQUAL ".h" OR "${_ext}" STREQUAL ".cpp")
            # message("clang for ${_file} ${_ext}")
            list(APPEND clang_files ${_file})
        endif()
    endforeach()

    add_test(NAME clang-format-test-text COMMAND ${clang_format} -n -Werror --style=file ${clang_files})
endif()

if(ruff)
    set(ruff_files)
    foreach(_file IN LISTS PYGENERATED_IFC_SRC PYGENERATED_IFC_SRC_OUT PYGENERATED_COMP_GEN_SRC
                           PYGENERATED_COMP_OUT_SRC PyProtoSourcesTextGen
    )
        get_filename_component(_ext ${_file} LAST_EXT)
        if("${_ext}" STREQUAL ".py" AND NOT ${_file} MATCHES ".*_pb2.py")
            # message("clang for ${_file} ${_ext}")
            list(APPEND ruff_files ${_file})
        endif()
    endforeach()

    add_test(NAME ruff-check-text COMMAND ${ruff} check ${ruff_files})
    add_test(NAME ruff-check-format-text COMMAND ${ruff} format --diff --line-length 300 ${ruff_files})
endif()

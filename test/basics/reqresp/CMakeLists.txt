project(BasicReqRespTest)

protobuf_generate(
    OUT_VAR
    ProtoSourcesWorker
    LANGUAGE
    cpp
    PROTOS
    worker/WorkerIfc.proto
    APPEND_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}
)

yacomponent_ifc_generate(GENERATED_IFC_SRC worker/WorkerIfc.xml)

yacomponent_generate(GENERATED_COMP_WORKER_SRC worker/WorkerComp.xml)
yacomponent_generate(GENERATED_COMP_CONTROL_SRC control/ControlComp.xml)

set(basic_reqresp_test_SRCS ${GENERATED_IFC_SRC} ${GENERATED_COMP_WORKER_SRC} ${GENERATED_COMP_CONTROL_SRC}
                            ${ProtoSourcesWorker} BasicReqRespTest.cpp BasicReqRespTest.h
)

set(basic_reqresp_gui_test_SRCS ${basic_reqresp_test_SRCS} BasicReqRespGuiTest.cpp BasicReqRespGuiTest.h
)

add_executable(basic_reqresp_test ${basic_reqresp_test_SRCS} BasicReqRespTestMain.cpp)
add_executable(basic_reqresp_gui_test ${basic_reqresp_gui_test_SRCS} BasicReqRespGuiTestMain.cpp)

target_link_libraries(basic_reqresp_test PRIVATE YaComponent Qt5::Core Qt5::Test)
target_link_libraries(basic_reqresp_gui_test PRIVATE YaComponent Qt5::Core Qt5::Test)

yc_add_test(NAME basic_reqresp_test COMMAND basic_reqresp_test)
set_tests_properties(basic_reqresp_test PROPERTIES TIMEOUT 60)

yc_add_test(NAME basic_reqresp_gui_test COMMAND basic_reqresp_test)
set_tests_properties(basic_reqresp_gui_test PROPERTIES TIMEOUT 60)

if(clang_format)
    set(clang_files)
    foreach(_file IN LISTS GENERATED_IFC_SRC GENERATED_COMP_WORKER_SRC GENERATED_COMP_WORKER_SRC)
        get_filename_component(_ext ${_file} LAST_EXT)
        if("${_ext}" STREQUAL ".h" OR "${_ext}" STREQUAL ".cpp")
            # message("clang for ${_file} ${_ext}")
            list(APPEND clang_files ${_file})
        endif()
    endforeach()

    add_test(NAME clang-format-test-text COMMAND ${clang_format} -n -Werror --style=file ${clang_files})
endif()

# if(ruff) set(ruff_files) foreach(_file IN LISTS PYGENERATED_IFC_SRC PYGENERATED_IFC_SRC_OUT PYGENERATED_COMP_GEN_SRC
# PYGENERATED_COMP_OUT_SRC PyProtoSourcesTextGen ) get_filename_component(_ext ${_file} LAST_EXT) if("${_ext}" STREQUAL
# ".py" AND NOT ${_file} MATCHES ".*_pb2.py") message("clang for ${_file} ${_ext}") list(APPEND ruff_files ${_file})
# endif() endforeach()

# add_test(NAME ruff-check-text COMMAND ${ruff} check ${ruff_files}) add_test(NAME ruff-check-format-text COMMAND
# ${ruff} format --diff --line-length 300 ${ruff_files}) endif()

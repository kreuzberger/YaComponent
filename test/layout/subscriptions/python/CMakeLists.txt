protobuf_generate(
    OUT_VAR
    PyProtoSourcesTextGen
    LANGUAGE
    python
    PROTOS
    ${SubscriptionsTest_SOURCE_DIR}/publisher/PublisherIfc.proto
    APPEND_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}
)

protobuf_generate(
    OUT_VAR
    ProtoSourcesTextGen
    LANGUAGE
    cpp
    PROTOS
    ${SubscriptionsTest_SOURCE_DIR}/publisher/PublisherIfc.proto
    APPEND_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}
)

yacomponent_ifc_generate(
    GENERATED_IFC_SRC_PUB_PY LANGUAGE python ${SubscriptionsTest_SOURCE_DIR}/publisher/PublisherIfc.xml
)
yacomponent_ifc_generate(
    GENERATED_IFC_SRC_PUB_CPP LANGUAGE cpp NO_DOC ${SubscriptionsTest_SOURCE_DIR}/publisher/PublisherIfc.xml
)

yacomponent_generate(
    GENERATED_COMP_PUB_SRC_PY LANGUAGE python ${SubscriptionsTest_SOURCE_DIR}/publisher/PublisherComp.xml
)

yacomponent_generate(
    GENERATED_COMP_PUB_SRC_CPP LANGUAGE cpp NO_DOC ${SubscriptionsTest_SOURCE_DIR}/publisher/PublisherComp.xml
)

yacomponent_generate(
    GENERATED_COMP_SUB_SRC_PY LANGUAGE python ${SubscriptionsTest_SOURCE_DIR}/subscriber/SubscriberComp.xml
)

add_custom_target(
    pyprotobuf_generate ALL DEPENDS ${PyProtoSourcesTextGen} ${GENERATED_IFC_SRC_PUB_PY} ${GENERATED_COMP_PUB_SRC_PY}
                                    ${GENERATED_COMP_SUB_SRC_PY} python-update
)

add_executable(
    publisher publisher.cpp publisher.h ${GENERATED_COMP_PUB_SRC_CPP} ${GENERATED_IFC_SRC_PUB_CPP}
              ${ProtoSourcesTextGen}
)
target_link_libraries(publisher PRIVATE YaComponent Qt5::Test)

yc_add_pytest(
    NAME
    pypubsubunittestsp
    WHAT
    pypubsubunittestsp.py
    --log-cli-level=Info
    PYTHONPATH
    "${CMAKE_CURRENT_BINARY_DIR};${pyyacomponent_SOURCE_DIR};${SubscriptionsTest_BINARY_DIR}/python/PublisherIfc/py;${SubscriptionsTest_BINARY_DIR}/python/PublisherComp/py;${SubscriptionsTest_BINARY_DIR}/python/SubscriberComp/py"
)

yc_add_pytest(
    NAME
    pypubsubunittestmp
    WHAT
    pypubsubunittestmp.py
    --log-cli-level=Info
    --publisher=${CMAKE_CURRENT_BINARY_DIR}/publisher
    PYTHONPATH
    "${CMAKE_CURRENT_BINARY_DIR};${pyyacomponent_SOURCE_DIR};${SubscriptionsTest_BINARY_DIR}/python/PublisherIfc/py;${SubscriptionsTest_BINARY_DIR}/python/PublisherComp/py;${SubscriptionsTest_BINARY_DIR}/python/SubscriberComp/py"
)

if(ruff)
    set(ruff_files)
    foreach(_file IN LISTS GENERATED_IFC_SRC_PUB_PY GENERATED_COMP_PUB_SRC_PY GENERATED_COMP_SUB_SRC_PY)
        get_filename_component(_ext ${_file} LAST_EXT)
        if("${_ext}" STREQUAL ".py" AND NOT ${_file} MATCHES ".*_pb2.py")
            # message("clang for ${_file} ${_ext}")
            list(APPEND ruff_files ${_file})
        endif()
    endforeach()

    add_test(NAME ruff-check-subscriptions COMMAND ${ruff} check ${ruff_files})
    add_test(NAME ruff-check-format-subscriptions COMMAND ${ruff} format --diff --line-length 300 ${ruff_files})
endif()

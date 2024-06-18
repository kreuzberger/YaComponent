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

pro_add_pytest(
    NAME pypubsubunittestsp WHAT pypubsubunittestsp.py PYTHONPATH
    "${CMAKE_CURRENT_BINARY_DIR};${pyyacomponent_SOURCE_DIR}"
)

yacomponent_ifc_generate(
    GENERATED_IFC_SRC_PUB LANGUAGE python ${SubscriptionsTest_SOURCE_DIR}/publisher/PublisherIfc.xml
)

add_custom_target(pyprotobuf_generate ALL DEPENDS ${PyProtoSourcesTextGen} ${GENERATED_IFC_SRC_PUB} python-update)

# add_dependencies(pypubsubunittestsp pyprotobuf_generate) yacomponent_ifc_generate(GENERATED_IFC_SRC_PUB
# ${SubscriptionsTest_SOURCE_DIR}/publisher/PublisherIfc.xml) yacomponent_ifc_generate(GENERATED_IFC_SRC_SUB
# ${SubscriptionsTest_SOURCE_DIR}/subscriber/SubscriberIfc.xml)

# yacomponent_generate(GENERATED_COMP_PUB_SRC ${SubscriptionsTest_SOURCE_DIR}/publisher/PublisherComp.xml)
# yacomponent_generate(GENERATED_COMP_SUB_SRC ${SubscriptionsTest_SOURCE_DIR}/subscriber/SubscriberComp.xml)

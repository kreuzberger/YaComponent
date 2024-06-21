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

yacomponent_ifc_generate(GENERATED_IFC_SRC_PUB ${SubscriptionsTest_SOURCE_DIR}/publisher/PublisherIfc.xml)
yacomponent_ifc_generate(GENERATED_IFC_SRC_SUB ${SubscriptionsTest_SOURCE_DIR}/subscriber/SubscriberIfc.xml)

yacomponent_generate(GENERATED_COMP_PUB_SRC ${SubscriptionsTest_SOURCE_DIR}/publisher/PublisherComp.xml)
yacomponent_generate(GENERATED_COMP_SUB_SRC ${SubscriptionsTest_SOURCE_DIR}/subscriber/SubscriberComp.xml)

set(publisher_SRCS ${GENERATED_IFC_SRC_PUB} ${GENERATED_COMP_PUB_SRC} ${ProtoSourcesTextGen} PubSubUnittestMP_PUB.cpp
                   PubSubUnittestMP_PUB.h
)

set(subscriber_fast_SRCS
    ${GENERATED_IFC_SRC_SUB}
    ${GENERATED_IFC_SRC_PUB}
    ${GENERATED_COMP_PUB_SRC}
    ${GENERATED_COMP_SUB_SRC}
    ${ProtoSourcesTextGen}
    PubSubUnittestMP_fast_SUB.cpp
    PubSubUnittestMP_fast_SUB.h
)

set(subscriber_slow_SRCS
    ${GENERATED_IFC_SRC_SUB}
    ${GENERATED_IFC_SRC_PUB}
    ${GENERATED_COMP_PUB_SRC}
    ${GENERATED_COMP_SUB_SRC}
    ${ProtoSourcesTextGen}
    PubSubUnittestMP_slow_SUB.cpp
    PubSubUnittestMP_slow_SUB.h
)

set(pubsubunittestmp_SRCS PubSubUnittestMP.cpp PubSubUnittestMP.h)

set(pubsubunittestSP_SRCS
    PubSubUnittestSP.cpp
    PubSubUnittestSP.h
    ${GENERATED_IFC_SRC_PUB}
    ${GENERATED_IFC_SRC_SUB}
    ${GENERATED_COMP_PUB_SRC}
    ${GENERATED_COMP_SUB_SRC}
    ${ProtoSourcesTextGen}
    PubSubUnittestSP_PUB.cpp
    PubSubUnittestSP_PUB.h
    PubSubUnittestSP_SUB.cpp
    PubSubUnittestSP_SUB.h
)

add_executable(publisherMPMT ${publisher_SRCS})
add_executable(subscriber_fast_MPMT ${subscriber_fast_SRCS})
add_executable(subscriber_slow_MPMT ${subscriber_slow_SRCS})
add_executable(pubsubunittestmp ${pubsubunittestmp_SRCS})
add_executable(pubsubunittestsp ${pubsubunittestSP_SRCS})

target_link_libraries(publisherMPMT PRIVATE YaComponent Qt5::Test)
target_link_libraries(subscriber_fast_MPMT PRIVATE YaComponent Qt5::Test)
target_link_libraries(subscriber_slow_MPMT PRIVATE YaComponent Qt5::Test)
target_link_libraries(pubsubunittestmp PRIVATE YaComponent Qt5::Test)
target_link_libraries(pubsubunittestsp PRIVATE YaComponent Qt5::Test)

yc_add_test(NAME pubsubunittestmp COMMAND pubsubunittestmp)
yc_add_test(NAME pubsubunittestsp COMMAND pubsubunittestsp)

set_tests_properties(pubsubunittestmp PROPERTIES TIMEOUT 30)
set_tests_properties(pubsubunittestsp PROPERTIES TIMEOUT 30)

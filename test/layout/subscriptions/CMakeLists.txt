project ( SubscriptionsTest)

PROTOBUF_GENERATE_CPP(ProtoSourcesTextGen ProtoHeadersTextGen publisher/PublisherIfc.proto)

YACOMPONENT_IFC_GENERATE(GENERATED_IFC_SRC_PUB publisher/PublisherIfc.xml)
YACOMPONENT_IFC_GENERATE(GENERATED_IFC_SRC_SUB subscriber/SubscriberIfc.xml)


YACOMPONENT_GENERATE(GENERATED_COMP_PUB_SRC publisher/PublisherComp.xml)
YACOMPONENT_GENERATE(GENERATED_COMP_SUB_SRC subscriber/SubscriberComp.xml)

YACOMPONENT_LAYOUT_GENERATE(GENERATED_LAYOUT_SRC PubSubUnittestMPMT.xml)

QT5_WRAP_CPP( UNITTESTMPSUB_FAST_MOCS PubSubUnittestMP_fast_SUB.h )
QT5_WRAP_CPP( UNITTESTMPSUB_SLOW_MOCS PubSubUnittestMP_slow_SUB.h)
QT5_WRAP_CPP( UNITTESTMPPUB_MOCS PubSubUnittestMP_PUB.h )

set( publisher_SRCS
  ${GENERATED_IFC_SRC_PUB}
  ${GENERATED_COMP_PUB_SRC}
  ${ProtoSourcesTextGen}
  ${GENERATED_LAYOUT_SRC}
  publishercompunittest.cpp
  PubSubUnittestMP_PUB.cpp
  PubSubUnittestMP_PUB.h
  ${UNITTESTMPPUB_MOCS}
)

set( subscriber_fast_SRCS
  ${GENERATED_IFC_SRC_SUB}
  ${GENERATED_IFC_SRC_PUB}
  ${GENERATED_COMP_PUB_SRC}
  ${GENERATED_COMP_SUB_SRC}
  ${ProtoSourcesTextGen}
  ${GENERATED_LAYOUT_SRC}
  subscriber_fast_compunittest.cpp
  PubSubUnittestMP_fast_SUB.cpp
  PubSubUnittestMP_fast_SUB.h
  ${UNITTESTMPSUB_FAST_MOCS}
)

set( subscriber_slow_SRCS
  ${GENERATED_IFC_SRC_SUB}
  ${GENERATED_IFC_SRC_PUB}
  ${GENERATED_COMP_PUB_SRC}
  ${GENERATED_COMP_SUB_SRC}
  ${ProtoSourcesTextGen}
  ${GENERATED_LAYOUT_SRC}
  subscriber_slow_compunittest.cpp
  PubSubUnittestMP_slow_SUB.cpp
  PubSubUnittestMP_slow_SUB.h
  ${UNITTESTMPSUB_SLOW_MOCS}
)

QT5_WRAP_CPP( UNITTESTMP_MOCS PubSubUnittestMP.h )

set( pubsubunittestmp_SRCS
  PubSubUnittestMP.cpp
  PubSubUnittestMP.h
  ${UNITTESTMP_MOCS}
)




add_executable( publisherMPMT  ${publisher_SRCS} )
add_executable( subscriber_fast_MPMT  ${subscriber_fast_SRCS} )
add_executable( subscriber_slow_MPMT  ${subscriber_slow_SRCS} )
add_executable( pubsubunittestmp ${pubsubunittestmp_SRCS} )


target_link_libraries(publisherMPMT Qt5::Core Qt5::Test protobuf::lib libzmq::static)
target_link_libraries(subscriber_fast_MPMT Qt5::Core Qt5::Test protobuf::lib libzmq::static)
target_link_libraries(subscriber_slow_MPMT Qt5::Core Qt5::Test protobuf::lib libzmq::static)
target_link_libraries(pubsubunittestmp Qt5::Core Qt5::Test protobuf::lib libzmq::static)

add_test( NAME pubsubunittestmp COMMAND pubsubunittestmp )

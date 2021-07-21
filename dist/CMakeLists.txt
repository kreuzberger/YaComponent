#install( TARGETS yacomponentgen DESTINATION "yacomponent/bin")
#install( TARGETS YaComponentLib DESTINATION "yacomponent/lib")
#install( TARGETS libzmq::static DESTINATION "yacomponent/lib")
#install( TARGETS protobuf::lib DESTINATION "yacomponent/lib")
install( DIRECTORY ${YaComponent_SOURCE_DIR}/src/perl/code/inc/ DESTINATION "yacomponent/include/yacomponent")
install( FILES FindYaComponent.cmake ${YaComponent_SOURCE_DIR}/cmake/YaComponent.cmake DESTINATION "yacomponent/cmake")
install( FILES CMakeLists.txt.in DESTINATION "yacomponent" RENAME CMakeLists.txt)


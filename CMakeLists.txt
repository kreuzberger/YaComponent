cmake_minimum_required(VERSION 2.6)
project(YaComponent)


set( DESIRED_QT_VERSION 4)

find_package(Qt4 REQUIRED)
find_package(Java REQUIRED)

INCLUDE (cmake/FindZeroMQ.cmake)
INCLUDE (cmake/FindPlantUML.cmake)
INCLUDE (cmake/FindSphinxBuild.cmake)

#INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )
if( CMAKE_HOST_UNIX )
  include_directories(/usr/include)
else( CMAKE_HOST_UNIX )
endif( CMAKE_HOST_UNIX )


enable_testing()
include(CTest)


find_package(Protobuf REQUIRED)




#ADD_DEFINITIONS( ${QT_DEFINITIONS} )
add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(test)


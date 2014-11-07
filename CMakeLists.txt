cmake_minimum_required(VERSION 2.6)
project(YaComponent)


set( DESIRED_QT_VERSION 4)

find_package(Qt4 REQUIRED)
#find_package(Java REQUIRED)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" )

find_package( ZeroMQ REQUIRED )
find_package( PlantUML REQUIRED)
find_package( SphinxBuild REQUIRED)
find_package( InstallQt REQUIRED)

#INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )
if( CMAKE_HOST_UNIX )
  include_directories(/usr/include)
else( CMAKE_HOST_UNIX )
endif( CMAKE_HOST_UNIX )


enable_testing()
include(CTest)


find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})


include_directories(${ZEROMQ_INCLUDE_DIRS})



#ADD_DEFINITIONS( ${QT_DEFINITIONS} )
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/cpp")

#installation default prefix like for old buildsys
set( CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE STRING "" FORCE)


add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(test)

cmake_minimum_required(VERSION 2.6)
project(YaComponent)


set( DESIRED_QT_VERSION 4)

find_package(Qt4 REQUIRED)
#find_package(Java REQUIRED)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" )

option( WITH_ZMQ "Build with delivered zeromq" ON)
option( WITH_PROTOBUF "Build with delivered protobuf" ON)

include( ExternalProject )

if( WITH_ZMQ )

  add_library(libzmq-static STATIC IMPORTED GLOBAL)

  ExternalProject_Add(zeromq
    URL "${CMAKE_SOURCE_DIR}/bin/zeromq-4.2.0.tar.gz"
    URL_MD5 "1fb2595d2a905a9e820c976a1d8348bc"

    SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/zmq/src"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/zmq/build"
    STAMP_DIR "${CMAKE_CURRENT_BINARY_DIR}/zmq/build-stamp"
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/zmq/
    INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/zmq/install
    CMAKE_ARGS "-DZMQ_BUILD_TESTS=OFF" "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/zmq/install/"
  )


  ExternalProject_Get_Property(zeromq install_dir)

  if( UNIX )
    set( ZEROMQ_LIBRARIES "${install_dir}/lib/libzmq-static.a" "pthread")
  else()
  endif()
  set( ZEROMQ_INCLUDE_DIRS "${install_dir}/include")

else()
  find_package( ZeroMQ REQUIRED)
endif()

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

if( WITH_PROTOBUF )
  if( UNIX )
    ExternalProject_Add(protobuf
      #SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.."
      URL "${CMAKE_SOURCE_DIR}/bin/protobuf-cpp-3.0.0.tar.gz"
      URL_MD5 "8f2a8786503459375fcf33517ce7f29b"
      STAMP_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf/build-stamp"
      SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf/build"
      BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf/build"
      INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/protobuf/install
      PREFIX ${CMAKE_CURRENT_BINARY_DIR}/protobuf/
      CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/protobuf/build/configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/protobuf/install
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND $(MAKE) install
    )

    ExternalProject_Get_Property(protobuf install_dir)
    set(PROTOBUF_LIBRARIES "${install_dir}/lib/libprotobuf.a")
  endif()
  set( PROTOBUF_INCLUDE_DIRS "${install_dir}/include")
  include(${CMAKE_SOURCE_DIR}/cmake/Protobuf.cmake)
else()
  find_package(Protobuf REQUIRED)
endif()

include_directories(${PROTOBUF_INCLUDE_DIRS})

add_definitions(-DZMQ_STATIC)
include_directories(${ZEROMQ_INCLUDE_DIRS})

#ADD_DEFINITIONS( ${QT_DEFINITIONS} )
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/cpp")

#installation default prefix like for old buildsys
set( CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE STRING "" FORCE)


add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(test)

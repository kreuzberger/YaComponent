cmake_minimum_required(VERSION 3.9)
project(YaComponent)

find_package( Qt5 COMPONENTS Core Test REQUIRED)
#find_package(Java REQUIRED)

# Force C++ standard, do not fall back, do not use compiler extensions
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" )

include( ExternalProject )

add_library(libzmq::static STATIC IMPORTED GLOBAL)

if (UNIX)
    set (LIBZMQ_STATIC libzmq-static.a)
else()
    set (LIBZMQ_STATIC libzmq-static.lib)
endif()

ExternalProject_Add(zeromq
URL "${CMAKE_SOURCE_DIR}/bin/zeromq-4.2.0.tar.gz"
URL_MD5 "1fb2595d2a905a9e820c976a1d8348bc"

SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/zmq/src"
BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/zmq/build"
STAMP_DIR "${CMAKE_CURRENT_BINARY_DIR}/zmq/build-stamp"
PREFIX ${CMAKE_CURRENT_BINARY_DIR}/zmq/
INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/zmq/install
BUILD_BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/zmq/install/lib/${LIBZMQ_STATIC}
CMAKE_ARGS "-DZMQ_BUILD_TESTS=OFF" "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/zmq/install/"
)

ExternalProject_Get_Property(zeromq install_dir)

set_property(TARGET libzmq::static
             PROPERTY IMPORTED_LOCATION "${install_dir}/lib/libzmq-static.a")

if( UNIX)
    target_link_libraries( libzmq::static INTERFACE pthread)
endif()

set_property(TARGET libzmq::static
             PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${install_dir}/include")


find_package( PlantUML REQUIRED)
find_package( SphinxBuild REQUIRED)

#if( UNIX )
#  include_directories(/usr/include)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#endif( )

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
      BUILD_COMMAND make
      INSTALL_COMMAND make install
      BUILD_BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/protobuf/install/lib/libprotobuf.a ${CMAKE_CURRENT_BINARY_DIR}/protobuf/install/bin/protoc
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
#add_subdirectory(doc)
add_subdirectory(test)

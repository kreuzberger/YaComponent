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
    set (LIBZMQ_STATIC libzmq.a)
else()
    set (LIBZMQ_STATIC libzmq.lib)
endif()

ExternalProject_Add(zeromq
URL "${CMAKE_SOURCE_DIR}/bin/zeromq-4.3.4.tar.gz"
URL_HASH SHA512=e198ef9f82d392754caadd547537666d4fba0afd7d027749b3adae450516bcf284d241d4616cad3cb4ad9af8c10373d456de92dc6d115b037941659f141e7c0e

SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/zmq/src"
BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/zmq/build"
STAMP_DIR "${CMAKE_CURRENT_BINARY_DIR}/zmq/build-stamp"
PREFIX ${CMAKE_CURRENT_BINARY_DIR}/zmq/
INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/zmq/install
BUILD_BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/zmq/install/lib/${LIBZMQ_STATIC}
CMAKE_ARGS "-DZMQ_BUILD_TESTS=OFF" "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/zmq/install/"
)

ExternalProject_Get_Property(zeromq install_dir)


add_dependencies(libzmq::static zeromq)

set_property(TARGET libzmq::static
             PROPERTY IMPORTED_LOCATION "${install_dir}/lib/libzmq.a")

if( UNIX)
    target_link_libraries( libzmq::static INTERFACE pthread)
endif()

file( MAKE_DIRECTORY "${install_dir}/include")
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

add_executable(protobuf::protoc IMPORTED GLOBAL)
add_library(protobuf::lib STATIC IMPORTED GLOBAL)

if( UNIX )
    include(ProcessorCount)
    ProcessorCount(N)

    ExternalProject_Add(protobuf
      #SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.."
      URL "${CMAKE_SOURCE_DIR}/bin/protobuf-python-3.17.3.tar.gz"
      #URL_HASH SHA512=efad397f5cdda6639d5e9980fe6eeadc1ef768bf1d96b1e528a1d7ba1d81ceb49e22cbd78d9b4ab3518236055140568342ff138204b4a47234fb2957a89d2db8
      URL_HASH SHA512=833e9fb80498d05b6d2e86391ac8e1def873e142ef8be0cc5604e66b542e88bcf1363ad6f5a85d70a0e25f9ce8b1bbddfb4d2d8a3d98f2974dcd7b7f286d989f
      STAMP_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf/build-stamp"
      SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf/src"
      SOURCE_SUBDIR "cmake"
      BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf/build"
      INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/protobuf/install
      PREFIX ${CMAKE_CURRENT_BINARY_DIR}/protobuf/
      LOG_MERGED_STDOUTERR TRUE
      #CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/protobuf/src/configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/protobuf/install
      #BUILD_COMMAND make -j ${N}
      #INSTALL_COMMAND make install
      CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/protobuf/install/"

      BUILD_BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/protobuf/install/lib/libprotobuf.a ${CMAKE_CURRENT_BINARY_DIR}/protobuf/install/bin/protoc
    )

    ExternalProject_Get_Property(protobuf install_dir)
    set_property(TARGET protobuf::lib
                 PROPERTY IMPORTED_LOCATION "${install_dir}/lib/libprotobuf.a"
    )
    set_property(TARGET protobuf::protoc
                 PROPERTY IMPORTED_LOCATION "${install_dir}/bin/protoc"
    )

    file( MAKE_DIRECTORY "${install_dir}/include")

    set_property(TARGET protobuf::lib
              PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${install_dir}/include")
endif()

add_dependencies( protobuf::lib protobuf )
add_dependencies( protobuf::protoc protobuf )

include(${CMAKE_SOURCE_DIR}/cmake/Protobuf.cmake)
add_definitions(-DZMQ_STATIC)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/cpp")

#installation default prefix like for old buildsys
set( CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE STRING "" FORCE)


add_subdirectory(src)
#add_subdirectory(doc)
add_subdirectory(test)

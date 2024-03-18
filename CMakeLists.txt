cmake_minimum_required(VERSION 3.22)
project(YaComponent)

find_package( Qt5 COMPONENTS Core Test REQUIRED)
include(GNUInstallDirs)

# Force C++ standard, do not fall back, do not use compiler extensions
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_DISABLE_PRECOMPILE_HEADERS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" )

option( WITH_BUNDLED_EXTERNALS "use bundled external dependencies" ON)

set( CMAKE_POSITION_INDEPENDENT_CODE ON)
if(WITH_BUNDLED_EXTERNALS)
    include(FetchContent)

    option( ZMQ_BUILD_TESTS OFF FORCE)
    option( ENABLE_PRECOMPILED OFF)
    FetchContent_Declare(
        zeromq
        URL "${CMAKE_SOURCE_DIR}/bin/zeromq-4.3.5.tar.gz"
        URL_HASH SHA512=a71d48aa977ad8941c1609947d8db2679fc7a951e4cd0c3a1127ae026d883c11bd4203cf315de87f95f5031aec459a731aec34e5ce5b667b8d0559b157952541
    )

    FetchContent_MakeAvailable(zeromq)

    find_package(ZeroMQ REQUIRED PATHS ${zeromq_BINARY_DIR})


    set(ABSL_PROPAGATE_CXX_STD ON CACHE INTERNAL "ABSL_FLAGS")
    set(ABSL_ENABLE_INSTALL ON)
    set(ABSL_CMAKE_BUILD_SHARED OFF)

    FetchContent_Declare(
        abseil
        URL "${CMAKE_SOURCE_DIR}/bin/abseil-cpp-20240116.1.tar.gz"
        URL_HASH SHA512=41504899ac4fd4a6eaa0a5fdf27a7765ec81962fb99b6a07982ceed32c5289e9eb12206c83a70fd44c5c3e1b96c2bfa160eb12f1dbbb45f1109d632c7690de90
    )

    FetchContent_MakeAvailable(abseil)

    set( ABSL_ROOT_DIR ${abseil_SOURCE_DIR})
    set( protobuf_MSVC_STATIC_RUNTIME OFF )
    set( protobuf_ABSL_PROVIDER module)
    set( protobuf_BUILD_TESTS off)

    FetchContent_Declare(protobuf
        URL "${CMAKE_SOURCE_DIR}/bin/protobuf-25.3.tar.gz"
        URL_HASH SHA512=1f73e237c919082e5423ae9e2ea8813dccf672c059051d1531fe89ffaa45872d3cf3052b8c3af26f674296ec17d7dc861c67b8f0834ed80261ce4a6a14ed7115
    )

    FetchContent_MakeAvailable(abseil protobuf)
else()
    find_package(ZeroMQ REQUIRED PATHS ${ZeroMQ_DIR} NO_DEFAULT_PATH)
    find_package(absl REQUIRED PATHS ${absl_DIR} NO_DEFAULT_PATH)
    find_package(utf8_range REQUIRED PATHS ${Protobuf_DIR} NO_DEFAULT_PATH)
    find_package(Protobuf REQUIRED PATHS ${Protobuf_DIR} NO_DEFAULT_PATH)
endif()

find_package( PlantUML REQUIRED)
find_package( SphinxBuild REQUIRED)

enable_testing()
include(CTest)

include(ProcessorCount)
ProcessorCount(N)
set(CMAKE_INSTALL_MESSAGE "LAZY") # one of NEVER, LAZY, ALWAYS


include(${CMAKE_SOURCE_DIR}/cmake/Protobuf.cmake)
add_definitions(-DZMQ_STATIC)

#installation default prefix like for old buildsys
set( CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE STRING "" FORCE)
set( CMAKE_INSTALL_MESSAGE "LAZY" ) # one of NEVER, LAZY, ALWAYS

include( cmake/YaComponent.cmake)

add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(test)
add_subdirectory(dist)


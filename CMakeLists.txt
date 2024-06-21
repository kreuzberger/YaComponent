cmake_minimum_required(VERSION 3.22)
project(yacomponent)

find_package(
    Qt5
    COMPONENTS Core Test
    REQUIRED
)
include(GNUInstallDirs)

# Force C++ standard, do not fall back, do not use compiler extensions
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_DISABLE_PRECOMPILE_HEADERS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

option(WITH_BUNDLED_EXTERNALS "use bundled external dependencies" ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(WITH_BUNDLED_EXTERNALS)
    include(FetchContent)

    option(ZMQ_BUILD_TESTS OFF FORCE)
    option(ENABLE_PRECOMPILED OFF)
    FetchContent_Declare(
        zeromq
        URL "${CMAKE_SOURCE_DIR}/bin/zeromq-4.3.5.tar.gz"
        URL_HASH
            SHA512=a71d48aa977ad8941c1609947d8db2679fc7a951e4cd0c3a1127ae026d883c11bd4203cf315de87f95f5031aec459a731aec34e5ce5b667b8d0559b157952541
    )

    FetchContent_MakeAvailable(zeromq)

    find_package(ZeroMQ REQUIRED PATHS ${zeromq_BINARY_DIR})

    set(ABSL_PROPAGATE_CXX_STD
        ON
        CACHE INTERNAL "ABSL_FLAGS"
    )
    set(ABSL_ENABLE_INSTALL ON)
    set(ABSL_CMAKE_BUILD_SHARED OFF)

    FetchContent_Declare(
        abseil
        URL "${CMAKE_SOURCE_DIR}/bin/abseil-cpp-20240116.1.tar.gz"
        URL_HASH
            SHA512=41504899ac4fd4a6eaa0a5fdf27a7765ec81962fb99b6a07982ceed32c5289e9eb12206c83a70fd44c5c3e1b96c2bfa160eb12f1dbbb45f1109d632c7690de90
    )

    FetchContent_MakeAvailable(abseil)

    set(ABSL_ROOT_DIR ${abseil_SOURCE_DIR})
    set(protobuf_MSVC_STATIC_RUNTIME OFF)
    set(protobuf_ABSL_PROVIDER module)
    set(protobuf_BUILD_TESTS off)

    FetchContent_Declare(
        protobuf
        URL "${CMAKE_SOURCE_DIR}/bin/protobuf-26.1.tar.gz"
        URL_HASH
            SHA512=0363ac09f92d8e040491425d444c8dca0b9b430e02d2dff6e2b28a0c2b2bea0d33a47f50bc9e2e2d4e8e22b65a02009a20c0066fb89c75df93a7b703dda42ed4
    )

    FetchContent_MakeAvailable(abseil protobuf)

    if(UNIX)
        include(${protobuf_BINARY_DIR}/cmake/protobuf/protobuf-generate.cmake)
    else()
        include(${protobuf_BINARY_DIR}/cmake/protobuf-generate.cmake)
    endif()

else()
    find_package(ZeroMQ REQUIRED PATHS ${ZeroMQ_DIR} NO_DEFAULT_PATH)
    find_package(absl REQUIRED PATHS ${absl_DIR} NO_DEFAULT_PATH)
    find_package(utf8_range REQUIRED PATHS ${Protobuf_DIR} NO_DEFAULT_PATH)
    find_package(Protobuf REQUIRED PATHS ${Protobuf_DIR} NO_DEFAULT_PATH)
endif()

find_package(PlantUML REQUIRED)
find_package(SphinxBuild REQUIRED)

find_package(Python 3.11 COMPONENTS Interpreter Development)

enable_testing()
include(CTest)

if(Python_FOUND)
    find_package(PySide2 REQUIRED)
    include(cmake/PyTestTools.cmake)
    execute_process(COMMAND $<TARGET_FILE:Python::Interpreter> --version)
endif()

include(ProcessorCount)
ProcessorCount(N)
set(CMAKE_INSTALL_MESSAGE "LAZY") # one of NEVER, LAZY, ALWAYS

# include(${CMAKE_SOURCE_DIR}/cmake/Protobuf.cmake)
add_definitions(-DZMQ_STATIC)

# installation default prefix like for old buildsys
set(CMAKE_INSTALL_PREFIX
    ${CMAKE_BINARY_DIR}/install
    CACHE STRING "" FORCE
)
set(CMAKE_INSTALL_MESSAGE "LAZY") # one of NEVER, LAZY, ALWAYS

include(cmake/YaComponent.cmake)

# use address sanitizer (needs gcc >= 4.8)
set(SANI_DEFAULT OFF)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND UNIX)
    set(SANI_DEFAULT ON)
endif()

option(WITH_ASAN "Build with address sanitizer (export ASAN_OPTIONS=leak_check_at_exit=0)" ${SANI_DEFAULT})
if(WITH_ASAN AND (CMAKE_CXX_COMPILER_ID STREQUAL Clang OR (CMAKE_CXX_COMPILER_ID STREQUAL GNU
                                                           AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 4.8))
)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -fno-omit-frame-pointer -fsanitize=address")
    # usually no symlink to soname, so list some of these as well
    find_library(LIBASAN NAMES asan libasan.so.8 libasan.so.7 libasan.so.6 libasan.so.5 libasan.so.4)
else()
    unset(LIBASAN CACHE)
endif()

# use undefined behavior sanitizer (gcc >= 4.9, clang >= ?)
option(WITH_UBSAN "Build with undefined behavior sanitizer" ${SANI_DEFAULT})
if(WITH_UBSAN)
    if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR (CMAKE_CXX_COMPILER_ID STREQUAL GNU AND CMAKE_CXX_COMPILER_VERSION
                                                                                       VERSION_GREATER_EQUAL 4.9)
    )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
        # This is supposed to fix false-positives when using a derived class through a base class pointer (base class is
        # virtual); see links below. However, -fsanatize=vptr errors still remain.
        # https://stackoverflow.com/questions/57294792/c-ubsan-produces-false-positives-with-derived-objects
        # https://bugs.llvm.org/show_bug.cgi?id=39191 https://gcc.gnu.org/bugzilla/show_bug.cgi?id=80963 string(REPLACE
        # "-fvisibility=hidden" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") string(REPLACE "-fvisibility=hidden" ""
        # CMAKE_EXE_LINKER_FLAGS
        # "${CMAKE_EXE_LINKER_FLAGS}")
    endif()
endif()

add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(test)
add_subdirectory(dist)

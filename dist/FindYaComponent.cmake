# - Find YaFsmGen
#
#  USE_YAFSM               - have the yafsmgen command found

include(CMakeParseArguments)

if( NOT YaFSM_FIND_COMPONENTS)
    set( YaFSM_FIND_COMPONENTS Generator Code)
    set( YaFSM_FIND_REQUIRED_Generator TRUE )
    set( YaFSM_FIND_REQUIRED_Code TRUE )
endif()


unset (_YaFSM_REQUIRED_VARS)

if( "Generator" IN_LIST YaFSM_FIND_COMPONENTS)
    if( YaFSM_FIND_REQUIRED_Generator)
        list( APPEND _YaFSM_REQUIRED_VARS YAFSMGEN)
    endif()

    find_program(YAFSMGEN
          NAMES yafsmgen
          PATHS ${CMAKE_CURRENT_LIST_DIR}/../bin
          DOC "yafsm executable"
          NO_DEFAULT_PATH
    )

    if( NOT YAFSMGEN STREQUAL "YAFSMGEN-NOTFOUND" )
        set( YaFSM_Generator_FOUND TRUE )
    endif()
endif()

if( "Code" IN_LIST YaFSM_FIND_COMPONENTS)
    if( YaFSM_FIND_REQUIRED_Code)
        list( APPEND _YaFSM_REQUIRED_VARS YAFSM_CODE_INCLUDE_DIR YAFSM_CODE_SRC_DIR)
    endif()

    find_path( YAFSM_CODE_INCLUDE_DIR
          NAMES IScxmlFSMEvent.h
          PATHS $<TARGET_FILE:YAFSMGEN>/../src/cpp/inc
          DOC "yafsm inc directory"
          NO_DEFAULT_PATH
    )
    find_path( YAFSM_CODE_SRC_DIR
          NAMES ScxmlFSMEvent.h
          PATHS $<TARGET_FILE:YAFSMGEN>/../src/cpp/qt
          DOC "yafsm src directory"
          NO_DEFAULT_PATH
    )

    set ( YaFSM_Code_FOUND TRUE)
    if( YAFSM_CODE_INCLUDE_DIR STREQUAL "YAFSM_CODE_INCLUDE_DIR-NOTFOUND")
        set( YaFSM_Code_FOUND FALSE)
    endif()
    if( YAFSM_CODE_SRC_DIR STREQUAL "YAFSM_CODE_SRC_DIR-NOTFOUND")
        set( YaFSM_Code_FOUND FALSE)
    endif()
endif()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args (YaFSM
  REQUIRED_VARS
    ${_YaFSM_REQUIRED_VARS}
  HANDLE_COMPONENTS
)

if(YaFSM_Generator_FOUND AND NOT TARGET YaFSM::Generator)
  add_executable(YaFSM::Generator IMPORTED GLOBAL)
  set_property(TARGET YaFSM::Generator
               PROPERTY IMPORTED_LOCATION "${YAFSMGEN}")
endif()

if(YaFSM_Code_FOUND AND NOT TARGET YaFSM::Code)
  add_library(YaFSM::Code INTERFACE IMPORTED GLOBAL )
  set_property(TARGET YaFSM::Code
               PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${YAFSM_CODE_INCLUDE_DIR}")
  set_property(TARGET YaFSM::Code
                PROPERTY INTERFACE_SOURCES "${YAFSM_CODE_SRC_DIR}")
endif()



include( ${CMAKE_CURRENT_LIST_DIR}/YaFSM.cmake)

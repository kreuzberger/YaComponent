# - Find plantuml program libraries
#
#  PLANTUML_FOUND               - have the plantuml jar found


FIND_FILE(PLANTUML_JAR
            NAMES plantuml.jar
            PATHS  ${CMAKE_CURRENT_LIST_DIR}/../bin
         )

if(NOT WIN32)
  FIND_PROGRAM(PLANTUML_EXE plantuml )
else()
  FIND_PROGRAM(PLANTUML_EXE
      NAMES plantuml.exe plantuml.cmd plantuml.bat
  )
endif()



if( PLANTUML_JAR STREQUAL "" AND PLANTUML_EXE STREQUAL "")
  set( PLANTUML_FOUND false )
else()
  if( NOT PLANTUML_EXE STREQUAL "")
    set( PLANTUML_CMD ${PLANTUML_EXE} CACHE FILEPATH "PlantUML command")
  else()
    find_package(Java REQUIRED)
    set( PLANTUML_CMD ${Java_JAVA_EXECUTEABLE} -jar ${PLANTUML_JAR} CACHE FILEPATH "PlantUML command")
  endif()

  set( PLANTUML_FOUND true )
endif()

MARK_AS_ADVANCED(
  PLANTUML_JAR
  PLANTUML_EXE
)

SET(PLANTUML_INCLUDE_DIRS)
SET(PLANTUML_LIBRARIES "${PLANTUML_JAR}")


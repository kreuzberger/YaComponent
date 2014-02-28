# - Find plantuml program libraries
#
#  PLANTUML_FOUND               - have the plantuml jar found


FIND_FILE(PLANTUML_JAR
            NAMES plantuml.jar
            PATHS  ${CMAKE_CURRENT_LIST_DIR}/../bin
         )

if( PLANTUML_JAR STREQUAL "" )
  set( PLANTUML_FOUND false )
else()
  set( PLANTUML_FOUND true )
endif()

SET(PLANTUML_INCLUDE_DIRS)
SET(PLANTUML_LIBRARIES "${PLANTUML_JAR}")


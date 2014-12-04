# - Find sphinx-build program
#
#  USE_DOC_SPHINX               - have the sphinx-build command found


set(USE_DOC_SPHINX true)
find_program(SPHINX_BUILD_EXECUTABLE sphinx-build PATH_SUFFIXES bin)
if (NOT SPHINX_BUILD_EXECUTABLE)
  message(STATUS "sphinx-build was not found, the documentation will not be generated.")
  set(USE_DOC_SPHINX false)
endif()



if (USE_DOC_SPHINX)
  macro (SPHINX_SLIDE_GENERATE outtarget)
    set( SPHINX_CLEAN_FILES  "${CMAKE_CURRENT_BINARY_DIR}/slides ${CMAKE_CURRENT_BINARY_DIR}/doctrees")
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/slides ${CMAKE_CURRENT_BINARY_DIR}/doctrees
      COMMAND ${SPHINX_BUILD_EXECUTABLE} -b slides  -d ${CMAKE_CURRENT_BINARY_DIR}/doctrees ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/slides
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}
      COMMENT "Built sphinx documentation for ${CMAKE_CURRENT_SOURCE_DIR}")
    add_custom_target( doc_${outtarget} ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/slides)
    set_directory_properties( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${SPHINX_CLEAN_FILES}" )
  endmacro( )

  macro (SPHINX_DOC_GENERATE outtarget)
    set( SPHINX_CLEAN_FILES  "${CMAKE_CURRENT_BINARY_DIR}/html ${CMAKE_CURRENT_BINARY_DIR}/doctrees-html")
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/html ${CMAKE_CURRENT_BINARY_DIR}/doctrees-html
      COMMAND ${SPHINX_BUILD_EXECUTABLE} -b html  -d ${CMAKE_CURRENT_BINARY_DIR}/doctrees-html ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/html
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}
      COMMENT "Built sphinx documentation for ${CMAKE_CURRENT_SOURCE_DIR}")

    add_custom_target( doc_${outtarget} ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/html)
    set_directory_properties( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${SPHINX_CLEAN_FILES}" )
  endmacro( )
endif()

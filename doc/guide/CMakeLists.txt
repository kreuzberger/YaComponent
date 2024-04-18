project (UserGuide)

add_subdirectory( examples )

if(USE_DOC_SPHINX)
  SPHINX_DOC_GENERATE(UserGuide BUILDERS HTML)
endif()

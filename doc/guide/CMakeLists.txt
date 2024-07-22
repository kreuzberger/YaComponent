project(UserGuide)

add_subdirectory(examples)

if(USE_DOC)
    sphinx_doc_generate(UserGuide BUILDERS HTML DEPENDS python-update)
endif()

project(UserGuide)

add_subdirectory(examples)

if(USE_DOC)
    configure_file(conf.py.in ${CMAKE_CURRENT_SOURCE_DIR}/conf.py @ONLY)
    sphinx_doc_generate(UserGuide BUILDERS HTML DEPENDS python-update)
endif()

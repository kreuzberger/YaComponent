project(CommunicationSlides)

if(USE_DOC)
    configure_file(conf.py.in ${CMAKE_CURRENT_SOURCE_DIR}/conf.py @ONLY)
    sphinx_doc_generate(CommunicationSlides BUILDERS REVEALJS DEPENDS python-update)
endif()

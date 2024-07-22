project(CommunicationSlides)

if(USE_DOC)
    sphinx_doc_generate(CommunicationSlides BUILDERS REVEALJS DEPENDS python-update)
endif()

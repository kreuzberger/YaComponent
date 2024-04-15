
set(CPACK_PACKAGE_NAME "${BRANDING_NAME_LC}")
set(CPACK_ARCHIVE_THREADS 0)
set(CPACK_THREADS 0) # use all cores if archive compression supports this

set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_SOURCE_IGNORE_FILES
    "/build/;\\\\.git/;.*txt.user;CPackSource.*"
)


set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)

set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_GENERATOR TGZ)

add_custom_target(dist-src COMMAND cpack --config ${CMAKE_BINARY_DIR}/CPackSourceConfig.cmake)
add_custom_target(dist COMMAND cpack --config ${CMAKE_BINARY_DIR}/CPackConfig.cmake)

if( WITH_BUNDLED_EXTERNALS)
else()
    set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME yacomponent)
endif()
include(CPack)

#install( TARGETS yacomplayout RUNTIME DESTINATION "yacomponent/bin" COMPONENT "yacomponent")
#install( TARGETS yacomponent RUNTIME DESTINATION "yacomponent/bin" COMPONENT "yacomponent")
#install( TARGETS YaComponentLib ARCHIVE DESTINATION "yacomponent/lib" COMPONENT "yacomponent")
#install( DIRECTORY ${YaComponentLib_SOURCE_DIR}/include/ DESTINATION "yacomponent/include" COMPONENT "yacomponent")
#install( FILES FindYaComponent.cmake ${YaComponent_SOURCE_DIR}/cmake/YaComponent.cmake DESTINATION "yacomponent/lib/cmake/YaComponent" COMPONENT "yacomponent")

set(CPACK_PACKAGE_NAME "${BRANDING_NAME_LC}")
#string(TOLOWER "${CPACK_PACKAGE_NAME}-${BRANDING_VERSION}-${CMAKE_HOST_SYSTEM_NAME}" CPACK_PACKAGE_FILE_NAME)
#set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${BRANDING_NAME_LC} ${BRANDING_VERSION}")

#set(CPACK_SOURCE_PACKAGE_NAME "${CPACK_PACKAGE_NAME}-${BRANDING_VERSION}-source")
#set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-source")
set(CPACK_ARCHIVE_THREADS 0)
set(CPACK_THREADS 0) # use all cores if archive compression supports this
#set(CPACK_SOURCE_PACKAGE_VERSION ${BRANDING_VERSION})
#set(CPACK_SOURCE_PACKAGE_VERSION_MAJOR "${BRANDING_VERSION_MAJOR}")
#set(CPACK_SOURCE_PACKAGE_VERSION_MINOR "${BRANDING_VERSION_MINOR}")
#set(CPACK_SOURCE_PACKAGE_VERSION_PATCH "${BRANDING_VERSION_BUILD}")

#set(CPACK_PACKAGE_VERSION ${BRANDING_VERSION})
#set(CPACK_PACKAGE_VERSION_MAJOR "${BRANDING_VERSION_MAJOR}")
#set(CPACK_PACKAGE_VERSION_MINOR "${BRANDING_VERSION_MINOR}")
#set(CPACK_PACKAGE_VERSION_PATCH "${BRANDING_VERSION_BUILD}")

#configure_file(CPackSourceProperties.cmake.in CPackSourceProperties.cmake @ONLY)

set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_SOURCE_IGNORE_FILES
    "/build/;\\\\.git/;.*txt.user;CPackSource.*"
)

#set(CPACK_PACKAGE_CONTACT "${BRANDING_CONTACT_MAIL}")

set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)

#set(CPACK_PROPERTIES_FILE ${CMAKE_BINARY_DIR}/CPackSourceProperties.cmake)
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_GENERATOR TGZ)
# set( CPACK_COMPONENTS_ALL )

add_custom_target(dist-src COMMAND cpack --config ${CMAKE_BINARY_DIR}/CPackSourceConfig.cmake)
add_custom_target(dist COMMAND cpack --config ${CMAKE_BINARY_DIR}/CPackConfig.cmake)

if( WITH_BUNDLED_EXTERNALS)
    add_custom_target(
        install-yacomponent
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_COMPONENT="yacomponent" -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_COMPONENT="Unspecified" -D"CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/yacomponent" -P${ZeroMQ_BINARY_DIR}/cmake_install.cmake
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_COMPONENT="protoc" -D"CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/yacomponent" -P${protobuf_BINARY_DIR}/cmake_install.cmake
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_COMPONENT="libprotobuf" -D"CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/yacomponent" -P${protobuf_BINARY_DIR}/cmake_install.cmake
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_COMPONENT="libprotobuf-lite" -D"CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/yacomponent" -P${protobuf_BINARY_DIR}/cmake_install.cmake
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_COMPONENT="protobuf-headers" -D"CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/yacomponent" -P${protobuf_BINARY_DIR}/cmake_install.cmake
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_COMPONENT="protobuf-export" -D"CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/yacomponent" -P${protobuf_BINARY_DIR}/cmake_install.cmake

        # for utf8_range
        #COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_COMPONENT="Unspecified" -D"CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/yacomponent" -P${protobuf_BINARY_DIR}/cmake_install.cmake
        #for abseil
        #COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_COMPONENT="Unspecified" -D"CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/yacomponent" -P${absl_BINARY_DIR}/cmake_install.cmake

        # COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_COMPONENT=${MODULE_FRAMEWORK_NAME}-modules -P
        # ${CMAKE_BINARY_DIR}/cmake_install.cmake
    )
else()
    add_custom_target(
        install-yacomponent
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_COMPONENT="yacomponent" -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
        # COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_COMPONENT=${MODULE_FRAMEWORK_NAME}-modules -P
        # ${CMAKE_BINARY_DIR}/cmake_install.cmake
    )
endif()
include(CPack)

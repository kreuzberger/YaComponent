set(INCLUDE_INSTALL_DIR "include")
set(LIBRARY_INSTALL_DIR "lib")
set(LIBRARY_VERSION "24.1")
set(LIBRARY_SYSTEM_NAME "${CMAKE_HOST_SYSTEM_NAME}")

include(CMakePackageConfigHelpers)

set(QT_VERSION ${Qt5Core_VERSION})

configure_package_config_file(
    YaComponentConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/YaComponentConfig.cmake
    INSTALL_DESTINATION yacomponent/lib/YaComponent/cmake
    PATH_VARS INCLUDE_INSTALL_DIR LIBRARY_INSTALL_DIR LIBRARY_VERSION LIBRARY_SYSTEM_NAME
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/YaComponentConfigVersion.cmake
    VERSION ${LIBRARY_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    EXPORT "YaComponentTargets"
    FILE "YaComponentTargets${LIBRARY_SYSTEM_NAME}.cmake"
    NAMESPACE YaComponent::
    DESTINATION lib/cmake/YaComponent
    COMPONENT "yacomponent"
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/YaComponentConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/YaComponentConfigVersion.cmake
    DESTINATION lib/cmake/YaComponent
    COMPONENT "yacomponent"
)

add_subdirectory(lib)
add_subdirectory(bin)

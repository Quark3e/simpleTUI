cmake_minimum_required(VERSION 3.14)

project(simpleTUI VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(simpleTUI)

target_sources(simpleTUI
    PRIVATE
        src/simpleTUI.cpp
)
target_include_directories(simpleTUI
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/Pos2d/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)


set_target_properties(simpleTUI PROPERTIES
    PUBLIC_HEADER "include/simpleTUI.hpp"
)

install(DIRECTORY include/ DESTINATION include/)

install(DIRECTORY lib/Pos2d/include/ DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)

message(STATUS "---------1-")

install(TARGETS simpleTUI
    EXPORT simpleTUITargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

install(EXPORT simpleTUITargets
    FILE simpleTUIConfig.cmake
    NAMESPACE simpleTUI::
    DESTINATION lib/cmake/simpleTUI
)


message(STATUS "---------2-")

include(CMakePackageConfigHelpers)
# configure_package_config_file(
#     ${CMAKE_CURRENT_SOURCE_DIR}/simpleTUIConfig.cmake.in
#     ${CMAKE_CURRENT_BINARY_DIR}/simpleTUIConfig.cmake
#     INSTALL_DESTINATION lib/cmake/simpleTUI
# )
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/simpleTUIConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/simpleTUIConfig.cmake
    INSTALL_DESTINATION lib/cmake/simpleTUI
)


message(STATUS "---------3-")

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/simpleTUIConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/simpleTUIConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/simpleTUIConfigVersion.cmake
    DESTINATION lib/cmake/simpleTUI
)

export(EXPORT simpleTUITargets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/simpleTUITargets.cmake
)


# library build and install commands:
#       mkdir build
#       cmake -B build
#       cd build
#       cmake .. -DCMAKE_INSTALL_PREFIX="{library install dir}"
#       cmake --build . --config Release
#       cmake --install . --config Release --prefix {library install dir}

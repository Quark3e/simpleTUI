cmake_minimum_required(VERSION 3.10)
project(simpleTUI_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(simpleTUI CONFIG REQUIRED HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../install")
if (UNIX)
    set_target_properties(simpleTUI::simpleTUI PROPERTIES IMPORTED_LOCATION ${CMAKE_INSTALL_PREFIX}/lib/libsimpleTUI.a)
endif (UNIX)

# Create executable for testing simpleTUI
add_executable(test_simpletui
    main.cpp
)


# Link against any required libraries
target_link_libraries(test_simpletui PRIVATE simpleTUI::simpleTUI)


# app build commands
#       mkdir build
#       cmake -B build -DCMAKE_INSTALL_PREFIX='{install dir of library}'
#       cmake --build build --config Debug


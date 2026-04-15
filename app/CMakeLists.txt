cmake_minimum_required(VERSION 3.10)
project(simpleTUI_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(simpleTUI CONFIG REQUIRED HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../install")

# Create executable for testing simpleTUI
add_executable(test_simpletui
    main.cpp
)


# Link against any required libraries
target_link_libraries(test_simpletui PRIVATE simpleTUI::simpleTUI)


# app build commands
#       mkdir build
#       cd build
#       cmake .. -DCMAKE_INSTALL_PREFIX
#       cmake -B .
#       cmake --build . --config Debug
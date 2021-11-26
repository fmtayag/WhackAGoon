#
# Copyright (c) 2018, 2019 Amine Ben Hassouna <amine.benhassouna@gmail.com>
#
# Permission is hereby granted, free of charge, to any
# person obtaining a copy of this software and associated
# documentation files (the "Software"), to deal in the
# Software without restriction, including without
# limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software
# is furnished to do so, subject to the following
# conditions:
#
# The above copyright notice and this permission notice
# shall be included in all copies or substantial portions
# of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
# ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
# TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
# SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
# IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the project name
project(WhackAGoon)

# Set the cmake minimum version to 3.5.1
cmake_minimum_required(VERSION 3.5.1)

# Add SDL2 CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Add cpp source files under the src directory
add_executable(${PROJECT_NAME}

    # WhackAGoon source files
	"src/main.cpp"
    "src/scenes.cpp"
    "src/sprites.cpp" 
    "src/widgets.cpp"
    "src/helpers.cpp"
    "src/managers.cpp"
    
)

# Add FMT library
find_package(fmt REQUIRED)
target_link_libraries(${PROJECT_NAME} fmt::fmt-header-only)

# Add all headers files under the include directory
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Add compiler errors/warnings flags
target_compile_options(${PROJECT_NAME} PRIVATE $<$<C_COMPILER_ID:MSVC>:/W4 /WX>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>)

# Add SDL2 library
find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::Main)

# Add SDL2_image librar
find_package(SDL2_image REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::Image)

# Add SDL2_ttf library
find_package(SDL2_ttf REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::TTF)

# Add SDL2_mixer library
find_package(SDL2_mixer REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::Mixer)

# # Copy assets
# file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/Debug)

# # Copy libraries
# file(GLOB DLLFILES
    # "libraries/*.dll"
# )
# file(COPY ${DLLFILES} DESTINATION ${CMAKE_BINARY_DIR}/Debug)
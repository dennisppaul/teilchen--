cmake_minimum_required(VERSION 3.12)

project(SketchLesson04_Spring)

# set umgebung path from environment variable, on macOS e.g `export UMGEBUNG_PATH="<absolute/path/to/library>"`

set(UMGEBUNG_PATH $ENV{UMGEBUNG_PATH})
if(NOT UMGEBUNG_PATH)
    message(FATAL_ERROR "UMGEBUNG_PATH is not set.")
endif()
set(EXAMPLE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../") # set path to teilchen library

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add source + header files from this directory

file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
include_directories(".")

# add umgebung

target_compile_definitions(${PROJECT_NAME} PRIVATE UMGEBUNG_WINDOW_TITLE="${PROJECT_NAME}") # set window title
add_subdirectory(${UMGEBUNG_PATH}
        ${CMAKE_BINARY_DIR}/umgebung-lib-${PROJECT_NAME}
)
add_umgebung_libs()

# add example + library

add_subdirectory(${EXAMPLE_PATH}
        ${CMAKE_BINARY_DIR}/umgebung-teilchen-example-${PROJECT_NAME}
)
target_link_libraries(${PROJECT_NAME} PRIVATE teilchen)

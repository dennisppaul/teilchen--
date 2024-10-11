cmake_minimum_required(VERSION 3.12)

project(teilchen)

## library source files

file(GLOB LIB_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(${PROJECT_NAME} STATIC ${LIB_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )

# use `INTERFACE` if *header-only* library
#add_library(${PROJECT_NAME} INTERFACE ${LIB_SOURCE_FILES})
#target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

#target_link_libraries(${PROJECT_NAME} PUBLIC ${SOME_OTHER_LIBRARIES})

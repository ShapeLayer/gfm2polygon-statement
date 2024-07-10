cmake_minimum_required(VERSION 3.10)
project(markdown_to_polygon_statement)

set(PROGRAM "md2pst")
set(PROGRAM_HEADERS
    src/polygon.hpp
)
set(PROGRAM_SOURCES
    src/main.cpp
    src/polygon.cpp
    ${PROGRAM_HEADERS}
)

configure_file(CMakeLists.cmark-gfm.txt cmark-gfm/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cmark-gfm
)
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cmark-gfm
)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${CMAKE_BINARY_DIR}/cmark-gfm/install" CACHE PATH "DESC" FORCE)

include_directories(${CMAKE_SOURCE_DIR}/cmark-gfm/src)
include_directories(${CMAKE_SOURCE_DIR}/cmark-gfm/build/src)
include_directories(${CMAKE_SOURCE_DIR}/cmark-gfm/extensions)
include_directories(${CMAKE_SOURCE_DIR}/install/include)
include_directories(${CMAKE_SOURCE_DIR}/install/lib)

add_executable(${PROGRAM} ${PROGRAM_SOURCES})

target_link_libraries(${PROGRAM} cmark-gfm)

cmake_minimum_required(VERSION 3.10)
project(c-server)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_executable(c-server ${SOURCES})

set_target_properties(
    c-server
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

add_custom_target(
    clean-all
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/CleanAll.cmake
)

file(
    WRITE
    ${CMAKE_SOURCE_DIR}/CleanAll.cmake
    "
file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/bin)
file(REMOVE_RECURSE ${CMAKE_BINARY_DIR})
"
)

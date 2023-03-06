cmake_minimum_required(VERSION 3.24)

project(main)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

find_package(json-c CONFIG)

include_directories(inc)

message("[PROJECT NAME] ${PROJECT_NAME}")

file(GLOB SOURCES "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC -L/usr/local/lib -lcurl PRIVATE json-c::json-c -lpthread)

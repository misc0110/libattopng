cmake_minimum_required(VERSION 2.8.9)
project (libattopng)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb -Wall")

include_directories(.)

add_executable(libattopng test.c libattopng.c) 
target_link_libraries(libattopng)
 

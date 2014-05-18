cmake_minimum_required(VERSION 2.8)
get_filename_component(PROJECT_LOCATION ${CMAKE_SOURCE_DIR} NAME)

### Project information ###
set(VER_MAJOR "0")
set(VER_MINOR "0")
set(VER_PATCH "0")
project(PROJECT_LOCATION)

### Add or remove packages ###
set(CMAKE_C_FLAGS "-Wall -lm -ledit -std=c99")

set(VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_PATCH}")
# So we can build outside of the main directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc ${CMAKE_CURRENT_SOURCE_DIR})

# Include files
set (PROJECT_INCLUDE_DIR include)
include_directories("${PROJECT_INCLUDE_DIR}")

file(GLOB_RECURSE files_SRC
      "./src/*.c"
)

file(GLOB_RECURSE files_INC
      "./include/*.h"
)

# Build the real deal
add_executable(alisp main.c ${files_SRC} ${files_INC})
target_link_libraries(alisp m edit)

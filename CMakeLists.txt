cmake_minimum_required(VERSION 3.10)
project(a)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Clean target
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/clean-all.cmake"
)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/clean-all.cmake"
    "file(REMOVE_RECURSE \"${CMAKE_CURRENT_BINARY_DIR}/*\")\n"
)

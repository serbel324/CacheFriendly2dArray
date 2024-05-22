cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 17)

set(INCPATH ${PROJECT_SOURCE_DIR}/include)

project(CacheFriendly2dArray)

set(SOURCES
    benchmark.cpp
)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${INCPATH}
)

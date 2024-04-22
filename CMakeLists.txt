cmake_minimum_required(VERSION 3.26)
project(lr1)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_library(lrLib STATIC Grammar.cpp Lr1.cpp Parser.cpp Reader.cpp Utility.cpp Writer.cpp)

add_executable(lr1
        main.cpp)

target_link_libraries(lr1 lrLib)

if(TESTS)
    set_target_properties(lr1 PROPERTIES COMPILE_DEFINITIONS "ProjectPath=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
add_subdirectory(tests)
endif()




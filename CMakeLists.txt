cmake_minimum_required(VERSION 2.25)
project(typinterm CXX)
set(CMAKE_CXX_STANDARD 20)

#add lib
add_library(core
    src/session.cpp
)
target_include_directories(core PUBLIC src)

add_executable(typinterm src/main.cpp)
target_link_libraries(typinterm PRIVATE core)

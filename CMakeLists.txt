cmake_minimum_required(VERSION 3.20)
project(typinterm CXX)
set(CMAKE_CXX_STANDARD 20)

#find_package(Notcurses++ CONFIG REQUIRED)

#add lib
add_library(core
    src/filereader.cpp
    src/input.cpp
    src/session.cpp
    src/renderer.cpp
)

target_include_directories(core PUBLIC src)
#target_link_libraries(core PUBLIC notcurses++)

add_executable(typinterm src/main.cpp)
target_link_libraries(typinterm PRIVATE core)
# ------------------------------------------
include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(catch2)

# ------------------------------------------
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)
add_executable(tests ${TEST_SOURCES})

target_link_libraries(tests
    PRIVATE
        core
        Catch2::Catch2WithMain
)

# ------------------------------------------
include(CTest)
include(Catch)
catch_discover_tests(tests)

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

cmake_minimum_required(VERSION 3.20)
set(CMAKE_C_COMPILER   clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
project(typinterm CXX)
if(POLICY CMP0135)
  # 展開時刻を使う (= NEW)。古い CMake でもエラーにならない書き方
  cmake_policy(SET CMP0135 NEW)
endif()

#find_package(Notcurses++ CONFIG REQUIRED)

#add lib
add_library(core
    src/filereader.cpp
    src/input.cpp
    src/session.cpp
    src/renderer.cpp
    src/llm_exerciseMaterial.cpp
    src/savematerial.cpp
)

target_include_directories(core PUBLIC src)
#target_link_libraries(core PUBLIC notcurses++)

add_executable(typinterm src/main.cpp)
target_link_libraries(typinterm PRIVATE core)

#-------------------------------------------------------------
include(FetchContent)
#-------------------------------------------------------------
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG dd967cb48ea6bcbad9f1da5ada0db8ac0d532c06) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)
target_link_libraries(typinterm PRIVATE cpr::cpr)
#-------------------------------------------------------------
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)
target_link_libraries(typinterm PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(core
  PUBLIC             # ← PUBLIC にすると include も下流へ伝搬
    cpr::cpr
    nlohmann_json::nlohmann_json
)

#----------------------------------------------------------------------------------
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

include(CTest)
include(Catch)
catch_discover_tests(tests)

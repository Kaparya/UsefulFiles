cmake_minimum_required(VERSION 3.24)
project(GameLife)

set(CMAKE_CXX_STANDARD 23)

add_executable(GameLife main.cpp GrassActions.cpp FieldActions.cpp BunnyLogic.cpp WolvesLogic.cpp)

include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(GameLife sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
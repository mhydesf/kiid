cmake_minimum_required(VERSION 3.17)

set(PROJECT kiid)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

project(${PROJECT})
find_package(Qt6 COMPONENTS Core Widgets Gui REQUIRED)

add_subdirectory(vendor/json)
include_directories(vendor/json/include)

add_executable(${PROJECT} main.cpp)

target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    nlohmann_json::nlohmann_json
)


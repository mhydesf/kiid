cmake_minimum_required(VERSION 3.17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Default build type: Debug" FORCE)
endif()

set(PROJECT kiid)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-fPIC)

project(${PROJECT})

find_package(Qt6 COMPONENTS Core Widgets Gui REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/vendor)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/vendor/json/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/fmt/include)

file(GLOB_RECURSE LIB_HEADERS "src/*.h")
file(GLOB_RECURSE LIB_SOURCES "src/*.h")
add_library(kiid_lib STATIC ${LIB_SOURCES} ${LIB_HEADERS})

add_executable(${PROJECT} main.cpp)

target_link_libraries(${PROJECT_NAME}
    kiid_lib
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    nlohmann_json::nlohmann_json
    fmt::fmt
)


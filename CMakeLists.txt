cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(VideoPlayer VERSION 1.0.0)

file(GLOB SOURCES
	src/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    include
    pl_mpeg
)

if (PROJECT_IS_TOP_LEVEL) 
    target_compile_definitions(${PROJECT_NAME} PRIVATE _VIDEO_PLAYER_EXPORTING)
endif()

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} $ENV{GEODE_SDK}/build)

# if(WIN32)
#     target_link_libraries(${PROJECT_NAME} opengl32)
# elseif(APPLE)
#     target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
# elseif(ANDROID)
#     target_link_libraries(${PROJECT_NAME} GLESv2)
# endif()

setup_geode_mod(${PROJECT_NAME})

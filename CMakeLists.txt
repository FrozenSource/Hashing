cmake_minimum_required(VERSION 3.18)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)
FetchContent_Declare(SetupHunter GIT_REPOSITORY https://github.com/cpp-pm/gate)
FetchContent_MakeAvailable(SetupHunter)

function(InstallVendor project_name git_repo)
        FetchContent_Declare(
                ${project_name}
                GIT_REPOSITORY ${git_repo}
                SOURCE_DIR "${PROJECT_SOURCE_DIR}/vendor/${project_name}"
                BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}
                SUBBUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}
        )
        FetchContent_GetProperties(${project_name})
        if(NOT "${project_name}_POPULATED")
                FetchContent_Populate(${project_name})
                message(STATUS "Downloaded ${project_name}")
                if(EXISTS "${PROJECT_SOURCE_DIR}/vendor/${project_name}/CMakeLists.txt")
                        message(STATUS "Configuring: ${project_name}")
                        add_subdirectory("${PROJECT_SOURCE_DIR}/vendor/${project_name}")
                endif()
                message(STATUS "Installed ${project_name}")
        else()
                message(STATUS "Found ${project_name}")
        endif()
endfunction()

set(This Hashing)
project(${This} LANGUAGES C CXX)

InstallVendor(Core https://CorrupD:hiddenhand@git.nipqer.nl/CorrupD/Core.git)
InstallVendor(BLAKE2 https://github.com/BLAKE2/BLAKE2.git)
file(GLOB_RECURSE BLAKE2_HEADERS RELATIVE ${PROJECT_SOURCE_DIR} "vendor/BLAKE2/ref/*.h")
file(GLOB_RECURSE BLAKE2_SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "vendor/BLAKE2/ref/*.c")
set(BLAKE2_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/vendor/BLAKE2/ref/")

set(Headers
        ${PROJECT_SOURCE_DIR}/include/Hashing.hpp)
set(Sources
        ${PROJECT_SOURCE_DIR}/src/Hashing.cpp)
        
add_library(${This} ${Headers} ${Sources})
target_link_libraries(${This} PUBLIC Core)
target_include_directories(${This} PUBLIC ${PROJECT_SOURCE_DIR}/include ${BLAKE2_INCLUDE_DIR})

if (WIN32)
        target_compile_definitions(${This} PUBLIC BUILD _CRT_SECURE_NO_WARNINGS)
endif()
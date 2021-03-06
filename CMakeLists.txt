cmake_minimum_required(VERSION 3.18)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)
FetchContent_Declare(SetupHunter GIT_REPOSITORY https://github.com/cpp-pm/gate)
FetchContent_MakeAvailable(SetupHunter)

if(NOT InstallVendor)
function(InstallVendor project_name git_repo branch)
        IF (NOT ${project_name})
                FetchContent_Declare(
                        ${project_name}
                        GIT_REPOSITORY ${git_repo}
                        GIT_TAG ${branch}
                        SOURCE_DIR "${PROJECT_SOURCE_DIR}/vendor/${project_name}"
                        BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${project_name}"
                        SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/${project_name}-sub"
                )
                FetchContent_MakeAvailable(${project_name})
                set(${project_name} CACHE INTERNAL "")
                message(STATUS "Found ${project_name} @ ${branch}")
        endif()
endfunction()
endif()

set(This Hashing)
project(${This} LANGUAGES C CXX)

InstallVendor(BLAKE2 https://github.com/BLAKE2/BLAKE2.git master)
file(GLOB_RECURSE BLAKE2_HEADERS "${PROJECT_SOURCE_DIR}/vendor/BLAKE2/ref/*.h")
file(GLOB_RECURSE BLAKE2_SOURCES "${PROJECT_SOURCE_DIR}/vendor/BLAKE2/ref/*.c")
set(BLAKE2_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/vendor/BLAKE2/ref/")

set(Headers
        ${PROJECT_SOURCE_DIR}/include/Hashing.hpp)
set(Sources
        ${PROJECT_SOURCE_DIR}/src/Hashing.cpp)
        
add_library(${This} ${BLAKE2_HEADERS} ${BLAKE2_SOURCES} ${Headers} ${Sources})
target_include_directories(${This} PUBLIC ${PROJECT_SOURCE_DIR}/include ${BLAKE2_INCLUDE_DIR})

if (WIN32)
        target_compile_definitions(${This} PUBLIC BUILD _CRT_SECURE_NO_WARNINGS)
endif()
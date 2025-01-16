cmake_minimum_required(VERSION 3.28)

# Project
if(DEFINED hawsoo_sentou_goraku_engine_GUARD)
    if(NOT hawsoo_sentou_goraku_engine_GUARD STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
        return() # Project has been already included by someone else
    endif()
else()
    set(hawsoo_sentou_goraku_engine_GUARD ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "hawsoo_sentou_goraku_engine guard")
endif()
project(hawsoo_sentou_goraku_engine
    VERSION 0.1.0
    LANGUAGES CXX
)

# C/C++ standard.
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Multithreaded build.
set(CMAKE_BUILD_PARALLEL_LEVEL 16)
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# Dependencies.
set(monolithic_renderer_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/components/monolithic_renderer/include)
add_subdirectory(components/monolithic_renderer)

set(multithreaded_job_system_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/components/multithreaded_job_system/include)
add_subdirectory(components/multithreaded_job_system)

set(world_entity_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/components/world_entity/include)
add_subdirectory(components/world_entity)

# Static library build.
add_library(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine.h
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${monolithic_renderer_INCLUDE_DIR}
        ${multithreaded_job_system_INCLUDE_DIR}
        ${world_entity_INCLUDE_DIR}
        ${cglm_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    monolithic_renderer
    multithreaded_job_system
)
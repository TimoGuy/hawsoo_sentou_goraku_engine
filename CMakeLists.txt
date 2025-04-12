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
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Multithreaded build.
set(CMAKE_BUILD_PARALLEL_LEVEL 16)
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# Dependencies.
add_compile_definitions(CGLM_FORCE_DEPTH_ZERO_TO_ONE)  # Force cglm to Vulkan bounds.
set(cglm_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/cglm/include)  # @NOTE: header only.
set(input_handling_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/components/input_handling/include)
set(monolithic_renderer_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/components/monolithic_renderer/include)
set(multithreaded_job_system_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/components/multithreaded_job_system/include)
set(ticking_world_simulation_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/components/ticking_world_simulation/include
    ${CMAKE_CURRENT_SOURCE_DIR}/components/ticking_world_simulation/third_party/JoltPhysics)
set(timing_reporter_macro_magic_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/components/timing_reporter_macro_magic/include)  # @NOTE: header only.
set(world_entity_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/components/world_entity/include)

add_subdirectory(components/input_handling)
add_subdirectory(components/monolithic_renderer)
add_subdirectory(components/multithreaded_job_system)
add_subdirectory(components/ticking_world_simulation)
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
        ${cglm_INCLUDE_DIR}
        ${monolithic_renderer_INCLUDE_DIR}
        ${multithreaded_job_system_INCLUDE_DIR}
        ${ticking_world_simulation_INCLUDE_DIR}
        ${world_entity_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    input_handling
    monolithic_renderer
    multithreaded_job_system
    ticking_world_simulation
    world_entity
)

# Create symlink to assets folder.
if(DEFINED ASSET_DIR)
    set(dir_name ${ASSET_DIR})
    set(target_dir_name ${CMAKE_BINARY_DIR}/assets)
    if(WIN32)
        string(REPLACE "/" "\\" win32_dir_name ${dir_name})
        string(REPLACE "/" "\\" win32_target_dir_name ${target_dir_name})
        if(EXISTS ${dir_name})
            execute_process(
                COMMAND cmd /C rmdir ${win32_target_dir_name}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
        endif()
        execute_process(
            COMMAND cmd /C mklink /J ${win32_target_dir_name} ${win32_dir_name}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    else()
        # @TODO: Probably add the delete symlink code right here?
        file(CREATE_LINK ${dir_name} ${target_dir_name} SYMBOLIC)
    endif()
else()
    message(FATAL_ERROR "Asset folder location must be defined in `ASSET_DIR`")
endif()

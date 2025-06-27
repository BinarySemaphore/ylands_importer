cmake_minimum_required(VERSION 3.10.0)
project(YlandsImporter VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_SKIP_INSTALL_RULES YES)
if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/Ox")
endif()


#######################################
# Primary CLI Program
#######################################
set(target_1_name importer)
set(target_1_sources
    main.cpp
    config.cpp
    utils.cpp
)
list(TRANSFORM target_1_sources PREPEND "src/")

add_executable(${target_1_name} ${target_1_sources})
target_compile_features(${target_1_name} PRIVATE cxx_std_17)
if (MSVC)
    target_compile_options(${target_1_name} PRIVATE /W3)
endif()
set_target_properties(${target_1_name} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug/base
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release/base
)


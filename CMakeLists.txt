cmake_minimum_required(VERSION 3.16)
project(KMeansCPP LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)

    set(CMAKE_CXX_FLAGS_DEBUG "-g") # Debug build: Includes debugging symbols
    set(CMAKE_CXX_FLAGS_RELEASE "-O3") # Release build: Includes optimization for performance
elseif (MSVC)
    add_compile_options(/W4 /EHsc) # Use cl.exe (Visual Studio compiler) on Windows
endif()

find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if (CLANG_TIDY_EXE)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
else()
    message(WARNING "clang-tidy not found! Skipping linting.")
endif()

# compile_commands.json file for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(KMeansCPP
    src/main.cpp
    src/kmeansFunctions.cpp    
)

# Platform-specific configurations
if (WIN32)
    # Visual Studio generator (when on Windows)
    set(CMAKE_GENERATOR_PLATFORM x64)  # Set the target platform (you can adjust this)
    message(STATUS "Configuring for Visual Studio on Windows using cl.exe")
elseif (UNIX)
    # Default for Linux/Unix systems (using Clang or GCC)
    message(STATUS "Configuring for Linux/Unix")
endif()

# Custom command to run after build (you can change this to match your setup)
add_custom_target(run
    COMMAND $<TARGET_FILE:KMeansCPP>
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} 
    DEPENDS KMeansCPP
    COMMENT "Running the KMeansCPP application"
)

# Optionally, you can make "run" the default target after build by making it depend on the build
add_dependencies(run KMeansCPP)
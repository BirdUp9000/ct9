# ------------------------------------------------------------------
# Project configuration
# ------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16)

project(ct9 VERSION 0.0.0
    DESCRIPTION "CT9 - Prefix Tree Text Autocomplete"
    HOMEPAGE_URL "https://github.com/BirdUp9000/ct9"
    LANGUAGES CXX
)

# ------------------------------------------------------------------
# Set the C++ standard
# ------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)

# ------------------------------------------------------------------
# Generate compile commands
# ------------------------------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------------
# Default build type: If no build type is specified, use Release.
# Possible build types: Release, Develop.
# ------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

# ------------------------------------------------------------------
# Creating option for compiling GUI
#   ON  - default
# ------------------------------------------------------------------
option(BUILD_GUI "Build X11 graphical interface" ON)

# ------------------------------------------------------------------
# Console version entrypoint
# ------------------------------------------------------------------
add_executable(ct9 
    src/main.cc
    include/ct9/Trie.h
)

target_include_directories(ct9
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ------------------------------------------------------------------
# If GUI version is on, find X11 and link GUI files
# ------------------------------------------------------------------
if(BUILD_GUI)
    message(STATUS "Building CT9 with GUI support.")
    find_package(X11 REQUIRED)
    if(NOT X11_FOUND)
        message(FATAL_ERROR "X11 library not found. Please install libX11-dev.")
    endif()

    target_link_libraries(ct9 PRIVATE X11::X11)
    # Вместо CT9_GUI=1 — определяем именно BUILD_GUI=1
    target_compile_definitions(ct9 PRIVATE BUILD_GUI=1)

else()
    # Если GUI не собираем, задаём BUILD_GUI=0
    target_compile_definitions(ct9 PRIVATE BUILD_GUI=0)
endif()

# ------------------------------------------------------------------
# Develop configuration:
# Fetch tests, build them, and apply debug-related compile options.
# ------------------------------------------------------------------

if(CMAKE_BUILD_TYPE STREQUAL "Develop")
    # Debug message
    message(STATUS "Building in 'Develop' mode with sanitizers and coverage.")

    enable_testing()
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.0
    )
    FetchContent_MakeAvailable(Catch2)

    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cc"
    )
    add_executable(run_tests ${TEST_SOURCES})
    target_compile_definitions(run_tests PRIVATE UNIT_TEST)
    target_link_libraries(run_tests PRIVATE Catch2::Catch2WithMain)
    add_test(NAME ct9_tests COMMAND run_tests --colour-mode ansi)

    add_custom_target(run_all_tests
        COMMAND ${CMAKE_CTEST_COMMAND}
        DEPENDS run_tests
        COMMENT "Running tests in Develop mode..."
    )

    # Compile options (for ct9)
    target_compile_options(ct9 PRIVATE
        -O0
        -g
        -ggdb3
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -D_GLIBCXX_DEBUG
    )

    # Link options (for ct9)
    target_link_options(ct9 PRIVATE
        -fsanitize=address
        -fprofile-arcs
        -ftest-coverage
    )

    # Compile options (for tests)
    target_compile_options(run_tests PRIVATE
        -O0
        -g
        -ggdb3
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -D_GLIBCXX_DEBUG
    )

    # Link options (for tests)
    target_link_options(run_tests PRIVATE
        -fsanitize=address
        -fprofile-arcs
        -ftest-coverage
    )
endif()

# ------------------------------------------------------------------
# Release configuration:
# Apply high optimization, strip debug info.
# ------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in 'Release' mode with -O3 optimization.")
    target_compile_options(ct9 PRIVATE
        -O3
        -DNDEBUG
    )
endif()

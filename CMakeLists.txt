################################################################################
## Project setup ###############################################################
################################################################################

cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(Calculator-Challenge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Don't use GNU extension (like -std=gnu++11) for portability
set(CMAKE_CXX_EXTENSIONS OFF)

################################################################################
## Fetch External Libraries ####################################################
################################################################################

include(FetchContent)

## Google Test #################################################################
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929 # release-1.11.0
)
FetchContent_MakeAvailable(googletest)

################################################################################
## Compile flags ###############################################################
################################################################################

add_compile_options(
        -pedantic                  # Enforce strict ISO C and ISO C++ compliance
        -pedantic-errors           # Turn all pedantic warnings into errors
        -Wall                      # Enable most common warnings
        -Wextra                    # Enable additional warnings not covered by -Wall
        -fstack-protector-strong   # Enable stack protection to detect stack overflows
        -ftrapv                    # Generate traps for signed overflow on integer operations
        -fwrapv                    # Ensure signed integer overflows wrap around (avoid undefined behaviour)
        -Wcast-align               # Warn about potential performance issues with misaligned casts
        -Wcast-qual                # Warn whenever a cast removes a type qualifier like const
        -Wconversion               # Warn for implicit type conversions that may alter a value
        -Wdouble-promotion         # Warn if a float is implicitly promoted to double
        -Wfloat-equal              # Warn if comparing floating-point values for equality (avoid rounding errors!)
        -Wformat=2                 # Enable high-level format string checking
        -Wformat-security          # Warn about possible security problems with format functions
        -Wmissing-include-dirs     # Warn if an include directory is missing
        -Wnon-virtual-dtor         # Warn if a class with virtual functions has a non-virtual destructor
        -Wold-style-cast           # Warn if using C-style casts
        -Wredundant-decls          # Warn if there are redundant declarations
        -Wshadow                   # Warn if a variable declaration shadows a previous declaration
        -Wsign-conversion          # Warn for implicit type conversions between signed and unsigned types
        -Wswitch-enum              # Warn if a switch statement does not handle all enumeration values
        -Wuninitialized            # Warn if a variable is used without being initialized
        -Wunused-parameter         # Warn if a function parameter is unused
)
## Options supported by GNU ####################################################
if (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    add_compile_options(
            -Walloc-zero              # Warn on zero-sized allocations
            -Walloca                  # Warn on usage of alloca (avoid stack overflow)
            -Wduplicated-branches     # Warn on duplicated branches in if-else statements
            -Wduplicated-cond         # Warn on duplicated conditions in if-else statements
            -Wformat-signedness       # Warn on format specifiers with signedness mismatch
            -Wimplicit-fallthrough=5  # Warn on implicit fallthroughs in switch cases
            -Wlogical-op              # Warn on suspicious use of logical operators
            -Werror                   # Treat all warnings as errors
    )
endif ()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_definitions(DEBUG_BUILD)
endif()

################################################################################
## Build options ###############################################################
################################################################################

option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCUMENTATION "Build documentation" ON)

################################################################################
## Tests #######################################################################
################################################################################

if (BUILD_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif ()

################################################################################
## Generate Documentation ######################################################
################################################################################

if (BUILD_DOCUMENTATION)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/doxygen/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/doxygen/Doxyfile)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Building Doxygen")

        # Build documentation at the same time as the project
        add_custom_target(doxygen ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating Doxygen"
                VERBATIM)
    else (DOXYGEN_FOUND)
        message("Doxygen not installed. Project documentation not generated")
    endif (DOXYGEN_FOUND)
endif ()

################################################################################
## Build #######################################################################
################################################################################

add_subdirectory(src)

################################################################################
## Status ######################################################################
################################################################################

message(STATUS)
message(STATUS "${PROJECT_NAME} Configuration:")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "BUILD_TESTS: ${BUILD_TESTS}")
message(STATUS "BUILD_DOCUMENTATION: ${BUILD_DOCUMENTATION}")
message(STATUS)

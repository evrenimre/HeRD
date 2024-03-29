cmake_minimum_required(VERSION 3.22) 
project(HeRD VERSION 0.0.0 
             DESCRIPTION "Stellar evolution simulation"
             LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Disallow in-source builds
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR “In-source builds are not allowed”)
endif()

# Build output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Build setup
set(CMAKE_CXX_COMPILER g++ CACHE STRING "Compiler")
if(NOT CMAKE_CXX_COMPILER)
		message(STATUS "Empty CMAKE_CXX_COMPILER. Setting to g++")
  	set(CMAKE_CXX_COMPILER g++)
endif(NOT CMAKE_CXX_COMPILER)

set(CMAKE_BUILD_TYPE DEBUG CACHE STRING "Build configuration")
if(NOT CMAKE_BUILD_TYPE)
		message(STATUS "Empty CMAKE_BUILD_TYPE. Setting to Debug")
  	set(CMAKE_BUILD_TYPE DEBUG)
endif(NOT CMAKE_BUILD_TYPE)

# Installation
set(VERSIONED_NAME ${PROJECT_NAME}-${PROJECT_VERSION})
set(INSTALL_INCLUDE_DIR "include/${VERSIONED_NAME}" CACHE PATH "Installation directory for header files")
set(INSTALL_LIB_DIR "lib/${VERSIONED_NAME}" CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR "bin/${VERSIONED_NAME}" CACHE PATH "Installation directory for executables")
set(INSTALL_CMAKE_DIR "${INSTALL_LIB_DIR}/CMake" CACHE PATH "Installation directory for CMake files")

# Options
option(ENABLE_CODE_COVERAGE "Enable coverage reporting" OFF)
option(USE_SANITISER "Enable instrumentation for sanitisers" OFF)

set(UNIT_TEST_LABELS "0_Compile" "1_Continuous" "2_Nightly" "ALL")
list(GET UNIT_TEST_LABELS 0 DEFAULT_UNIT_TEST_LEVEL)
set(UNIT_TEST_LEVEL ${DEFAULT_UNIT_TEST_LEVEL} CACHE STRING "Unit test level")	# Unit tests
set_property(CACHE UNIT_TEST_LEVEL PROPERTY STRINGS ${UNIT_TEST_LABELS} )	# Numbers are introduced for correct lexicographical ordering, from simpler to more complex tests

set(CLANG_TIDY_NAME "clang-tidy" CACHE PATH "Name of the clang-tidy binary")

# Helper functions for creating targets. Used in configuration
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)
include(HerdCreateTarget)	

# Dependencies
find_package(Boost 1.74.0 REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(range-v3 REQUIRED)

# Configuration
set(CONFIG_DIR "${PROJECT_SOURCE_DIR}/Config")

include(HerdConfigureCompilerAndLinker)	# Compiler and linker
include(HerdConfigureStaticCheckers)	# Static checkers
include(HerdConfigureDoxygen)	# Documentation
include(HerdConfigureInstrumentation)	# Instrumentation
include(HerdConfigureUnitTests)	# Unit tests

# Source files
enable_testing()
add_subdirectory(src)

# Unit tests
add_custom_target(UnitTests ALL
                  DEPENDS ${TEST_TARGETS}	# Set in HerdConfigureUnitTests
)

set(CMAKE_CTEST_ARGUMENTS --output-on-failure)
add_custom_command(TARGET UnitTests
                   COMMENT "Run tests" 
                   POST_BUILD COMMAND ${CMAKE_MAKE_PROGRAM} ARGS test
                   WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)



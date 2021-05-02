cmake_minimum_required( VERSION 3.16 ) 
project( HeRD VERSION 0.0.0 
              DESCRIPTION "Stellar evolution simulation"
              LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

set( CMAKE_BUILD_TYPE DEBUG )

# Utilities

set( CONFIG_DIR "${PROJECT_SOURCE_DIR}/Config" )

set(CMAKE_LINK_WHAT_YOU_USE ON)

find_program( CLANG_TIDY clang-tidy )
if( CLANG_TIDY )
	set( CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY}; -checks=*,-modernize-use-trailing-return-type,-readability-named-parameter -line-filter=[{"name":".*\.UnitTests.cpp"}])
endif()

find_program( CPPCHECK cppcheck )
if( CPPCHECK )
	set( CMAKE_CXX_CPPCHECK ${CPPCHECK}; --std=c++20 --force )
endif()

find_program( IWYU iwyu )
if( IWYU )
	set( CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU}; -Xiwyu -transitive_includes_only -mapping_file="${CONFIG_DIR}/Herd.imp" )
endif()

find_program( CCACHE ccache )
if( CCACHE )
	set( CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE} )
endif()

set( CMAKE_LINK_WHAT_YOU_USE TRUE )

# Dependencies
find_package( Boost 1.71.0 REQUIRED )
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(range-v3 REQUIRED)

# Documentation
find_package( Doxygen )
if( DOXYGEN_FOUND )
	set( DOXYGEN_EXTRACT_ALL YES )
	set( DOXYGEN_EXTRACT_PRIVATE YES )
	set( DOXYGEN_EXTRACT_STATIC YES )
	set( DOXYGEN_GENERATE_TREEVIEW YES )
	set( DOXYGEN_CITE_BIB_FILES ${CONFIG_DIR}/Herd.bib )
	set( DOXYGEN_GENERATE_LATEX YES)
	doxygen_add_docs( docs src )	
endif()

# Build output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Installation
set( VERSIONED_NAME ${PROJECT_NAME}-${PROJECT_VERSION} )
set( INSTALL_INCLUDE_DIR "include/${VERSIONED_NAME}" )
set( INSTALL_LIB_DIR "lib/${VERSIONED_NAME}" )
set( INSTALL_BIN_DIR "bin/${VERSIONED_NAME}" )

# Macros used in targets
include( "${PROJECT_SOURCE_DIR}/CMake/AddInstallTargetCommon.cmake" )
include( "${PROJECT_SOURCE_DIR}/CMake/AddLibraryCommon.cmake" )

# Source files
enable_testing()
add_subdirectory( src )

# Unit tests
add_custom_target( UnitTests ALL
                   DEPENDS ${TEST_TARGETS}	# Set at the next level
)
add_custom_command(TARGET UnitTests
                   COMMENT "Run tests"
                   POST_BUILD COMMAND ${CMAKE_CTEST_COMMAND} ARGS --output-on-failure
                   WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)

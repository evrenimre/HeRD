# Configures static checkers

if(NOT DEFINED CONFIG_DIR)
	message(FATAL_ERROR "CONFIG_DIR not defined")
endif()

if(NOT DEFINED CLANG_TIDY_NAME)
	message(FATAL_ERROR "CLANG_TIDY_NAME not defined")
endif()

find_program(CLANG_TIDY ${CLANG_TIDY_NAME})
if(CLANG_TIDY)
	set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY}; -checks=*,-modernize-use-trailing-return-type,-readability-named-parameter # All tests except
																			    -line-filter=[{"name":".*\.UnitTests.cpp"}]	# Exclude unit tests
	)
	message(STATUS "Using ${CLANG_TIDY}")
endif()

find_program(CPPCHECK cppcheck)
if(CPPCHECK)
	set(CPPCHECK_BUILD_DIR ${CMAKE_BINARY_DIR}/cppcheck)
	make_directory(${CPPCHECK_BUILD_DIR})
	set(CMAKE_CXX_CPPCHECK ${CPPCHECK}; --std=c++${CMAKE_CXX_STANDARD} 
																			-i ${CMAKE_BINARY_DIR}	# Exclude the binary dir
																			--inline-suppr .
																			--suppress-xml=${CONFIG_DIR}/cppcheckSuppress.xml . 
																			--cppcheck-build-dir=${CPPCHECK_BUILD_DIR}
	)
	message(STATUS "Using ${CPPCHECK}")
endif()

find_program(IWYU iwyu)
if(IWYU)
	set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU}; -Xiwyu 
																							-transitive_includes_only
																							-mapping_file="${CONFIG_DIR}/Herd.imp" # Headers that can substitute for others
	)
 	message(STATUS "Using ${IWYU}")
endif()
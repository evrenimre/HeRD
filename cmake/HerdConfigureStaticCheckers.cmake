# Configures static checkers

if(NOT DEFINED CONFIG_DIR)
	message(FATAL_ERROR "CONFIG_DIR not defined")
endif()

set(CMAKE_LINK_WHAT_YOU_USE ON)

find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
	set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY}; -checks=*,-modernize-use-trailing-return-type,-readability-named-parameter # All tests except
																			    -line-filter=[{"name":".*\.UnitTests.cpp"}]	# Exclude unit tests
	)
endif()

find_program(CPPCHECK cppcheck)
if(CPPCHECK)
	set(CMAKE_CXX_CPPCHECK ${CPPCHECK}; --std=c++${CMAKE_CXX_STANDARD})
endif()

find_program(IWYU iwyu)
if(IWYU)
	set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU}; -Xiwyu 
																							-transitive_includes_only
																							-mapping_file="${CONFIG_DIR}/Herd.imp" # Headers that can substitute for others
	)
endif()

find_program(CCACHE ccache)
if(CCACHE)
	set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

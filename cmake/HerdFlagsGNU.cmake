# Defines compiler flags for GNU compilers

set(COMPILER_CXX_WARNING_FLAGS "-Wall -Wextra --pedantic -Werror" )	# Enables all warnings for more diagnostics

set(COMPILER_CXX_COVERAGE_FLAGS -O0 -g --coverage)	# Code coverage
set(LINKER_CXX_COVERAGE_FLAGS --coverage)

set(COMPILER_CXX_SANITISER_FLAGS -O0 -g -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined)	# Sanitiser
set(LINKER_CXX_SANITISER_FLAGS -fsanitize=address -fsanitize=undefined)	# Sanitiser

find_program(GOLD gold)
if(GOLD)
	set(LINKER_CXX_LD_FLAGS -fuse-ld=gold)
	message(STATUS "Using ${GOLD}")
endif()
# g++ links libstdc++ and libm by default. No need to explicitly link against them 
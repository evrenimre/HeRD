# Defines compiler flags for GNU compilers

set(COMPILER_CXX_WARNING_FLAGS "-Wall -Wextra --pedantic -Werror" )	# Enables all warnings for more diagnostics

set(COMPILER_CXX_COVERAGE_FLAGS -O0 -g --coverage)	# Code coverage
set(LINKER_CXX_COVERAGE_FLAGS --coverage)

set(COMPILER_CXX_SANITISER_FLAGS -O0 -g -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined)	# Sanitiser
set(LINKER_CXX_SANITISER_FLAGS -fsanitize=address -fsanitize=undefined)	# Sanitiser

#Linker
#Try LLD. It is faster and actively developed
find_program(LLD lld)
if(LLD)
	set(LINKER_CXX_LD_FLAGS -fuse-ld=lld)
	message(STATUS "Using ${LLD}")
endif()
# g++ links libstdc++ and libm by default. No need to explicitly link against them 
# Compiler and linker flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	include(HerdFlagsGNU)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	include(HerdFlagsClang)
else()
	message(WARNING "Could not find a file for compiler and linker flags. Features requiring specific flags may not work correctly")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_CXX_WARNING_FLAGS}")	# Default compiler flags
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${LINKER_CXX_DEFAULT_FLAGS}")	# Default linker flags

find_program(CCACHE ccache)
if(CCACHE)
	set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
	message(STATUS "Using ccache")	
endif()
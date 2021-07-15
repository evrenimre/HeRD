# This works as long as the flags used in the GNU module are identical with their Clang equivalents
include(HerdFlagsGNU)

if(NOT DEFINED CLANG_USE_LIBSTDC++)
		message(FATAL_ERROR "CLANG_USE_LIBSTDC++ not defined")
endif()

if(${CLANG_USE_LIBSTDC++})
	set(LINKER_CXX_DEFAULT_FLAGS "${LINKER_CXX_DEFAULT_FLAGS} -lstdc++ -lm")
endif()

# Use libstdc++ instead of libc++ . 
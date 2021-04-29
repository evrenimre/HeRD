# Config
get_filename_component( TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE )
set( HEADER_LIST Quantities.h 
								 QuantityRange.h
)
set( SOURCE_LIST QuantityRange.cpp )

add_library_common()
add_install_target_common()

# Unit tests
enable_testing()
add_subdirectory( UnitTests )
# Config
get_filename_component( TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE )
set( HEADER_LIST Quantities.h 
								 QuantityRange.h
)
set( SOURCE_LIST QuantityRange.cpp )

add_library_common()
target_link_libraries( ${TARGET_NAME} PUBLIC Boost::boost )

add_install_target_common()

# Unit tests
add_subdirectory( UnitTests )
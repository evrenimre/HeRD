# Config
get_filename_component( TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE )
set( HEADER_LIST Constants.h
								 LuminosityRadiusTemperature.h
)
set( SOURCE_LIST LuminosityRadiusTemperature.cpp )

add_library_common()
target_link_libraries( Physics PRIVATE Generic )

add_install_target_common()

# Unit tests
enable_testing()
add_subdirectory( UnitTests )
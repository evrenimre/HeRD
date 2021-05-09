# Config
get_filename_component( TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE )
set( HEADER_LIST RandomTestFixture.h
								 UnitTestUtilityFunctions.h 
)
set( SOURCE_LIST RandomTestFixture.cpp
	 							 UnitTestUtilityFunctions.cpp
)

add_library_common()
target_link_libraries( SSE PRIVATE boost_unit_test_framework.a
													 PUBLIC Boost::boost
)

add_install_target_common()

# Unit tests
add_subdirectory( UnitTests )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}" )	# This makes it possible to use the parent folders in includes

add_subdirectory( ComputeZAMS )
add_subdirectory( Generic )
add_subdirectory( Physics )
add_subdirectory( SSE )
add_subdirectory( UnitTestUtils )

# Unit test target list
# Unit tests are automatically run after all targets in this list are built
set( TEST_TARGETS TestGeneric
									TestPhysics
									TestSSE PARENT_SCOPE 														
)
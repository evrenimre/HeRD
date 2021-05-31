include_directories("${CMAKE_CURRENT_SOURCE_DIR}")	# This makes it possible to use the parent folders in includes

add_subdirectory(ComputeZAMS)
add_subdirectory(Exceptions) 
add_subdirectory(Generic)
add_subdirectory(Physics)
add_subdirectory(SSE)
add_subdirectory(UnitTestUtils)

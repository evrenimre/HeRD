# Config
get_filename_component( TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE )
set( HEADER_LIST StarState.h
								 ZeroAgeMainSequence.h 
)
set( SOURCE_LIST ZeroAgeMainSequence.cpp )

add_library_common()
target_link_libraries( SSE PRIVATE Generic
																	 Physics
																	 Boost::boost
																	 range-v3::range-v3
													 PUBLIC	 Eigen3::Eigen
)

add_install_target_common()

# Unit tests
add_subdirectory( UnitTests )
# Config
get_filename_component( TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE )

add_executable( ${TARGET_NAME} ComputeZAMS.cpp )

																				  
target_link_libraries( ${TARGET_NAME} PRIVATE Generic
																			 				SSE
																			 				Boost::boost
																			 				boost_program_options
)

add_install_target_common()

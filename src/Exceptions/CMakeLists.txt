# Config
get_filename_component( TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE )
set( HEADER_LIST PreconditionError.h
								 RuntimeError.h
)

add_interface_library_common()
target_link_libraries( ${TARGET_NAME} INTERFACE boost_stacktrace_backtrace
																							  Boost::boost
)

target_compile_definitions( ${TARGET_NAME} INTERFACE BOOST_STACKTRACE_USE_BACKTRACE
																										 BOOST_STACKTRACE_LINK
)
 
add_install_target_common()
# Config
get_filename_component(TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE)
set(HEADER_LIST PreconditionError.h
								RuntimeError.h
)

set( INTERFACE_DEPS_LIST boost_stacktrace_backtrace
												 Boost::boost
 )

herd_add_interface_library(TARGET ${TARGET_NAME} HEADERS ${HEADER_LIST}
																						 		 INTERFACE_DEPS ${INTERFACE_DEPS_LIST}
																						 		 INSTALL
																						 		 COVERAGE
)

target_compile_definitions(${TARGET_NAME} INTERFACE BOOST_STACKTRACE_USE_BACKTRACE
																										BOOST_STACKTRACE_LINK
)

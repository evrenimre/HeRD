# Config
get_filename_component(TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE)
set(HEADER_LIST Exception.h
								ExceptionWrappers.h
								PreconditionError.h
								RuntimeError.h
)

set(SOURCE_LIST ExceptionWrappers.cpp
								PreconditionError.cpp
)

set(PUBLIC_DEPS_LIST Concepts
										 boost_stacktrace_backtrace
										 Boost::boost
)

herd_add_static_library(TARGET ${TARGET_NAME} HEADERS ${HEADER_LIST}
												 											SOURCES ${SOURCE_LIST}
												 											PUBLIC_DEPS ${PUBLIC_DEPS_LIST}
												 											INSTALL
												 											INSTRUMENT
)

target_compile_definitions(${TARGET_NAME} PUBLIC BOOST_STACKTRACE_USE_BACKTRACE
																								 BOOST_STACKTRACE_LINK
)

add_subdirectory(UnitTests)

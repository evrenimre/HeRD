get_filename_component(TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE)
set(HEADER_LIST StarState.h
								ZeroAgeMainSequence.h 
)
set(SOURCE_LIST ZeroAgeMainSequence.cpp)

set(PRIVATE_DEPS_LIST Exceptions
											Generic
											Physics
											Boost::boost
											range-v3::range-v3
)

set(PUBLIC_DEPS_LIST PUBLIC Eigen3::Eigen)

herd_add_static_library(TARGET ${TARGET_NAME} HEADERS ${HEADER_LIST}
												 											SOURCES ${SOURCE_LIST}
												 											PRIVATE_DEPS ${PRIVATE_DEPS_LIST}
												 											PUBLIC_DEPS ${PUBLIC_DEPS_LIST}
												 											INSTALL
)

# Unit tests
add_subdirectory( UnitTests )
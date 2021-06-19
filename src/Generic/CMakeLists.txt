get_filename_component(TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE)
set(HEADER_LIST Quantities.h 
								QuantityRange.h
)
set(SOURCE_LIST QuantityRange.cpp)

set(PUBLIC_DEPS_LIST PUBLIC Exceptions
														Boost::boost
)

herd_add_static_library(TARGET ${TARGET_NAME} HEADERS ${HEADER_LIST}
												 											SOURCES ${SOURCE_LIST}
												 											PRIVATE_DEPS ${PRIVATE_DEPS_LIST}
												 											PUBLIC_DEPS ${PUBLIC_DEPS_LIST}
												 											INSTALL
												 											COVERAGE
)


# Unit tests
add_subdirectory(UnitTests)
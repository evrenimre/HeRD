get_filename_component(TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE)
set(HEADER_LIST MathHelpers.h
								Quantity.h 
								QuantityRange.h
)
set(SOURCE_LIST MathHelpers.cpp
								Quantity.cpp
								QuantityRange.cpp
)

set(PUBLIC_DEPS_LIST PUBLIC Exceptions
														Boost::boost
														Eigen3::Eigen
)

herd_add_static_library(TARGET ${TARGET_NAME} HEADERS ${HEADER_LIST}
												 											SOURCES ${SOURCE_LIST}
												 											PRIVATE_DEPS ${PRIVATE_DEPS_LIST}
												 											PUBLIC_DEPS ${PUBLIC_DEPS_LIST}
												 											INSTALL
												 											INSTRUMENT
)


# Unit tests
add_subdirectory(UnitTests)

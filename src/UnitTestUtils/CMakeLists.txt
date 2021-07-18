get_filename_component(TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE)
set(HEADER_LIST DataLoaderFixture.h
								RandomTestFixture.h
								UnitTestUtilityFunctions.h 
)
set(SOURCE_LIST DataLoaderFixture.cpp
								RandomTestFixture.cpp
	 							UnitTestUtilityFunctions.cpp
)

set(PRIVATE_DEPS_LIST boost_unit_test_framework)

set(PUBLIC_DEPS_LIST PUBLIC Concepts
														Boost::boost
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
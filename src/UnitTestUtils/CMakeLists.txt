get_filename_component(TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE)
set(HEADER_LIST RandomTestFixture.h
								UnitTestUtilityFunctions.h 
)
set(SOURCE_LIST RandomTestFixture.cpp
	 							UnitTestUtilityFunctions.cpp
)

set(PRIVATE_DEPS_LIST boost_unit_test_framework.a)

set(PUBLIC_DEPS_LIST PUBLIC Boost::boost)

herd_add_static_library( TARGET ${TARGET_NAME} HEADERS ${HEADER_LIST}
												 											 SOURCES ${SOURCE_LIST}
												 											 PRIVATE_DEPS ${PRIVATE_DEPS_LIST}
												 											 PUBLIC_DEPS ${PUBLIC_DEPS_LIST}
												 											 INSTALL
												 											 COVERAGE
)

# Unit tests
add_subdirectory(UnitTests)
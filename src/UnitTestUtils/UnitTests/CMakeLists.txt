set( TEST_TARGET_NAME "Test${TARGET_NAME}" )	# TARGET_NAME defined by parent

add_executable( ${TEST_TARGET_NAME} TestUnitTestUtils.cpp
																		RandomTestFixtureTests.cpp
)

target_link_libraries( ${TEST_TARGET_NAME} PRIVATE UnitTestUtils																									 
																									 boost_unit_test_framework.a
																									 Boost::boost
)

add_test( NAME ${TARGET_NAME} COMMAND ${TEST_TARGET_NAME} )
set( TEST_TARGET_NAME "Test${TARGET_NAME}" )	# TARGET_NAME defined by parent

add_executable( ${TEST_TARGET_NAME} TestSSE.cpp
															 			ZeroAgeMainSequenceUnitTests.cpp
)

target_link_libraries( ${TEST_TARGET_NAME} PRIVATE SSE
																									 UnitTestUtils
																									 boost_unit_test_framework.a
																									 Boost::headers
)

add_test( NAME ${TARGET_NAME} COMMAND ${TEST_TARGET_NAME} )

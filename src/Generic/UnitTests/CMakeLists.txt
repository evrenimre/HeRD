set(TEST_TARGET_NAME "Test${TARGET_NAME}")	# TARGET_NAME defined by parent

set(SOURCE_LIST TestGeneric.cpp
								MathHelpersUnitTests.cpp
								QuantityRangeUnitTests.cpp
								QuantityUnitTests.cpp
)

set(PRIVATE_DEPS_LIST Exceptions
											Generic
											UnitTestUtils
											boost_unit_test_framework
											Boost::boost
)

herd_add_executable(TARGET ${TEST_TARGET_NAME} SOURCES ${SOURCE_LIST}
																							 PRIVATE_DEPS ${PRIVATE_DEPS_LIST}
)

add_test(NAME ${TARGET_NAME} COMMAND ${TEST_TARGET_NAME} ${TEST_LABEL_ARG})

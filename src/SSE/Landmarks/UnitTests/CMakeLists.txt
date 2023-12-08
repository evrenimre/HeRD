set(TEST_TARGET_NAME "Test${TARGET_NAME}")	# TARGET_NAME defined by parent

set(SOURCE_LIST TestLandmarks.cpp
								GiantBranchRadiusUnitTests.cpp
								LandmarkUnitTests.cpp
)

set(PRIVATE_DEPS_LIST Generic
											Exceptions
											Landmarks
											UnitTestUtils
											boost_unit_test_framework
)

herd_add_executable(TARGET ${TEST_TARGET_NAME} SOURCES ${SOURCE_LIST}
																							 PRIVATE_DEPS ${PRIVATE_DEPS_LIST}
)

add_test(NAME ${TARGET_NAME} COMMAND ${TEST_TARGET_NAME} ${TEST_LABEL_ARG})

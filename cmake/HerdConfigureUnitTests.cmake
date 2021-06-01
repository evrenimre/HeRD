# Configures unit tests
# Defines TEST_TARGETS, TEST_LABEL_ARG

if(NOT DEFINED UNIT_TEST_LABEL)
	message(FATAL_ERROR "UNIT_TEST_LABEL not defined")
endif()

# Test label string

if(UNIT_TEST_LABEL STREQUAL "ALL" )
	set(TEST_LABEL_ARG --run_test=*)
else()
	set(TEST_LABEL_ARG --run_test=@Compile:@${UNIT_TEST_LABEL})	# Always run compile tests. Otherwise, if the filter does not hit any tests, the test fails
endif()
	
# Unit test target list
# Unit tests are automatically run after all targets in this list are built
set(TEST_TARGETS TestGeneric
								 TestPhysics
								 TestSSE 
								 TestUnitTestUtils 														
)
# Configures unit tests
# Defines TEST_TARGETS, TEST_LABEL_ARG

if(NOT DEFINED UNIT_TEST_LEVEL)
	message(FATAL_ERROR "UNIT_TEST_LEVEL not defined")
endif()

# Test label string


if(UNIT_TEST_LEVEL STREQUAL "ALL")
	set(TEST_LABEL_ARG --run_test=*)	
else()
	# Higher level tests always run those at the lower level as well

	set(TEST_LABEL_ARG --run_test=@Compile)	# Always run compile tests. Otherwise, if the filter does not hit any tests, the test fails
	
	if(UNIT_TEST_LEVEL STRGREATER_EQUAL "1_CI")
		set(TEST_LABEL_ARG ${TEST_LABEL_ARG}:@CI)
	endif()
	
	if(UNIT_TEST_LEVEL STRGREATER_EQUAL "2_Nightly")
		set(TEST_LABEL_ARG ${TEST_LABEL_ARG}:@Nightly)
	endif()
	
endif()
	
# Unit test target list
# Unit tests are automatically run after all targets in this list are built
set(TEST_TARGETS TestGeneric
								 TestPhysics
								 TestSSE 
								 TestUnitTestUtils 														
)
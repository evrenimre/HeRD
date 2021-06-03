# Configures unit tests
# Defines TEST_TARGETS, TEST_LABEL_ARG

if(NOT DEFINED UNIT_TEST_LEVEL)
	message(FATAL_ERROR "UNIT_TEST_LEVEL not defined")
endif()

if(NOT DEFINED UNIT_TEST_LABELS)
	message(FATAL_ERROR "UNIT_TEST_LABELS not defined")
endif()

# Test label string

message( ${UNIT_TEST_LEVEL})
if(UNIT_TEST_LEVEL STREQUAL "ALL")
	set(TEST_LABEL_ARG --run_test=*)	
else()
	# Higher level tests always run those at the lower level as well

	list(GET UNIT_TEST_LABELS 0 LABEL_COMPILE)
	set(TEST_LABEL_ARG --run_test=@${LABEL_COMPILE})	# Always run compile tests. Otherwise, if the filter does not hit any tests, the test fails
	
	list(GET UNIT_TEST_LABELS 1 LABEL_CONTINUOUS)
	if(UNIT_TEST_LEVEL STRGREATER_EQUAL ${LABEL_CONTINUOUS})
		set(TEST_LABEL_ARG ${TEST_LABEL_ARG}:@${LABEL_CONTINUOUS})
	endif()

	list(GET UNIT_TEST_LABELS 2 LABEL_NIGHTLY)	
	if(UNIT_TEST_LEVEL STRGREATER_EQUAL ${LABEL_NIGHTLY})
		set(TEST_LABEL_ARG ${TEST_LABEL_ARG}:@${LABEL_NIGHTLY})
	endif()
	
endif()
	
# Unit test target list
# Unit tests are automatically run after all targets in this list are built
set(TEST_TARGETS TestGeneric
								 TestPhysics
								 TestSSE 
								 TestUnitTestUtils 														
)
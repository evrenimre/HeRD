set(TEST_TARGET_NAME "Test${TARGET_NAME}")	# TARGET_NAME defined by parent

set(SOURCE_LIST TestSSE.cpp
								ConvectiveEnvelopeUnitTests.cpp
								EvolutionStageUnitTests.cpp
								EvolutionStateUnitTests.cpp
								SingleStarEvolutionUnitTests.cpp
								SSETestDataManager.cpp
								SSETestUtils.cpp
								StellarWindMassLossUnitTests.cpp
								TrackPointUnitTests.cpp
							  ZeroAgeMainSequenceUnitTests.cpp
)

set(PRIVATE_DEPS_LIST SSE
											UnitTestUtils
											boost_unit_test_framework
											Boost::boost
											range-v3::range-v3
)

herd_add_executable(TARGET ${TEST_TARGET_NAME} SOURCES ${SOURCE_LIST}
																							 PRIVATE_DEPS ${PRIVATE_DEPS_LIST}
)

set(TEST_DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Data/ZAMS.trackpoints.xml")
set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Data/EvolutionaryTracks")

add_test(NAME ${TARGET_NAME} COMMAND ${TEST_TARGET_NAME} ${TEST_LABEL_ARG} -- --data-path=${TEST_DATA_PATH} --data-dir=${TEST_DATA_DIR})

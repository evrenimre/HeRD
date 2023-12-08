get_filename_component(TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE)
set(HEADER_LIST BaseOfGiantBranch.h
								Constants.h
								GiantBranchRadius.h
								HeliumIgnition.h						
								TerminalMainSequence.h
								Utilities.h
								Utilities.hpp
								ZeroAgeMainSequence.h 
)

set(SOURCE_LIST BaseOfGiantBranch.cpp
								GiantBranchRadius.cpp
								HeliumIgnition.cpp
								TerminalMainSequence.cpp
								ZeroAgeMainSequence.cpp
)

set(PRIVATE_DEPS_LIST Generic
											range-v3::range-v3
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
get_filename_component(TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE)
set(HEADER_LIST BaseOfGiantBranch.h						
								TerminalMainSequence.h
)

set(SOURCE_LIST BaseOfGiantBranch.cpp
								TerminalMainSequence.cpp

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
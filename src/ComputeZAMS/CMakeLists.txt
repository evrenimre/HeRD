get_filename_component(TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE)

set(SOURCE_LIST ComputeZAMS.cpp)

set(PRIVATE_DEPS_LIST Generic
											Landmarks
											Physics
											Boost::boost
											boost_program_options
)

herd_add_executable(TARGET ${TARGET_NAME} SOURCES ${SOURCE_LIST}
																					PRIVATE_DEPS ${PRIVATE_DEPS_LIST}
																					INSTALL
)

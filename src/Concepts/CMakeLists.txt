# Config
get_filename_component(TARGET_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WLE)
set(HEADER_LIST GenericConcepts.h)

herd_add_interface_library(TARGET ${TARGET_NAME} HEADERS ${HEADER_LIST}
																						 		 INSTALL
)

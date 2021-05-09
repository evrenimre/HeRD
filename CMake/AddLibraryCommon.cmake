# Common code for adding a library target
# TARGET_NAME, HEADER_LIST, SOURCE_LIST defined by caller

macro( add_library_common )

	add_library( ${TARGET_NAME} STATIC ${HEADER_LIST}
																		 ${SOURCE_LIST} 
	)
	
	target_include_directories( ${TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
																									  "$<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}>"							                       
	)

	target_link_libraries( ${TARGET_NAME} PUBLIC ${COVERAGE_CONFIG} )	# Instruments the library and its dependents for coverage analysis 
endmacro()
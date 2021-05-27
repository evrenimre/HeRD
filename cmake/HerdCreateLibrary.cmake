# TARGET_NAME, HEADER_LIST, SOURCE_LIST defined by caller

# Common code for adding a library target
macro( add_library_common )

	add_library( ${TARGET_NAME} STATIC ${HEADER_LIST}
																		 ${SOURCE_LIST} 
	)
	
	target_include_directories( ${TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
																									  "$<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}>"							                       
	)

	target_link_libraries( ${TARGET_NAME} PUBLIC ${COVERAGE_CONFIG} )	# Instruments the library and its dependents for coverage analysis 
endmacro()

# Common code for adding an interface library target
macro( add_interface_library_common )

	add_library( ${TARGET_NAME} INTERFACE )
	
	target_include_directories( ${TARGET_NAME} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
																									  	 "$<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}>"							                       
	)

	target_link_libraries( ${TARGET_NAME} INTERFACE ${COVERAGE_CONFIG} )	# Instruments the library and its dependents for coverage analysis 
endmacro()

# Common code for adding an install target
# TARGET_NAME and HEADER_LIST defined by caller

macro( add_install_target_common )
	
	# Installation
	set( EXPORT_TARGET_NAME "${TARGET_NAME}Targets" )
	install( TARGETS ${TARGET_NAME} EXPORT ${EXPORT_TARGET_NAME}
										       LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
										       ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
										       RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
										       INCLUDES DESTINATION "${INSTALL_INCLUDE_DIR}/${TARGET_NAME}"
	)
	
	install( FILES ${HEADER_LIST} DESTINATION "${INSTALL_INCLUDE_DIR}/${TARGET_NAME}")
	
	
	install( EXPORT ${EXPORT_TARGET_NAME}
	         FILE ${EXPORT_TARGET_NAME}.cmake
	         NAMESPACE ${CMAKE_PROJECT_NAME}::
	         DESTINATION "${INSTALL_LIB_DIR}/cmake/${TARGET_NAME}"
	)

endmacro()
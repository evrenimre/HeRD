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
	         NAMESPACE ${TARGET_NAME}::
	         DESTINATION "${INSTALL_LIB_DIR}/cmake/${TARGET_NAME}"
	)

endmacro()
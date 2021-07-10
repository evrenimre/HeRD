# Helper functions to create libraries

# Adds a static library
function(herd_add_static_library)
	set(options INSTALL	# Creates an install target
							INSTRUMENT	# Adds instrumentation code  to library and its dependents
	)
	set(one_value_keywords TARGET	# Target name
	)
	set(multi_value_keywords HEADERS # Headers
													 SOURCES	# Source files
							  					 PRIVATE_DEPS	# Private dependencies
							  					 PUBLIC_DEPS	# Public dependencies
							  					 INTERFACE_DEPS # Interface dependencies
	)
	cmake_parse_arguments(i "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})

	add_library(${i_TARGET} STATIC ${i_HEADERS}
																${i_SOURCES} 
	)
	
	target_include_directories(${i_TARGET} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
																								"$<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}>"							                       
	)

	target_link_libraries(${i_TARGET} PRIVATE ${i_PRIVATE_DEPS} 
																		PUBLIC  ${i_PUBLIC_DEPS}
																		INTERFACE ${i_INTERFACE_DEPS}
	)

	if(i_INSTRUMENT)
		target_link_libraries(${i_TARGET} PUBLIC ${INSTRUMENTATION_TARGET})
	endif()

	if(i_INSTALL)
		herd_internal_add_install_target()
	endif()

endfunction()

# Adds an interface library
function(herd_add_interface_library)
	set(options INSTALL	# If true, creates an install target
							INSTRUMENT	# Adds instrumentation code to library and its dependents
	)
	set(one_value_keywords TARGET	# Target name
	)
	set(multi_value_keywords HEADERS # Headers
								  				 INTERFACE_DEPS # Interface dependencies
	)
	cmake_parse_arguments(i "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})

	add_library(${i_TARGET} INTERFACE)
	
	target_include_directories(${i_TARGET} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
																									 	 "$<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}>"							                       
	)

	target_link_libraries(${i_TARGET} INTERFACE ${i_INTERFACE_DEPS})
	
	if(i_INSTRUMENT)
		target_link_libraries(${i_TARGET} INTERFACE ${INSTRUMENTATION_TARGET})
	endif()
	
	if(i_INSTALL)
		herd_internal_add_install_target()
	endif()
	
endfunction()

# Adds an executable
function(herd_add_executable)
	set(options INSTALL # If true, creates an install target
	)
	set(one_value_keywords TARGET	# Target name
	)
	set(multi_value_keywords SOURCES	# Source files
							  					 PRIVATE_DEPS	# Private dependencies
	)
	cmake_parse_arguments(i "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})
	
	add_executable(${i_TARGET} ${i_SOURCES})
	
	target_link_libraries(${i_TARGET} PRIVATE ${i_PRIVATE_DEPS})

	if(i_INSTALL)
		herd_internal_add_install_target()
	endif()
	
endfunction()

# Macro for creating installation targets. For internal use.
macro(herd_internal_add_install_target)
	set( EXPORT_TARGET_NAME "${i_TARGET}Targets" )
	install( TARGETS ${i_TARGET} EXPORT ${EXPORT_TARGET_NAME}
										      		 LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
											       	 ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
											       	 RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
											       	 INCLUDES DESTINATION "${INSTALL_INCLUDE_DIR}/${i_TARGET}"
	)
		
	install( FILES ${i_HEADERS} DESTINATION "${INSTALL_INCLUDE_DIR}/${i_TARGET}")
		
	install( EXPORT ${EXPORT_TARGET_NAME} FILE ${EXPORT_TARGET_NAME}.cmake
		       															NAMESPACE ${CMAKE_PROJECT_NAME}::
		       															DESTINATION "${INSTALL_CMAKE_DIR}/${i_TARGET}"
	)
endmacro()

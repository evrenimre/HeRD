# Configures instrumentation
# Operation:
# Create a new target for each enabled option
# The instrumentation target depends on each enabled target
# Targets requiring instrumentation introduce a dependency to the instrumentation target


set(INSTRUMENTATION_DEPS)	# Each enabled instrumentation option is added to this list

# Sanitiser

if(NOT DEFINED USE_SANITISER)
	message(FATAL_ERROR "USE_SANITISER not defined")
endif()

if(USE_SANITISER)
	if(COMPILER_CXX_SANITISER_FLAGS)
	
		set(SANITISER_TARGET SanitiserTarget)
		herd_add_interface_library(TARGET ${SANITISER_TARGET} INSTALL)
		
		target_compile_options(${SANITISER_TARGET} INTERFACE ${COMPILER_CXX_SANITISER_FLAGS})
		target_link_options(${SANITISER_TARGET} INTERFACE ${LINKER_CXX_SANITISER_FLAGS})
		
		list(APPEND INSTRUMENTATION_DEPS ${SANITISER_TARGET})
		
			message(STATUS "sanitiser enabled")	
	else()
		 message(FATAL_ERROR "COMPILER_CXX_SANITISER_FLAGS not defined")
	endif()
endif()

# Code coverage

if(NOT DEFINED ENABLE_CODE_COVERAGE)
	message(FATAL_ERROR "ENABLE_CODE_COVERAGE not defined")
endif()

if(ENABLE_CODE_COVERAGE)
	if(COMPILER_CXX_COVERAGE_FLAGS AND LINKER_CXX_COVERAGE_FLAGS)
	
		set(COVERAGE_TARGET CoverageTarget)
		herd_add_interface_library(TARGET ${COVERAGE_TARGET} INSTALL)
	
	  target_compile_options(${COVERAGE_TARGET} INTERFACE ${COMPILER_CXX_COVERAGE_FLAGS})
	  target_link_options(${COVERAGE_TARGET} INTERFACE ${LINKER_CXX_COVERAGE_FLAGS})
	  
	  list(APPEND INSTRUMENTATION_DEPS ${COVERAGE_TARGET})
	  
	  	message(STATUS "Code coverage enabled")	
	else()
		message(FATAL_ERROR "COMPILER_CXX_COVERAGE_FLAGS and/or LINKER_CXX_COVERAGE_FLAGS not defined")  
	endif()
endif()

# Instrumentation target
set(INSTRUMENTATION_TARGET InstrumentationTarget)
herd_add_interface_library(TARGET ${INSTRUMENTATION_TARGET} INSTALL
																														INTERFACE_DEPS ${INSTRUMENTATION_DEPS}
)


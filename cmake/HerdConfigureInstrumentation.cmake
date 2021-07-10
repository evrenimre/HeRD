# Configures instrumentation

if(NOT DEFINED USE_SANITISER)
	message(FATAL_ERROR "USE_SANITISER not defined")
endif()

if(USE_SANITISER)
	if(COMPILER_CXX_SANITISER_FLAGS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_CXX_SANITISER_FLAGS}")
	else()
		 message(FATAL_ERROR "COMPILER_CXX_SANITISER_FLAGS not defined")
	endif()
endif()

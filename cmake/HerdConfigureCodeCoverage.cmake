# Configures code coverage

if(NOT DEFINED ENABLE_CODE_COVERAGE)
	message(FATAL_ERROR "ENABLE_CODE_COVERAGE not defined")
endif()

set(COVERAGE_TARGET CoverageTarget)
herd_add_interface_library(TARGET ${COVERAGE_TARGET} INSTALL)

if(ENABLE_CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options( ${COVERAGE_TARGET} INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  target_link_options(${COVERAGE_TARGET} INTERFACE --coverage)
endif()
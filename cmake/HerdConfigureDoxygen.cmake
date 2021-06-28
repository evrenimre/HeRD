# Configures Doxygen

if(NOT DEFINED CONFIG_DIR)
	message(FATAL_ERROR "CONFIG_DIR not defined")
endif()

find_package( Doxygen )
if( DOXYGEN_FOUND )
	set( DOXYGEN_EXTRACT_ALL YES )
	set( DOXYGEN_EXTRACT_PRIVATE YES )
	set( DOXYGEN_EXTRACT_STATIC YES )
	set( DOXYGEN_GENERATE_TREEVIEW YES )
	set( DOXYGEN_CITE_BIB_FILES ${CONFIG_DIR}/Herd.bib )
	set( DOXYGEN_GENERATE_LATEX NO)
	set( DOXYGEN_USE_MATHJAX YES )
	doxygen_add_docs( docs src )	
endif()
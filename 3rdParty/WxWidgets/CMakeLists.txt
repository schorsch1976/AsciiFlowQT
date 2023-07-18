######################################################################
# WX
######################################################################
if (BUILD_WITH_WX)
	find_package(wxWidgets REQUIRED)

	add_library(wx	INTERFACE IMPORTED GLOBAL)

	target_compile_definitions	(	wx	INTERFACE ${wxWidgets_DEFINITIONS})
	target_include_directories	(	wx	INTERFACE ${wxWidgets_INCLUDE_DIRS})
	target_link_directories		(	wx	INTERFACE ${wxWidgets_LIBRARY_DIRS})
	target_link_libraries(			wx	INTERFACE ${wxWidgets_LIBRARIES})
endif()

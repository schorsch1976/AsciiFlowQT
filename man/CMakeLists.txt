if(UNIX)
	option(BUILD_MAN_PAGE "Build man page" ON)
	if(BUILD_MAN_PAGE)
		add_custom_target(man ALL)

		if (BUILD_WITH_WX)
			add_custom_command(
				TARGET man
				COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/AsciiFlowWx.txt | gzip -9 > ${CMAKE_CURRENT_BINARY_DIR}/AsciiFlowWx.1.gz
				OUTPUTS ${CMAKE_CURRENT_BINARY_DIR}/AsciiFlowWx.1.gz
			)

			add_custom_command(
				TARGET man
				DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/AsciiFlowWx.1.gz
			)
			install(FILES ${CMAKE_CURRENT_BINARY_DIR}/AsciiFlowWx.1.gz DESTINATION ${CMAKE_INSTALL_PREFIX}/share/man/man1)
		endif()

		if (BUILD_WITH_QT)
			add_custom_command(
				TARGET man
				COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/AsciiFlowQT.txt | gzip -9 > ${CMAKE_CURRENT_BINARY_DIR}/AsciiFlowQT.1.gz
				OUTPUTS ${CMAKE_CURRENT_BINARY_DIR}/AsciiFlowQT.1.gz
			)

			add_custom_command(
				TARGET man
				DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/AsciiFlowQT.1.gz
			)
			install(FILES ${CMAKE_CURRENT_BINARY_DIR}/AsciiFlowQT.1.gz DESTINATION ${CMAKE_INSTALL_PREFIX}/share/man/man1)
		endif()
	endif()
endif()


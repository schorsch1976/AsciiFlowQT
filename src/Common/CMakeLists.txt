set(TARGET AsciiFlowCommon)

add_library(${TARGET} SHARED)
AddClangTidy(${TARGET})
AddClangFormat(${TARGET})

# Generate the API Header
include(GenerateExportHeader)
generate_export_header(${TARGET}
		EXPORT_MACRO_NAME	${TARGET}_API
		EXPORT_FILE_NAME	${CMAKE_CURRENT_BINARY_DIR}/${TARGET}/${TARGET}API.h
		)
target_compile_definitions(${TARGET} PRIVATE	${TARGET}_EXPORTS)
target_include_directories(${TARGET} PUBLIC		${CMAKE_CURRENT_BINARY_DIR}/)


target_sources(AsciiFlowCommon
	PRIVATE
		AsciiArtData.cpp
		IsPrint.cpp
		Point.cpp
		Rect.cpp
		ToolArrow.cpp
		ToolClass.cpp
		ToolErase.cpp
		ToolFreehand.cpp
		ToolLine.cpp
		ToolMove.cpp
		ToolRectangle.cpp
		ToolResize.cpp
		ToolText.cpp

	PUBLIC
		include/${TARGET}/AsciiArtData.h
		include/${TARGET}/IsPrint.h
		include/${TARGET}/ITool.h
		include/${TARGET}/KeyEvent.h
		include/${TARGET}/Rect.h
		include/${TARGET}/Point.h
		include/${TARGET}/ToolArrow.h
		include/${TARGET}/ToolClass.h
		include/${TARGET}/ToolErase.h
		include/${TARGET}/ToolFreehand.h
		include/${TARGET}/ToolLine.h
		include/${TARGET}/ToolMove.h
		include/${TARGET}/ToolRectangle.h
		include/${TARGET}/ToolResize.h
		include/${TARGET}/ToolText.h
)

target_include_directories(${TARGET}
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(${TARGET}
	PRIVATE
		AsciiFlowCommon_EXPORTS
)

install(TARGETS ${TARGET}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

install(DIRECTORY include/AsciiFlowCommon
        DESTINATION include
		FILE_PERMISSIONS 		OWNER_WRITE OWNER_READ
								GROUP_READ WORLD_READ
		DIRECTORY_PERMISSIONS	OWNER_WRITE OWNER_READ OWNER_EXECUTE
								GROUP_READ GROUP_EXECUTE
								WORLD_READ WORLD_EXECUTE
)

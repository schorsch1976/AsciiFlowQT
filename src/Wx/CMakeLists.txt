############################################################################
# executable type
############################################################################
if (WIN32)
	set(EXECUTABLE_TYPE WIN32)
else()
	set(EXECUTABLE_TYPE)
endif()

set(TARGET	AsciiFlowWx)

add_executable(${TARGET} ${EXECUTABLE_TYPE})
AddClangTidy(${TARGET})
AddClangFormat(${TARGET})

target_sources(${TARGET}
	PRIVATE
		main.cpp

		App.cpp
		App.h

		MainFrame.cpp
		MainFrame.h

		ImportDialog.cpp
		ImportDialog.h

		ExportDialog.cpp
		ExportDialog.h

		AboutDialog.cpp
		AboutDialog.h

		PreferencesDialog.cpp
		PreferencesDialog.h

		wxAsciiArt.cpp
		wxAsciiArt.h
)

target_compile_definitions(${TARGET}
	PRIVATE
		-DGIT_COMMIT_HASH=${GIT_COMMIT_HASH}
		-DGIT_BRANCH=${GIT_BRANCH}
		-DGIT_DESCRIPTION=${GIT_DESCRIPTION}
)

target_link_libraries(${TARGET}
	PRIVATE
		wx
		AsciiFlowCommon
)

install(TARGETS ${TARGET}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

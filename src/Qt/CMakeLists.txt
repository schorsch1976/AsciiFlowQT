############################################################################
# executable type
############################################################################
if (WIN32)
	set(EXECUTABLE_TYPE WIN32)
else()
	set(EXECUTABLE_TYPE)
endif()

set(TARGET	AsciiFlowQt)

add_executable(${TARGET} ${EXECUTABLE_TYPE})
AddClangTidy(${TARGET})
AddClangFormat(${TARGET})

find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

set_property(TARGET ${TARGET} PROPERTY AUTOMOC ON)
set_property(TARGET ${TARGET} PROPERTY AUTOUIC ON)
set_property(TARGET ${TARGET} PROPERTY AUTORCC ON)

target_sources(${TARGET}
	PRIVATE
		about.h
		about.cpp
		about.ui

		changepreferencesdialog.cpp
		changepreferencesdialog.h
		changepreferencesdialog.ui

		exportdialog.cpp
		exportdialog.h
		exportdialog.ui

		importdialog.cpp
		importdialog.h
		importdialog.ui

		mainwindow.cpp
		mainwindow.h
		mainwindow.ui

		qasciiart.h
		qasciiart.cpp

		resource.qrc

		main.cpp
)

target_include_directories(${TARGET}
		PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(${TARGET}
	PRIVATE
		-DGIT_COMMIT_HASH=${GIT_COMMIT_HASH}
		-DGIT_BRANCH=${GIT_BRANCH}
		-DGIT_DESCRIPTION=${GIT_DESCRIPTION}
)

target_link_libraries(${TARGET}
	PRIVATE
		Qt5::Core
		Qt5::Widgets
		Qt5::Gui
		AsciiFlowCommon
)

install(TARGETS ${TARGET}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)
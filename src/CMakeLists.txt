SET(SRCS
    about.cpp
    about.h
    about.ui

    AsciiArtData.cpp
    AsciiArtData.h

    exportdialog.cpp
    exportdialog.h
    exportdialog.ui

    importdialog.cpp
    importdialog.h
    importdialog.ui

    ITool.cpp
    ITool.h

    main.cpp

    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    qasciiart.cpp
    qasciiart.h

    resource.qrc

    changepreferencesdialog.cpp
    changepreferencesdialog.h
    changepreferencesdialog.ui

    ToolArrow.cpp
    ToolArrow.h

    ToolClass.cpp
    ToolClass.h

    ToolErase.cpp
    ToolErase.h

    ToolFreehand.cpp
    ToolFreehand.h

    ToolLine.cpp
    ToolLine.h

    ToolMove.cpp
    ToolMove.h

    ToolRectangle.cpp
    ToolRectangle.h

    ToolText.cpp
    ToolText.h
)

SET(LIBS
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
)

IF(UNIX)
	IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		SET(LIBS	${LIBS}
			stdc++
		)
	ENDIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
ENDIF(UNIX)

IF(WIN32)
	SET(SUBSYSTEM "WIN32")
ELSE(WIN32)
	SET(SUBSYSTEM)
ENDIF(WIN32)

ADD_EXECUTABLE(AsciiFlowQT         ${SUBSYSTEM} ${SRCS})
TARGET_LINK_LIBRARIES(AsciiFlowQT				${LIBS})

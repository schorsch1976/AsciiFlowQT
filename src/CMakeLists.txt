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

ADD_EXECUTABLE(AsciiFlowQT          ${SRCS})
TARGET_LINK_LIBRARIES(AsciiFlowQT   ${LIBS})
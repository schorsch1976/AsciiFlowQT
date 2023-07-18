add_subdirectory(Common)
if (BUILD_WITH_QT)
	add_subdirectory(Qt)
endif()

if (BUILD_WITH_WX)
	add_subdirectory(Wx)
endif()

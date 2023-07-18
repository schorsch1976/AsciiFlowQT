######################################################################
# OS
######################################################################
function(OS)
	if (UNIX)
		message(STATUS "This is UNIX")
		add_definitions(-DUNIX)

		############################################################################
		# CPP FLAGS
		############################################################################
		set(CMAKE_CXX_FLAGS   "-Wall ${CMAKE_CXX_FLAGS} -Werror=switch")

		############################################################################
		# RPATH
		# https://cmake.org/Wiki/CMake_RPATH_handling
		############################################################################
		# use, i.e. don't skip the full RPATH for the build tree
		set(CMAKE_SKIP_BUILD_RPATH  FALSE)

		# when building, don't use the install RPATH already
		# (but later on when installing)
		set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

		set(CMAKE_INSTALL_RPATH "${TARGET_LIB_PATH}")

		# add the automatically determined parts of the RPATH
		# which point to directories outside the build tree to the install RPATH
		set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


		# the RPATH to be used when installing, but only if it's not a system directory
		list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${TARGET_LIB_PATH}" isSystemDir)
		if("${isSystemDir}" STREQUAL "-1")
			set(CMAKE_INSTALL_RPATH "${TARGET_LIB_PATH}")
		endif("${isSystemDir}" STREQUAL "-1")

		if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
			message(STATUS "This is Linux")
			add_definitions(-DLINUX)

			set(LINUX TRUE)
			set(FREEBSD FALSE)
			set(DARWIN FALSE)

		endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
		if(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
			message(STATUS "This is FreeBSD")
			add_definitions(-DFREEBSD)

			set(LINUX FALSE)
			set(FREEBSD TRUE)
			set(DARWIN FALSE)

		endif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
		if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
			message(STATUS "This is Darwin")
			add_definitions(-DDARWIN)

			set(LINUX FALSE)
			set(FREEBSD FALSE)
			set(DARWIN TRUE)

		endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	endif (UNIX)

	if (WIN32)
		add_definitions(-D_WIN32_WINNT=0x0501)
		if (MSVC)
			message(STATUS "This is Windows: Microsoft Visual Studio")

			add_definitions(-D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING)
			add_definitions(-D_CRT_SECURE_NO_WARNINGS)
			add_definitions(-D_SCL_SECURE_NO_WARNINGS)

			# C4251 is just noise
			# https://stackoverflow.com/questions/24511376/how-to-dllexport-a-class-derived-from-stdruntime-error/24512237#24512237
			# disable C4251
			add_compile_options(/wd4251)
			add_compile_options(/wd4275)
			add_compile_options(/MP)
		endif (MSVC)

		if(MSYS)
			message(STATUS "This is Windows: MSYS")
			set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations)
		endif(MSYS)
	endif (WIN32)
endfunction()
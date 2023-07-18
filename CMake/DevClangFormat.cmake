# adopted from the book: Modern C++ for C++
function(AddClangFormat target)
	if (DEV_CLANG_FORMAT)
		find_program(CLANG-FORMAT_PATH clang-format REQUIRED)

		set(EXPRESSION h hpp hh cc cxx cpp)
		list(TRANSFORM EXPRESSION PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/*.")

		file(GLOB_RECURSE SOURCE_FILES FOLLOW_SYMLINKS
			LIST_DIRECTORIES false ${EXPRESSION})

		add_custom_command(TARGET ${target}
			PRE_BUILD
			COMMAND ${CLANG-FORMAT_PATH} -i --style=file ${SOURCE_FILES})
	endif()
endfunction()

# adopted from the book: Modern C++ for C++
function(AddClangTidy target)
	if (DEV_CLANG_TIDY)
		find_program(CLANG-TIDY_PATH clang-tidy REQUIRED)
		message(STATUS "clang-tidy: ${CLANG-TIDY_PATH}")

		list(APPEND CLANG-TIDY_PATH "-checks=bugprone-*,-bugprone-macro-parentheses,-bugprone-suspicious-include,modernize-*,-modernize-use-trailing-return-type,performance-*,readability-*") # "--fix"

		set_target_properties(${target} PROPERTIES
			CXX_CLANG_TIDY
			"${CLANG-TIDY_PATH}"
		)
	endif()
endfunction()

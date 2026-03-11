include_guard()

function(setup_compiler_options project_name)
	target_compile_definitions(
		${project_name}
		PUBLIC NOMINMAX
		PUBLIC WIN32_LEAN_AND_MEAN
	)

	if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		if(CMAKE_BUILD_TYPE STREQUAL "Release")
			target_compile_options(${project_name} PRIVATE /O2 /Oi /GL /std:c++latest)
		endif()
	else() # GCC / Clang
		# Disable automatic C++ module support
		set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

		target_compile_options(${project_name} PRIVATE -freflection)

		# Ensure no hidden flags are passed
		string(REPLACE "-fmodules-ts" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	endif()
endfunction()
########################################################################
# This cmake file contains a bunch of variables for managing the build #
########################################################################

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_FOLDER_NAME "__BUILD__")
set(BUILD_CACHE_FOLDER_NAME "__BUILD_CACHE__")
set(G_BUILD_DESTINATION "${CMAKE_SOURCE_DIR}/${BUILD_FOLDER_NAME}" CACHE INTERNAL "")
set(G_BUILD_CACHE_DESTINATION "${CMAKE_SOURCE_DIR}/${BUILD_CACHE_FOLDER_NAME}" CACHE INTERNAL "")

########################################################################################################################

# Makes installation more universal and straightforward.
set(
	CMAKE_INSTALL_PREFIX
	${CMAKE_BINARY_DIR}
	CACHE INTERNAL ""
)

########################################################################################################################

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${G_BUILD_CACHE_DESTINATION})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${G_BUILD_CACHE_DESTINATION})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${G_BUILD_CACHE_DESTINATION})

########################################################################################################################

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(BUILD_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_definitions(BUILD_RELEASE)
endif()
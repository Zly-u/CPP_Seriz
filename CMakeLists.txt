cmake_minimum_required(VERSION 4.1)

project(
	CerializationLibDev
	LANGUAGES CXX
)

include(GNUInstallDirs)

include(cmake/BuildConfig.cmake)

# -----------------------------------------------------------------------------

option(DO_TESTS "Google Tests." OFF)

add_subdirectory(SerializationLib)
add_subdirectory(App)
if(DO_TESTS OR DEFINED ENV{CLION_IDE})
#	add_subdirectory(Tests)
endif()

# -----------------------------------------------------------------------------
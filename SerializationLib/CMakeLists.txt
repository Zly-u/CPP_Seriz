project(SerializationLib LANGUAGES CXX)

include(GNUInstallDirs)

string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UpperCase)

file(
	GLOB_RECURSE INCLUDES
	"include/*.hpp" "include/*.h"
)

file(
	GLOB_RECURSE SOURCES
	"src/*.hpp" "src/*.h"
	"src/*.cpp"
)

file(
	GLOB_RECURSE LIBS_SOURCES
	"libs/*.c" "libs/*.cpp" "libs/*.hpp" "libs/*.h"
)

add_library(${PROJECT_NAME} SHARED ${INCLUDES} ${SOURCES} ${LIBS_SOURCES})

include(${CMAKE_SOURCE_DIR}/cmake/Compilers.cmake)
setup_compiler_options(${PROJECT_NAME})

target_include_directories(
	${PROJECT_NAME}
	PUBLIC include
	PUBLIC libs
	PUBLIC src
)

target_compile_definitions(${PROJECT_NAME} PUBLIC SERIZ_EXPORTS)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

install(
	TARGETS ${PROJECT_NAME}
	DESTINATION ${G_BUILD_DESTINATION}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}
)
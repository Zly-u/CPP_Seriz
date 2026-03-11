project(App LANGUAGES CXX)

# -----------------------------------------------------------------------------

file(
	GLOB_RECURSE SOURCES
	"src/*.hpp" "src/*.h"
	"src/*.cpp"
)

file(
	GLOB_RECURSE LIBS_SOURCES
	"libs/*.hpp" "libs/*.h"
	"libs/*.c" "libs/*.cpp"
)

add_executable(${PROJECT_NAME} "main.cpp" ${SOURCES} ${LIBS_SOURCES})

include(${CMAKE_SOURCE_DIR}/cmake/Compilers.cmake)
setup_compiler_options(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE SerializationLib)

target_include_directories(${PROJECT_NAME} PUBLIC src)
target_include_directories(${PROJECT_NAME} PUBLIC libs)

# -----------------------------------------------------------------------------

install(
	TARGETS ${PROJECT_NAME} SerializationLib
	DESTINATION ${G_BUILD_DESTINATION}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}
	ARCHIVE DESTINATION lib
)

#install(
#	FILES "./game_of_life_input.json"
#	DESTINATION ${G_BUILD_DESTINATION}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}
#)

#add_custom_command(
#	TARGET ${PROJECT_NAME}
#	PRE_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy
#	"${CMAKE_CURRENT_SOURCE_DIR}/game_of_life_input.json"
#	"${G_BUILD_CACHE_DESTINATION}"
#	COMMENT "Copying files before build"
#)
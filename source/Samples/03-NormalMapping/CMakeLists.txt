set( FOLDER_NAME 03-NormalMapping )
project( "Sample-${FOLDER_NAME}" )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

file( GLOB SOURCE_FILES
	Src/*.cpp
)

file( GLOB HEADER_FILES
	Src/*.hpp
	Src/*.inl
)

file( GLOB GLSL_SHADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.vert
	${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.frag
)

file( GLOB SHADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.*
)

source_group( "Shader Files" FILES ${GLSL_SHADER_FILES} )
include_directories( ${CMAKE_SOURCE_DIR}/Samples/00-Common/Src )

add_executable( ${PROJECT_NAME} WIN32
	${SOURCE_FILES}
	${HEADER_FILES}
	${GLSL_SHADER_FILES}
)

target_link_libraries( ${PROJECT_NAME}
	${VkLib_LIBRARIES}
	Utils
	Renderer
	Sample-00-Common
	${BinaryLibraries}
	${wxWidgets_LIBRARIES}
	${GTK2_LIBRARIES}
	${ASSIMP_LIBRARIES}
)

add_dependencies( ${PROJECT_NAME}
	Sample-00-Common
)

add_target_precompiled_header( ${PROJECT_NAME}
	Src/PrecompiledHeader.hpp
	Src/PrecompiledHeader.cpp
	${CMAKE_CXX_FLAGS}
	${SOURCE_FILES}
)

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Samples" )

foreach( SHADER ${SHADER_FILES} )
	add_custom_command(
		TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory
			$<$<CONFIG:Debug>:${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/share/${FOLDER_NAME}/Shaders>
			$<$<CONFIG:Release>:${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/share/${FOLDER_NAME}/Shaders>
			$<$<CONFIG:RelWithDebInfo>:${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/share/${FOLDER_NAME}/Shaders>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADER}
			$<$<CONFIG:Debug>:${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/share/${FOLDER_NAME}/Shaders>
			$<$<CONFIG:Release>:${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/share/${FOLDER_NAME}/Shaders>
			$<$<CONFIG:RelWithDebInfo>:${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/share/${FOLDER_NAME}/Shaders>
	)
endforeach()

cmake_minimum_required (VERSION 3.0)
project (hw-1)
set (CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
	message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

add_subdirectory (external)

include_directories(
    external/glfw-3.1.2/include/
    external/glm-0.9.7.1/
    external/glew-1.13.0/include/
    .
)

set(ALL_LIBS
    ${OPENGL_LIBRARY}
    glfw
    GLEW_1130
)

set(UTIL_SOURCE
	util/shader.cpp
	util/shader.h
	util/window_initializer.cpp
	util/window_initializer.h
	util/model.cpp
	util/model.h
	util/buffers.cpp
	util/buffers.h
	util/camera.cpp
	util/camera.h
)

add_definitions(
    -DTW_STATIC
    -DTW_NO_LIB_PRAGMA
    -DTW_NO_DIRECT3D
    -DGLEW_STATIC
    -D_CRT_SECURE_NO_WARNINGS
)

add_executable(hw-1_triangles
		triangles/main.cpp

		${UTIL_SOURCE}
		triangles/VertexShader.glsl
		triangles/FragmentShader1.glsl
		triangles/FragmentShader2.glsl)
target_link_libraries(hw-1_triangles ${ALL_LIBS})


add_executable(hw-1_figure
		figure/main.cpp

		${UTIL_SOURCE}
		figure/VertexShader.glsl
		figure/FragmentShader.glsl)
target_link_libraries(hw-1_figure ${ALL_LIBS})


add_custom_command(
		TARGET hw-1_triangles POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/hw-1_triangles${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/triangles/"
)
add_custom_command(
		TARGET hw-1_figure POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/hw-1_figure${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/figure/"
)
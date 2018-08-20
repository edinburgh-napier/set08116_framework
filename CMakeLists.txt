cmake_minimum_required(VERSION 3.12)
# Compiler flags
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc" CACHE INTERNAL "" FORCE)
endif(MSVC)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(enu_graphics_framework)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)
add_library(enu_graphics_framework STATIC ${SOURCE_FILES})
target_include_directories(enu_graphics_framework PUBLIC src)
	
option(ENU_GFX_TEST "build framework test .exe" OFF)
if(ENU_GFX_TEST)
  add_executable(framework_test "test/test.cpp")
  target_include_directories(framework_test PRIVATE "src/")
  target_link_libraries(framework_test enu_graphics_framework)
endif()
	
#GLFW options
option(GLFW_BUILD_DOCS "" OFF)
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_DOCUMENT_INTERNALS "" OFF)
option(GLFW_INSTALL "" OFF)
#GLEW options
option(BUILD_UTILS "" OFF)
option(BUILD_SHARED_LIBS "" ON)
option(glew-cmake_BUILD_MULTI_CONTEXT "" OFF)
option(glew-cmake_BUILD_SHARED "" ON)
option(glew-cmake_BUILD_SINGLE_CONTEXT "" ON)
option(glew-cmake_BUILD_STATIC "" OFF)
#GLM options
option(GLM_DYNAMIC_LIBRARY_ENABLE "" OFF)
option(GLM_INSTALL_ENABLE "" OFF)
option(GLM_TEST_ENABLE "" OFF)
#ASSIMP
option(ASSIMP_BUILD_ASSIMP_TOOLS "" OFF)
option(ASSIMP_BUILD_TESTS "" OFF)
set(LIBRARY_SUFFIX "" CACHE STRING "the suffix for the assimp windows library")
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_3DS_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_MD2_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_MD3_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_MD5_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_MDL_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
#TODO: go back and enable this, submit bug report
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)

set(BuildTests OFF CACHE BOOL "" FORCE)

#================================================
#### Add External Dependencies ####
#================================================

# GLFW https://github.com/glfw/glfw.git 56ecd62f
add_subdirectory("lib/glfw")
target_include_directories(enu_graphics_framework PUBLIC "lib/glfw/include/")
#================================================
# GLEW https://github.com/Perlmint/glew-cmake.git ea68a21
add_subdirectory("lib/glew")
target_include_directories(enu_graphics_framework PUBLIC "lib/glew/include")
#================================================
# GLM https://github.com/g-truc/glm.git 77332664
add_subdirectory("lib/glm")
target_include_directories(enu_graphics_framework PUBLIC "lib/glm")
#================================================
# assimp https://github.com/assimp/assimp 114c48b
add_subdirectory("lib/assimp")
target_include_directories(enu_graphics_framework PRIVATE "lib/assimp/include")
target_include_directories(enu_graphics_framework PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/lib/assimp/include")
#================================================
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_sources(stb_image INTERFACE 
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_image.h
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_image_write.h
)
#================================================
# OpenGL
FIND_PACKAGE( OpenGL REQUIRED)
if(NOT ${OPENGL_FOUND})
  message(FATAL_ERROR "OPENGL NOT FOUND")
endif()
#====================================================================
target_link_libraries(enu_graphics_framework 
	PUBLIC ${OPENGL_gl_LIBRARY} 
	PUBLIC glfw
	PUBLIC libglew_shared
  PRIVATE stb_image
  PRIVATE assimp
)
#====================================================================
set(junk 
	IrrXML uninstall 
	UpdateAssimpLibsDebugSymbolsAndDLLs zlib zlibstatic glm_static glm_shared
)
set(deps glfw assimp libglew_shared)

FOREACH(jnk ${junk})
	#Hide junks in junk VS project folder
	set_target_properties(${jnk} PROPERTIES FOLDER "JUNK")
ENDFOREACH()

FOREACH(dep ${deps})
	#Hide deps in dep VS project folder
	set_target_properties(${dep} PROPERTIES FOLDER "DEPS")
	#Disable warnings for deps
	target_compile_options(${dep} PUBLIC "/W0")
	#use all core compilation
	target_compile_options(${dep} PUBLIC "/MP")
ENDFOREACH()

if(${ENU_GFX_TEST})
	add_custom_command(TARGET framework_test POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/res"
		"$<TARGET_FILE_DIR:framework_test>"
	)

	FOREACH(dep ${deps})
		add_custom_command(TARGET framework_test POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:${dep}>
			$<TARGET_FILE_DIR:framework_test>
		)
	ENDFOREACH()
  #set_target_properties(framework_test PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(TargetDir)")
endif()


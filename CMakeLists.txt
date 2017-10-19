cmake_minimum_required(VERSION 3.9)
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
option(GLM_TEST_ENABLE_FAST_MATH "" ON)
option(GLM_TEST_ENABLE_LANG_EXTENSIONS "" ON)
option(GLM_TEST_ENABLE_SIMD_AVX2 "" ON)
option(GLM_TEST_ENABLE_SIMD_SSE3 "" ON)
#ASSIMP
option(ASSIMP_BUILD_ASSIMP_TOOLS "" OFF)
option(ASSIMP_BUILD_TESTS "" OFF)
set(LIBRARY_SUFFIX "" CACHE STRING "the suffix for the assimp windows library")
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
#LIBPNG
set(PNG_TESTS OFF CACHE BOOL "" FORCE)
set(PNG_STATIC OFF CACHE BOOL "" FORCE)
set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)

#TODO: go back and enable this, submit bug report
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_MDC_IMPORTER OFF CACHE BOOL "" FORCE)
#set(ASSIMP_BUILD_3DS_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_AC_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASE_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSBIN_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSXML_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_B3D_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_BVH_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_COLLADA_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_DXF_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_CSM_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_HMP_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_IRRMESH_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_IRR_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_LWO_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_LWS_IMPORTER OFF CACHE BOOL "" FORCE)
#set(ASSIMP_BUILD_MD2_IMPORTER OFF CACHE BOOL "" FORCE)
#set(ASSIMP_BUILD_MD3_IMPORTER OFF CACHE BOOL "" FORCE)
#set(ASSIMP_BUILD_MD5_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_MDC_IMPORTER OFF CACHE BOOL "" FORCE)
#set(ASSIMP_BUILD_MDL_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_NFF_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_NDO_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OFF_IMPORTER OFF CACHE BOOL "" FORCE)
#set(ASSIMP_BUILD_OBJ_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OGRE_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OPENGEX_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_PLY_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_MS3D_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_COB_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_BLEND_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_IFC_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_XGL_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_Q3D_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_Q3BSP_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_RAW_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SIB_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SMD_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_STL_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TERRAGEN_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_3D_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_X_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_3MF_IMPORTER OFF CACHE BOOL "" FORCE)
set(BuildTests OFF CACHE BOOL "" FORCE)
set(IL_TESTS OFF CACHE BOOL "" FORCE)
set(IL_BUILD_ILUT OFF CACHE BOOL "" FORCE)
#todo tell IL to calm down and not parse everything

#====================================================================
#### Add External Dependencies ####
# ZLIB https://github.com/madler/zlib cacf7f1
add_subdirectory("lib/zlib")
target_include_directories(enu_graphics_framework PRIVATE "lib/zlib")
target_include_directories(enu_graphics_framework PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/lib/zlib")
#link_directories("${CMAKE_BINARY_DIR}/lib/zlib")
SET(ZLIB_FOUND 1)
#This is for assimp
SET(ZLIB_LIBRARIES zlib)
#This is for libpng
SET(ZLIB_LIBRARY zlib)
SET(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/zlib/")
set(PNG_BUILD_ZLIB ON CACHE BOOL "" FORCE)

#================================================
# GLFW https://github.com/glfw/glfw.git 56ecd62f
add_subdirectory("lib/glfw")
target_include_directories(enu_graphics_framework PUBLIC "lib/glfw/include/")
#================================================
# GLEW https://github.com/Perlmint/glew-cmake.git ea68a21
add_subdirectory("lib/glew")
target_include_directories(enu_graphics_framework PUBLIC "lib/glew/include")
#set_target_properties(libglew_shared PROPERTIES
#  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
#  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
#)
#================================================
# GLM https://github.com/g-truc/glm.git 77332664
add_subdirectory("lib/glm")
target_include_directories(enu_graphics_framework PUBLIC "lib/glm")
#================================================
# assimp https://github.com/assimp/assimp 114c48b
add_subdirectory("lib/assimp")
target_include_directories(enu_graphics_framework PRIVATE "lib/assimp/include")
target_include_directories(enu_graphics_framework PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/lib/assimp/include")
target_include_directories(assimp PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/lib/zlib")
#================================================
# libpng https://github.com/glennrp/libpng.git c1effe1bb
add_subdirectory("lib/libpng")
#target_include_directories(enu_graphics_framework PRIVATE "lib/libpng")
SET(PNG_INCLUDE_DIRS "lib/libpng" "${CMAKE_CURRENT_BINARY_DIR}/lib/libpng" CACHE PATH "")
target_include_directories(enu_graphics_framework PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/lib/libpng")
target_include_directories(png PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/lib/zlib")
target_include_directories(pngfix PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/lib/zlib")
target_include_directories(png-fix-itxt PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/lib/zlib")
SET(PNG_LIBRARIES png CACHE STRING "")
SET(IL_PNG_LIB ON CACHE BOOL "" FORCE)
#================================================
# libjpeg https://github.com/dooglz/libjpeg.git
add_subdirectory("lib/libjpeg")
#(enu_graphics_framework PRIVATE "lib/libjpeg")
SET(JPEG_INCLUDE_DIRS "lib/libjpeg" "${CMAKE_CURRENT_BINARY_DIR}/lib/libjpeg" CACHE PATH "" FORCE)
target_include_directories(enu_graphics_framework PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/lib/libjpeg")
SET(JPEG_LIBRARIES jpeg CACHE STRING "" FORCE)
SET(IL_JPEG_LIB ON CACHE BOOL "" FORCE)
#================================================
# devil https://github.com/dooglz/DevIL.git
add_subdirectory("lib/devil/DevIL")
target_include_directories(enu_graphics_framework PRIVATE "lib/devil/DevIL/include")
target_include_directories(IL PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/lib/libpng")
target_include_directories(IL PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/lib/libjpeg")
#set(IL_RUNTIME_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE PATH "Output location for Devil .dlls/.so" FORCE)
#set(IL_LIBRARY_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} CACHE PATH "Output location for Devil .libs" FORCE)

#================================================
# OpenGL
FIND_PACKAGE( OpenGL REQUIRED)
if(NOT ${OPENGL_FOUND})
  message(FATAL_ERROR "OPENGL NOT FOUND")
endif()

#====================================================================

#====================================================================

#target_link_libraries(IL PRVIATE PNG PRVIATE jpeg)
#target_link_libraries(ILU PRVIATE IL)

target_link_libraries(enu_graphics_framework 
PUBLIC ${OPENGL_gl_LIBRARY} 
PUBLIC glfw
PUBLIC libglew_shared
PRIVATE assimp 
PRIVATE ILU 
PRIVATE zlib
)

#set_target_properties(glfw PROPERTIES FOLDER "DEPS")
#set_target_properties(zlib PROPERTIES FOLDER "DEPS")
#set_target_properties(IL PROPERTIES FOLDER "DEPS")
#set_target_properties(png PROPERTIES FOLDER "DEPS")
#set_target_properties(jpeg PROPERTIES FOLDER "DEPS")
#set_target_properties(ILU PROPERTIES FOLDER "DEPS")
#set_target_properties(pngfix PROPERTIES FOLDER "DEPS")
#set_target_properties(genfiles PROPERTIES FOLDER "DEPS")
#set_target_properties(libglew_shared PROPERTIES FOLDER "DEPS")
#
#set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER "JUNK")
#set_target_properties(uninstall PROPERTIES FOLDER "JUNK")
#set_target_properties(glm_dummy PROPERTIES FOLDER "JUNK")
#set_target_properties(example PROPERTIES FOLDER "JUNK")
#set_target_properties(minigzip PROPERTIES FOLDER "JUNK")
#set_target_properties(zlibstatic PROPERTIES FOLDER "JUNK")


set(deps glfw zlib jpeg png IL ILU pngfix png-fix-itxt assimp libglew_shared)


if(${ENU_GFX_TEST})

add_custom_command(TARGET framework_test POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
"${PROJECT_SOURCE_DIR}/res"
"$<TARGET_FILE_DIR:framework_test>")

FOREACH(dep ${deps})
add_custom_command(TARGET framework_test POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
$<TARGET_FILE:${dep}>
$<TARGET_FILE_DIR:framework_test>)
ENDFOREACH()
endif()

if(${MSVC})
#Disable warnings for deps
  FOREACH(dep ${deps})
  target_compile_options(${dep} PUBLIC "/W0")
  target_compile_options(${dep} PUBLIC "/MP")
  ENDFOREACH()
file( WRITE "${CMAKE_CURRENT_BINARY_DIR}/enu_graphics_framework.vcxproj.user" 
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>
    <Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">
    <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">
        <LocalDebuggerWorkingDirectory>$(TargetDir)</LocalDebuggerWorkingDirectory>
        <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
    </PropertyGroup>
    <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">
        <LocalDebuggerWorkingDirectory>$(TargetDir)</LocalDebuggerWorkingDirectory>
        <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
    </PropertyGroup>
    </Project>")
 endif()
cmake_minimum_required(VERSION 3.9)
# Compiler flags
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc" CACHE INTERNAL "" FORCE)
endif(MSVC)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(enu_graphics_framework)


#### Setup Directories ####
#Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
link_directories("${CMAKE_BINARY_DIR}/lib")
option(ENU_GFX_TEST "MyOption" OFF)

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
set(PNG_BUILD_ZLIB ON CACHE BOOL "" FORCE)
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
# ZLIB https://github.com/madler/zlib
add_subdirectory("lib/zlib")
include_directories("lib/zlib")
include_directories("${CMAKE_BINARY_DIR}/lib/zlib")
link_directories("${CMAKE_BINARY_DIR}/lib/zlib")
SET(ZLIB_FOUND 1)
#This is for assimp
SET(ZLIB_LIBRARIES zlib)
#This is for libpng
SET(ZLIB_LIBRARY zlib)
SET(ZLIB_INCLUDE_DIR"lib/zlib")
#================================================
# GLFW https://github.com/glfw/glfw.git
add_subdirectory("lib/glfw")
include_directories("lib/glfw/include/GLFW")
link_directories("${CMAKE_BINARY_DIR}/lib/glfw")
#================================================
# GLEW https://github.com/Perlmint/glew-cmake.git
add_subdirectory("lib/glew")
include_directories("lib/glew")
link_directories("${CMAKE_BINARY_DIR}/lib/glew")
#================================================
# GLM https://github.com/g-truc/glm.git
add_subdirectory("lib/glm")
include_directories("lib/glm")
link_directories("${CMAKE_BINARY_DIR}/lib/glm")
#================================================
# assimp https://github.com/assimp/assimp
add_subdirectory("lib/assimp")
include_directories("lib/assimp")
link_directories("${CMAKE_BINARY_DIR}/lib/assimp")
#================================================
# libpng https://github.com/glennrp/libpng.git
add_subdirectory("lib/libpng")
include_directories("lib/libpng")
link_directories("${CMAKE_BINARY_DIR}/lib/libpng")
SET(PNG_INCLUDE_DIRS "lib/libpng" "${CMAKE_BINARY_DIR}/lib/libpng" CACHE STRING "")
SET(PNG_LIBRARIES png CACHE STRING "")
SET(IL_PNG_LIB ON CACHE BOOL "" FORCE)
#================================================
# libjpeg https://github.com/dooglz/libjpeg.git
add_subdirectory("lib/libjpeg")
include_directories("lib/libjpeg")
link_directories("${CMAKE_BINARY_DIR}/lib/libjpeg")
SET(JPEG_INCLUDE_DIRS "lib/libjpeg" "${CMAKE_BINARY_DIR}/lib/libjpeg" CACHE STRING "" FORCE)
SET(JPEG_LIBRARIES jpeg CACHE STRING "" FORCE)
SET(IL_JPEG_LIB ON CACHE BOOL "" FORCE)
#================================================
# devil https://github.com/DentonW/DevIL.git
add_subdirectory("lib/devil/DevIL")
include_directories("lib/devil/DevIL")
link_directories("${CMAKE_BINARY_DIR}/lib/devil")
#====================================================================



file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)
if(ENU_GFX_TEST)
  add_executable(enu_graphics_framework ${SOURCE_FILES})
else()
  list(REMOVE_ITEM SOURCE_FILES "src/main.cpp")
  include (GenerateExportHeader)
  add_library(enu_graphics_framework STATIC ${SOURCE_FILES})
  GENERATE_EXPORT_HEADER( enu_graphics_framework
                           BASE_NAME enu_graphics_framework
                           EXPORT_MACRO_NAME enu_graphics_framework_EXPORT
                           EXPORT_FILE_NAME enu_graphics_framework_Export.h
                           STATIC_DEFINE enu_graphics_framework_BUILT_AS_STATIC
  )
endif()

FIND_PACKAGE( OpenGL REQUIRED)
if(${OPENGL_FOUND})
  target_link_libraries(enu_graphics_framework ${OPENGL_gl_LIBRARY}  )
else()
  message("OPENGL NOT FOUND")
endif()

add_custom_command(TARGET enu_graphics_framework POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E copy_directory  
                    "${PROJECT_SOURCE_DIR}/res"
                    $<TARGET_FILE_DIR:enu_graphics_framework>)           

if(${MSVC})
  target_compile_options(enu_graphics_framework PUBLIC /MP)
  target_link_libraries(enu_graphics_framework glfw3dll assimp DevIL ILU zlibd optimized libglew_shared debug libglew_sharedd)
else()
  target_link_libraries(enu_graphics_framework glfw assimp DevIL zlibd optimized glew_shared debug glewd)
endif()

#set_target_properties(libglew_shared PROPERTIES FOLDER "GLEW")
add_dependencies(png zlib)
add_dependencies(assimp zlib)
#add_dependencies(IL png jpeg zlib)
add_dependencies(enu_graphics_framework libglew_shared glfw glm assimp IL ILU)
#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#set(enu_graphics_framework_incs ${dirs} PARENT_SCOPE)

set_target_properties(glfw PROPERTIES FOLDER "DEPS")
set_target_properties(zlib PROPERTIES FOLDER "DEPS")
set_target_properties(IL PROPERTIES FOLDER "DEPS")
set_target_properties(png PROPERTIES FOLDER "DEPS")
set_target_properties(jpeg PROPERTIES FOLDER "DEPS")
set_target_properties(ILU PROPERTIES FOLDER "DEPS")
set_target_properties(pngfix PROPERTIES FOLDER "DEPS")
set_target_properties(genfiles PROPERTIES FOLDER "DEPS")
set_target_properties(png-fix-itxt PROPERTIES FOLDER "DEPS")

set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER "JUNK")
set_target_properties(uninstall PROPERTIES FOLDER "JUNK")
set_target_properties(glm_dummy PROPERTIES FOLDER "JUNK")
set_target_properties(example PROPERTIES FOLDER "JUNK")
set_target_properties(minigzip PROPERTIES FOLDER "JUNK")
set_target_properties(zlibstatic PROPERTIES FOLDER "JUNK")


set(deps glfw zlib jpeg png IL ILU pngfix png-fix-itxt assimp)

if(${MSVC})
#Disable warnings for deps
  FOREACH(dep ${deps})
  target_compile_options(${dep} PUBLIC "/W0")
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
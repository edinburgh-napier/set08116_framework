cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
project(enu_graphics_framework)
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
foreach (config ${CMAKE_CONFIGURATION_TYPES})
  string (TOUPPER ${config} config)
  set_target_properties(${_project} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}"
    ARCHIVE_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}"
    LIBRARY_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}"
    #PDB_OUTPUT_DIRECTORY_${config} ${sym_path}
    VS_INTERMEDIATE_DIRECTORY_${config} "temp"
  )
  SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
  SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
  SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
  SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
  SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
  SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
  SET( VS_INTERMEDIATE_DIRECTORY_${config} "temp/${config}")
  link_directories("${OUTPUT_DIRECTORY}/${config}")
endforeach ()

option(ENU_GFX_TEST "MyOption" OFF)

include(DownloadProject.cmake)
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
#
set(PNG_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(PNG_TESTS OFF CACHE BOOL "" FORCE)
set(PNG_STATIC OFF CACHE BOOL "" FORCE)
#
#set(IL_NO_PNG 0)
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
 
download_project(PROJ zlib
  GIT_REPOSITORY      https://github.com/madler/zlib
  GIT_TAG             2fa463b
  ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)
add_subdirectory(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})
include_directories(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})
SET(ZLIB_FOUND 1)
#This is for assimp
SET(ZLIB_LIBRARIES zlib)
#This is for libpng
SET(ZLIB_LIBRARY zlib)
SET(ZLIB_INCLUDE_DIR ${zlib_SOURCE_DIR})
  
download_project(PROJ glfw
  GIT_REPOSITORY      https://github.com/glfw/glfw.git
  GIT_TAG             0f488ac
  ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)
add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
include_directories("${glfw_SOURCE_DIR}/include")

download_project(PROJ glew
  GIT_REPOSITORY      https://github.com/Perlmint/glew-cmake.git
  GIT_TAG             7be0923
  ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)
add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR})
include_directories("${glew_SOURCE_DIR}/include")

download_project(PROJ glm
  GIT_REPOSITORY      https://github.com/g-truc/glm.git
  GIT_TAG             6a1e2ec
  ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)
add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
include_directories("${glm_SOURCE_DIR}")

download_project(PROJ assimp
  GIT_REPOSITORY      https://github.com/assimp/assimp
  GIT_TAG             a8673d4
  ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)
add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR})
include_directories("${assimp_SOURCE_DIR}/include")

download_project(PROJ libpng
  GIT_REPOSITORY      https://github.com/dooglz/libpng.git
  GIT_TAG             14e885a
  ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)
add_subdirectory(${libpng_SOURCE_DIR} ${libpng_BINARY_DIR})
include_directories("${libpng_SOURCE_DIR}")

SET(PNG_FOUND 1)
SET(PNG_INCLUDE_DIRS ${libpng_SOURCE_DIR} ${libpng_BINARY_DIR})
SET(PNG_LIBRARIES png)
SET(IL_PNG_LIB png)

#download_project(PROJ devil
#  GIT_REPOSITORY      https://github.com/dooglz/DevIL.git
#  GIT_TAG             a2d9193
#  ${UPDATE_DISCONNECTED_IF_AVAILABLE}
#)

download_project(PROJ devil
  GIT_REPOSITORY      https://github.com/DentonW/DevIL
  GIT_TAG             845185a
  ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)
add_subdirectory("${devil_SOURCE_DIR}/DevIL/" ${devil_BINARY_DIR})
include_directories("${devil_SOURCE_DIR}/DevIL/include")


link_directories("${glew_BINARY_DIR}/../lib/")

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

add_custom_command(TARGET enu_graphics_framework POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_directory   # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/res"      # <--this is in-file
        $<TARGET_FILE_DIR:enu_graphics_framework>)                 # <--this is out-file path

        
if(${MSVC})
  target_compile_options(enu_graphics_framework PUBLIC /MP)
  target_link_libraries(enu_graphics_framework glfw3dll assimp DevIL ILU zlibd optimized libglew_shared debug libglew_sharedd)
else()
  target_link_libraries(enu_graphics_framework glfw assimp DevIL zlibd optimized glew_shared debug glewd)
endif()


set_target_properties(libglew_shared PROPERTIES FOLDER "GLEW")
add_dependencies(png zlib)
add_dependencies(assimp zlib)
add_dependencies(IL png zlib)
add_dependencies(enu_graphics_framework libglew_shared glfw glm assimp IL ILU)
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
set(enu_graphics_framework_incs ${dirs} PARENT_SCOPE)

if(${MSVC})
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
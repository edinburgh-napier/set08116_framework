cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
project(enu_graphics_framework)

SET(OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
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

endforeach ()


include(DownloadProject.cmake)

option(GLFW_BUILD_DOCS "" OFF)
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_DOCUMENT_INTERNALS "" OFF)
option(GLFW_INSTALL "" OFF)

download_project(PROJ       glfw
        GIT_REPOSITORY      https://github.com/glfw/glfw.git
        GIT_TAG             master
        ${UPDATE_DISCONNECTED_IF_AVAILABLE}
        )
add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})

#download_project(PROJ       glew
#        GIT_REPOSITORY      https://github.com/nigels-com/glew.git
#        GIT_TAG             master
#        CMAKE_SUBDIR    /build/cmake
#        ${UPDATE_DISCONNECTED_IF_AVAILABLE}
#        )
#ßadd_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR})

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)

add_executable(enu_graphics_framework ${SOURCE_FILES})

FIND_PACKAGE( OpenGL REQUIRED)
if(${OPENGL_FOUND})
    target_link_libraries(enu_graphics_framework ${OPENGL_gl_LIBRARY}  )
else()
    message("OPENGL NOT FOUND")
endif()

target_link_libraries(enu_graphics_framework glfw )
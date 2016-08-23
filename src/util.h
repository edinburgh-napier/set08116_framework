#pragma once

#include "stdafx.h"

namespace graphics_framework {
#if defined(DEBUG) | defined(_DEBUG)

// Debug message callback for OpenGL
// Thanks to Sam Serrels for this one
void __stdcall opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                     const GLchar *message, const void *user_param);

// GLFW error callback
void glfw_debug_callback(int error, const char *message);

// Enables memory leak checking
inline void set_debug() {
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glfwSetErrorCallback(glfw_debug_callback);
  glDebugMessageCallback(opengl_debug_callback, 0);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
}

// Checks for any OpenGL errors
inline bool get_GL_error(int line, const std::string &file) {
  // Get the current error
  GLenum error = glGetError();
  // If there is an error display message
  if (error) {
    // Display error
    std::cerr << "OpenGL Error: " << error << std::endl;
    std::cerr << "At line " << line << " in file " << file << std::endl;
    return true;
  }
  return false;
}
#define CHECK_GL_ERROR get_GL_error(__LINE__, __FILE__)
#define SET_DEBUG set_debug()
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#else
#define CHECK_GL_ERROR false
#define SET_DEBUG
#endif
// Utility function to check if a file exists
inline bool check_file_exists(const std::string &filename) {
  std::ifstream file(filename);
  return file.good();
}

// Utility function to convert screen pos to world ray
void screen_pos_to_world_ray(float mouse_X, float mouse_Y, unsigned int screen_width, unsigned int screen_height,
                             const glm::mat4 &view, const glm::mat4 &proj, glm::vec3 &origin, glm::vec3 &direction);

// Utility function to test intersection between ray and mesh bounding box
bool test_ray_oobb(const glm::vec3 &origin, const glm::vec3 &direction, const glm::vec3 &aabb_min,
                   const glm::vec3 &aabb_max, const glm::mat4 &model, float &distance);
}
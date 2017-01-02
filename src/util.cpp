#include "util.h"
#include "stdafx.h"
#include <IL/il.h>
#include <IL/ilu.h>

namespace graphics_framework {

bool get_devil_error() {
  bool ret = false;
  ILenum error = ilGetError();

  if (error != IL_NO_ERROR) {
    do {
      ret = true;
      std::cerr << "DevIL error: " << iluErrorString(error) << std::endl;
    } while ((error = ilGetError()));
  }
  return ret;
}

inline std::string get_severity(GLenum severity) {
  switch (severity) {
  case GL_DEBUG_SEVERITY_LOW:
    return "LOW SEVERITY";

  case GL_DEBUG_SEVERITY_MEDIUM:
    return "MEDIUM SEVERITY";

  case GL_DEBUG_SEVERITY_HIGH:
    return "HIGH SEVERITY";
  }
  return "UNKNOWN SEVERITY";
}

inline std::string get_source(GLenum source) {
  switch (source) {
  case GL_DEBUG_SOURCE_API:
    return "Source: API";

  case GL_DEBUG_SOURCE_APPLICATION:
    return "Source: APPLICATION";

  case GL_DEBUG_SOURCE_OTHER:
    return "Source: OTHER";

  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    return "Source: SHADER COMPILER";

  case GL_DEBUG_SOURCE_THIRD_PARTY:
    return "Source: THIRD PARTY";

  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    return "Source: WINDOW SYSTEM";
  }
  return "Source: UNKNOWN";
}

// Debug message callback for OpenGL
// Thanks to Sam Serrels for this one
void __stdcall opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                     const GLchar *message, const void *user_param) {
  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    std::cerr << "An OpenGL debug error has been detected: " << message << std::endl;
    std::cerr << get_severity(severity) << std::endl;
    std::cerr << get_source(source) << std::endl;
    break;

  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    std::cerr << "OpenGL deprecated behaviour detected: " << message << std::endl;
    std::cerr << get_severity(severity) << std::endl;
    std::cerr << get_source(source) << std::endl;
    break;

  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    std::cerr << "OpenGL undefined behaviour detected: " << message << std::endl;
    std::cerr << get_severity(severity) << std::endl;
    std::cerr << get_source(source) << std::endl;
    break;

  case GL_DEBUG_TYPE_PORTABILITY:
    std::cerr << "OpenGL portability problem detected: " << message << std::endl;
    std::cerr << get_severity(severity) << std::endl;
    std::cerr << get_source(source) << std::endl;
    break;

  case GL_DEBUG_TYPE_PERFORMANCE:
    std::cerr << "OpenGL performance problem detected: " << message << std::endl;
    std::cerr << get_severity(severity) << std::endl;
    std::cerr << get_source(source) << std::endl;
    break;

#ifdef OGL_DEBUG_OTHER
  case GL_DEBUG_TYPE_OTHER:
    std::cerr << "Other OpenGL error detected: " << message << std::endl;
    std::cerr << get_severity(severity) << std::endl;
    std::cerr << get_source(source) << std::endl;
    break;
#endif
  }
}

// GLFW error callback
void glfw_debug_callback(int error, const char *message) {
  std::cerr << "A GLFW error has occurred: " << message << std::endl;
}

// Utility function to convert screen pos to world ray
void screen_pos_to_world_ray(float mouse_X, float mouse_Y, unsigned int screen_width, unsigned int screen_height,
                             const glm::mat4 &view, const glm::mat4 &proj, glm::vec3 &origin, glm::vec3 &direction) {
  // The ray start and end positions, screen coordinates
  float xx = (2.0f * mouse_X) / screen_width - 1.0f;
  float yy = (2.0f * (screen_height - mouse_Y)) / screen_height - 1.0f;
  glm::vec4 ray_start_screen(xx, yy, -1.0f, 1.0f);
  glm::vec4 ray_end_screen(xx, yy, 0.0f, 1.0f);

  // Calculate combined inverse matrix - goes from screen space to world space
  glm::mat4 inverse_matrix = glm::inverse(proj * view);

  // Calculate ray start and end in the world
  glm::vec4 ray_start_world = (inverse_matrix * ray_start_screen);
  ray_start_world /= ray_start_world.w;
  glm::vec4 ray_end_world = (inverse_matrix * ray_end_screen);
  ray_end_world /= ray_end_world.w;

  // Now calculate ray direction from start and end points
  direction = glm::vec3(ray_end_world - ray_start_world);
  direction = glm::normalize(direction);

  // Origin is just starting position
  origin = glm::vec3(ray_start_world);
}

// Utility function to test intersection between ray and mesh bounding box
bool test_ray_oobb(const glm::vec3 &origin, const glm::vec3 &direction, const glm::vec3 &aabb_min,
                   const glm::vec3 &aabb_max, const glm::mat4 &model, float &distance) {
  float t_min = 0.0f;
  float t_max = 100000.0f;

  // Calculate OOBB position in world space
  glm::vec3 OOBB_pos_world(model[3].x, model[3].y, model[3].z);

  // Calculate direction from ray origin to OOBB position
  glm::vec3 delta = OOBB_pos_world - origin;

  // Test intersection with the 2 planes perpendicular to the OOBB x-axis
  {
    // Get x-axis of transformed object
    glm::vec3 x_axis(model[0].x, model[0].y, model[0].z);
    x_axis = glm::normalize(x_axis);
    // Calculate the cosine of the x_axis and delta
    float e = glm::dot(x_axis, delta);
    // Calculate the cosine of the ray direction and x_axis
    float f = glm::dot(direction, x_axis);

    // Test if ray and x_axis are parallel
    if (fabs(f) > 0.001f) {
      // Calculate intersection distance with the left plane
      float t1 = (e + aabb_min.x) / f;
      // Calculate intersection distance with the right plane
      float t2 = (e + aabb_max.x) / f;

      // t1 needs to be the nearest intersection
      if (t1 > t2)
        std::swap(t1, t2);

      // t_max is the nearest far intersection
      t_max = glm::min(t_max, t2);
      // t_min is the farthese near intersection
      t_min = glm::max(t_min, t1);

      // If far is closer than near there is no intersection on this axis
      if (t_max < t_min)
        return false;
    }
    // ray and x_axis is almost parallel
    else if (-e + aabb_min.x > 0.0f || -e + aabb_max.x < 0.0f)
      return false;
  }

  // Test intersection with the 2 planes perpendicular to the OOBB y-axis
  {
    // Get y-axis of transformed object
    glm::vec3 y_axis(model[1].x, model[1].y, model[1].z);
    y_axis = glm::normalize(y_axis);
    // Calculate the cosine of the y_axis and delta
    float e = glm::dot(y_axis, delta);
    // Calculate the cosine of the ray direction and y_axis
    float f = glm::dot(direction, y_axis);

    // Test if ray and y_axis are parallel
    if (fabs(f) > 0.001f) {
      // Calculate intersection distance with the left plane
      float t1 = (e + aabb_min.y) / f;
      // Calculate intersection distance with the right plane
      float t2 = (e + aabb_max.y) / f;

      // t1 needs to be the nearest intersection
      if (t1 > t2)
        std::swap(t1, t2);

      // t_max is the nearest far intersection
      t_max = glm::min(t_max, t2);
      // t_min is the farthese near intersection
      t_min = glm::max(t_min, t1);

      // If far is closer than near there is no intersection on this axis
      if (t_max < t_min)
        return false;
    }
    // ray and y_axis is almost parallel
    else if (-e + aabb_min.y > 0.0f || -e + aabb_max.y < 0.0f)
      return false;
  }

  // Test intersection with the 2 planes perpendicular to the OOBB z-axis
  {
    // Get x-axis of transformed object
    glm::vec3 z_axis(model[2].x, model[2].y, model[2].z);
    z_axis = glm::normalize(z_axis);
    // Calculate the cosine of the z_axis and delta
    float e = glm::dot(z_axis, delta);
    // Calculate the cosine of the ray direction and z_axis
    float f = glm::dot(direction, z_axis);

    // Test if ray and z_axis are parallel
    if (fabs(f) > 0.001f) {
      // Calculate intersection distance with the left plane
      float t1 = (e + aabb_min.z) / f;
      // Calculate intersection distance with the right plane
      float t2 = (e + aabb_max.z) / f;

      // t1 needs to be the nearest intersection
      if (t1 > t2)
        std::swap(t1, t2);

      // t_max is the nearest far intersection
      t_max = glm::min(t_max, t2);
      // t_min is the farthese near intersection
      t_min = glm::max(t_min, t1);

      // If far is closer than near there is no intersection on this axis
      if (t_max < t_min)
        return false;
    }
    // ray and z_axis is almost parallel
    else if (-e + aabb_min.z > 0.0f || -e + aabb_max.z < 0.0f)
      return false;
  }

  // Set distance and return true
  distance = t_min;
  return true;
}
}
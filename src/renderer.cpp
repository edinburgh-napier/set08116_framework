#include "stdafx.h"

#include "renderer.h"
#include "util.h"
#include <IL/il.h>

namespace graphics_framework {
float renderer::_clear_r;
float renderer::_clear_g;
float renderer::_clear_b;
// Initialise the renderer singleton
renderer *renderer::_instance = nullptr;

// Helper function to display OpenGL information
void print_GL_info() {
  std::clog << "GL Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::clog << "GL Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::clog << "GL Version: " << glGetString(GL_VERSION) << std::endl;
  std::clog << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

// Builds any necessary content for the render framework
void build_content() {}

void renderer::set_screen_dimensions(unsigned int w, unsigned int h) {
  _instance->_width = w;
  _instance->_height = h;
  set_screen_mode(windowed);
}

void renderer::set_screen_mode(ScreenMode sm) {
  auto monitor = glfwGetPrimaryMonitor();
  auto mode = glfwGetVideoMode(monitor);
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  switch (sm) {
  case renderer::windowed:
    glfwWindowHint(GLFW_DECORATED, GL_TRUE);
    glfwSetWindowMonitor(_instance->_window, NULL, mode->width / 2 - (_instance->_width / 2),
                         mode->height / 2 - (_instance->_height / 2), _instance->_width, _instance->_height,
                         mode->refreshRate);
    break;
  case renderer::borderless:
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    //-1's, beacuse if windows/AMD sniffs a window at the exact monitor size it converts to fullscreen
    glfwSetWindowMonitor(_instance->_window, NULL, 0, 0, mode->width - 1, mode->height - 1, mode->refreshRate);
    _instance->_width = mode->width - 1;
    _instance->_height = mode->height - 1;
    break;
  case renderer::fullscreen:
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    glfwSetWindowMonitor(_instance->_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    _instance->_width = mode->width;
    _instance->_height = mode->height;
    break;
  default:
    break;
  }
  glViewport(0, 0, _instance->_width, _instance->_height);
}

double renderer::get_screen_aspect() {
  return static_cast<double>(get_screen_width()) / static_cast<double>(get_screen_height());
}

// Initialises the renderer
bool renderer::initialise(const std::string &title, renderer::ScreenMode sm, unsigned int width, unsigned int height) {
  renderer::_clear_r = 0.0f;
  renderer::_clear_g = 1.0f;
  renderer::_clear_b = 1.0f;
  // Set running to false
  _instance->_running = false;

  glewExperimental = GL_TRUE;
  // Try and initialise GLFW
  if (!glfwInit()) {
    // Display error
    std::cerr << "ERROR - initialisting renderer" << std::endl;
    std::cerr << "Could not initialise GLFW" << std::endl;
    return false;
  }

  // Get the primary monitor
  auto monitor = glfwGetPrimaryMonitor();
  // Get its current video mode
  auto video_mode = glfwGetVideoMode(monitor);

  // Set window hints for GLFW

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // glfwWindowHint(GLFW_ICONIFIED, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, video_mode->refreshRate);

// If in debug mode, set window dimensions to 800 x 600
#if defined(DEBUG) | defined(_DEBUG)
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

  if (sm == windowed) {
    glfwWindowHint(GLFW_DECORATED, GL_TRUE);
    _instance->_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwSetWindowPos(_instance->_window, video_mode->width / 2 - (width / 2), video_mode->height / 2 - (height / 2));
    _instance->_width = width;
    _instance->_height = height;
  } else if (sm == borderless) {
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    //-1's, beacuse if windows/AMD sniffs a window at the exact monitor size it converts to fullscreen
    _instance->_window =
        glfwCreateWindow(video_mode->width - 1, video_mode->height - 1, "Render Framework", nullptr, nullptr);
    _instance->_width = video_mode->width - 1;
    _instance->_height = video_mode->height - 1;
  } else {
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    _instance->_window = glfwCreateWindow(video_mode->width, video_mode->height, "Render Framework", monitor, nullptr);
    _instance->_width = video_mode->width;
    _instance->_height = video_mode->height;
  }

  // Check if window was created
  if (_instance->_window == nullptr) {
    // Display error
    std::cerr << "ERROR - initialising renderer" << std::endl;
    std::cerr << "Could not create window with GLFW" << std::endl;
    // Terminate GLFW
    glfwTerminate();
    return false;
  }

  // Make the window's context current
  glfwMakeContextCurrent(_instance->_window);

  // Set swap interval - make the window refresh with the monitor
  glfwSwapInterval(1);

  // Print OpenGL info
  print_GL_info();

  // Initialise GLEW
  auto status = glewInit();
  if (status != GLEW_OK) {
    // Display error
    std::cerr << "ERROR - initialising renderer" << std::endl;
    std::cerr << "Error initialising GLEW: " << glewGetErrorString(status) << std::endl;
    // Terminate GLFW
    glfwTerminate();
    return false;
  }

  // Init DevIL
  {
    std::clog << "Loading DevIL Image Loader, Version: " << IL_VERSION << std::endl;
    ilInit();
    if (get_devil_error()) {
      return false;
    }
  }

#if defined(DEBUG) | defined(_DEBUG)
  SET_DEBUG;
#endif

  // Set clear colour to cyan
  glClearColor(_clear_r, _clear_g, _clear_b, 1.0f);

  // Check for any errors
  if (CHECK_GL_ERROR) {
    // Display error - not fatal
    std::cerr << "ERROR - initialising renderer" << std::endl;
    std::cerr << "Error enabling clients states textures" << std::endl;
  }

  // Enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Check for any errors
  if (CHECK_GL_ERROR) {
    // Display error - not fatal
    std::cerr << "ERROR - initialising renderer" << std::endl;
    std::cerr << "Error enabling blending" << std::endl;
  }

  // Enable depth testing
  glEnable(GL_DEPTH_TEST);

  // Check for any errors
  if (CHECK_GL_ERROR) {
    // Display error - not fatal
    std::cerr << "ERROR - initialising renderer" << std::endl;
    std::cerr << "Error enabling depth testing" << std::endl;
  }

  // Enable back face culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Check for any errors
  if (CHECK_GL_ERROR) {
    // Display error - not fatal
    std::cerr << "ERROR - initialising renderer" << std::endl;
    std::cerr << "Error enabling back face culling" << std::endl;
  }

  // Enable smoothing
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_MULTISAMPLE);

  // Check for any errors
  if (CHECK_GL_ERROR) {
    // Display error - not fatal
    std::cerr << "ERROR - initialising renderer" << std::endl;
    std::cerr << "Error enabling smoothing / multi-sampling" << std::endl;
  }

  // Enable offsetting - avoids depth conflicts
  glPolygonOffset(1.0f, 1.0f);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glEnable(GL_POLYGON_OFFSET_LINE);
  glEnable(GL_POLYGON_OFFSET_POINT);

  // Check for any errors
  if (CHECK_GL_ERROR) {
    // Display error - not fatal
    std::cerr << "ERROR - initialising renderer" << std::endl;
    std::cerr << "Error enabling polygon offsetting" << std::endl;
  }

  // Enable seamless cube maps
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  // Check for any errors
  if (CHECK_GL_ERROR) {
    // Display error - not fatal
    std::cerr << "ERROR - initialising renderer" << std::endl;
    std::cerr << "Error enabling seamless cube maps" << std::endl;
  }

  // Enable point size manipulation in effects
  glEnable(GL_PROGRAM_POINT_SIZE);

  // Check for any errors
  if (CHECK_GL_ERROR) {
    // Display error - not fatal
    std::cerr << "ERROR - initialising renderer" << std::endl;
    std::cerr << "Error enabling point sizes in shaders" << std::endl;
  }

  // Set running to true
  _instance->_running = true;

  // Log
  std::clog << "LOG - renderer initialised" << std::endl;

  // Create reusable content
  build_content();

  // Log
  std::clog << "LOG - common content built" << std::endl;

  // Return true
  return true;
}

// Begins a render
bool renderer::begin_render() {
  // If not running return false
  if (!_instance->_running) {
    // Display error
    std::cerr << "ERROR - beginning render" << std::endl;
    std::cerr << "Renderer is not running" << std::endl;
    return false;
  }

  // Clear the screen
  clear();

  // Return true
  return true;
}

// Ends a render
void renderer::end_render() {
  // If not running return false
  if (!_instance->_running) {
    // Just return.  Don't log.  This should have been displayed already
    return;
  }

  // Swap the buffers
  swap_buffers();

  // Poll events
  glfwPollEvents();
}

// Clears the screen and associated buffers
void renderer::clear() {
  // Check that we are running
  if (!_instance->_running) {
    // Display error
    std::cerr << "ERROR - clearing screen" << std::endl;
    std::cerr << "Renderer is not running" << std::endl;
    return;
  }

  // Clear the buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

// Swaps the front and back buffers
void renderer::swap_buffers() {
  // Check if running.  If not, return
  if (!_instance->_running)
    return;

  // Swap the buffers via GLFW
  glfwSwapBuffers(_instance->_window);
}

void renderer::toggle_vsync(const bool toggle) { glfwSwapInterval((int)toggle); }

// Shuts down the renderer
void renderer::shutdown() {
  // Log
  std::clog << "LOG - shutdown called on renderer" << std::endl;
  // Set running to false
  _instance->_running = false;
  // Shutdown DevIL
  ilShutDown();
  get_devil_error();
  // Terminated GLFW
  glfwTerminate();
  // Log
  std::clog << "LOG - renderer shutdown" << std::endl;
}

// Binds an effect to the renderer
void renderer::bind(const effect &eff) throw(...) {
  // Check that program is valid
  assert(eff.get_program() != 0);
  // Set effect
  _instance->_effect = eff;
  // Use the program
  glUseProgram(eff.get_program());
  // Check for any errors
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - binding effect to renderer" << std::endl;
    std::cerr << "OpenGL could not use the program" << std::endl;
    // Throw exception
    throw std::runtime_error("Error using effect with OpenGL");
  }
}

// Binds a texture to the renderer at the given index
void renderer::bind(const texture &tex, int index) throw(...) {
  // Check texture is valid
  assert(tex.get_id() != 0);
  // Check that index is valid
  assert(index >= 0);
  // Set active texture
  glActiveTexture(GL_TEXTURE0 + index);
  // Bind texture
  glBindTexture(tex.get_type(), tex.get_id());
  // Check for error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - binding texture to renderer" << std::endl;
    std::cerr << "OpenGL could not bind the texture" << std::endl;
    // Throw exception
    throw std::runtime_error("Error using texture with OpenGL");
  }
}

// Binds a cubemap to the renderer at the given index
void renderer::bind(const cubemap &tex, int index) throw(...) {
  // Check texture is valid
  assert(tex.get_id() != 0);
  // Check that index is valid
  assert(index >= 0);
  // Set active texture
  glActiveTexture(GL_TEXTURE0 + index);
  // Bind texture
  glBindTexture(GL_TEXTURE_CUBE_MAP, tex.get_id());
  // Check for error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - binding cubemap to renderer" << std::endl;
    std::cerr << "OpenGL could not bind the texture" << std::endl;
    // Throw exception
    throw std::runtime_error("Error using cubemap with OpenGL");
  }
}

// Binds a material to the currently bound effect
void renderer::bind(const material &mat, const std::string &name) throw(...) {
  // Check for emissive
  auto idx = _instance->_effect.get_uniform_location(name + ".emissive");
  if (idx != -1)
    glUniform4fv(idx, 1, glm::value_ptr(mat.get_emissive()));
  // Check for diffuse reflection
  idx = _instance->_effect.get_uniform_location(name + ".diffuse_reflection");
  if (idx != -1)
    glUniform4fv(idx, 1, glm::value_ptr(mat.get_diffuse()));
  // Check for specular reflection
  idx = _instance->_effect.get_uniform_location(name + ".specular_reflection");
  if (idx != -1)
    glUniform4fv(idx, 1, glm::value_ptr(mat.get_specular()));
  // Check for shininess
  idx = _instance->_effect.get_uniform_location(name + ".shininess");
  if (idx != -1)
    glUniform1f(idx, mat.get_shininess());
  // Check for error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - binding material to renderer" << std::endl;
    std::cerr << "OpenGL could not set the uniforms" << std::endl;
    // Throw exception
    throw std::runtime_error("Error using material with renderer");
  }
}

// Binds a directional light to the currently bound effect
void renderer::bind(const directional_light &light, const std::string &name) throw(...) {
  // Check for ambient intensity
  auto idx = _instance->_effect.get_uniform_location(name + ".ambient_intensity");
  if (idx != -1)
    glUniform4fv(idx, 1, glm::value_ptr(light.get_ambient_intensity()));
  // Check for light colour
  idx = _instance->_effect.get_uniform_location(name + ".light_colour");
  if (idx != -1)
    glUniform4fv(idx, 1, glm::value_ptr(light.get_light_colour()));
  // Check for light direction
  idx = _instance->_effect.get_uniform_location(name + ".light_dir");
  if (idx != -1)
    glUniform3fv(idx, 1, glm::value_ptr(light.get_direction()));
  // Check for error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - binding directional light to renderer" << std::endl;
    std::cerr << "OpenGL could not set the uniforms" << std::endl;
    // Throw exception
    throw std::runtime_error("Error using directional light with renderer");
  }
}

// Binds a point light to the currently bound effect
void renderer::bind(const point_light &point, const std::string &name) throw(...) {
  // Check for light colour
  auto idx = _instance->_effect.get_uniform_location(name + ".light_colour");
  if (idx != -1)
    glUniform4fv(idx, 1, glm::value_ptr(point.get_light_colour()));
  // Check for position
  idx = _instance->_effect.get_uniform_location(name + ".position");
  if (idx != -1)
    glUniform3fv(idx, 1, glm::value_ptr(point.get_position()));
  // Check for constant
  idx = _instance->_effect.get_uniform_location(name + ".constant");
  if (idx != -1)
    glUniform1f(idx, point.get_constant_attenuation());
  // Check for linear
  idx = _instance->_effect.get_uniform_location(name + ".linear");
  if (idx != -1)
    glUniform1f(idx, point.get_linear_attenuation());
  // Check for quadratic
  idx = _instance->_effect.get_uniform_location(name + ".quadratic");
  if (idx != -1)
    glUniform1f(idx, point.get_quadratic_attenuation());
  // Check for error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - binding point light to renderer" << std::endl;
    std::cerr << "OpenGL could not set the uniforms" << std::endl;
    // Throw exception
    throw std::runtime_error("Error using point light with renderer");
  }
}

// Binds a vector of point lights to the currently bound effect
void renderer::bind(const std::vector<point_light> &points, const std::string &name) throw(...) {
  // Iterate through each light, setting values as required
  unsigned int n = 0;
  for (auto &p : points) {
    std::stringstream stream;
    stream << name << "[" << n << "]";
    auto point_name = stream.str();
    // Check for light colour
    auto idx = _instance->_effect.get_uniform_location(point_name + ".light_colour");
    if (idx != -1)
      glUniform4fv(idx, 1, glm::value_ptr(p.get_light_colour()));
    // Check for position
    idx = _instance->_effect.get_uniform_location(point_name + ".position");
    if (idx != -1)
      glUniform3fv(idx, 1, glm::value_ptr(p.get_position()));
    // Check for constant
    idx = _instance->_effect.get_uniform_location(point_name + ".constant");
    if (idx != -1)
      glUniform1f(idx, p.get_constant_attenuation());
    // Check for linear
    idx = _instance->_effect.get_uniform_location(point_name + ".linear");
    if (idx != -1)
      glUniform1f(idx, p.get_linear_attenuation());
    // Check for quadratic
    idx = _instance->_effect.get_uniform_location(point_name + ".quadratic");
    if (idx != -1)
      glUniform1f(idx, p.get_quadratic_attenuation());
    // Increment light number
    ++n;
  }
  // Check for error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - binding vector of point lights to renderer" << std::endl;
    std::cerr << "OpenGL could not set the uniforms" << std::endl;
    // Throw exception
    throw std::runtime_error("Error using point light with renderer");
  }
}

// Binds a spot light to the currently bound effect
void renderer::bind(const spot_light &spot, const std::string &name) throw(...) {
  // Check for light colour
  auto idx = _instance->_effect.get_uniform_location(name + ".light_colour");
  if (idx != -1)
    glUniform4fv(idx, 1, glm::value_ptr(spot.get_light_colour()));
  // Check for position
  idx = _instance->_effect.get_uniform_location(name + ".position");
  if (idx != -1)
    glUniform3fv(idx, 1, glm::value_ptr(spot.get_position()));
  // Check for direction
  idx = _instance->_effect.get_uniform_location(name + ".direction");
  if (idx != -1)
    glUniform3fv(idx, 1, glm::value_ptr(spot.get_direction()));
  // Check for constant
  idx = _instance->_effect.get_uniform_location(name + ".constant");
  if (idx != -1)
    glUniform1f(idx, spot.get_constant_attenuation());
  // Check for linear
  idx = _instance->_effect.get_uniform_location(name + ".linear");
  if (idx != -1)
    glUniform1f(idx, spot.get_linear_attenuation());
  // Check for quadratic
  idx = _instance->_effect.get_uniform_location(name + ".quadratic");
  if (idx != -1)
    glUniform1f(idx, spot.get_quadratic_attenuation());
  // Check for power
  idx = _instance->_effect.get_uniform_location(name + ".power");
  if (idx != -1)
    glUniform1f(idx, spot.get_power());
  // Check for error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - binding spot light to renderer" << std::endl;
    std::cerr << "OpenGL could not set the uniforms" << std::endl;
    // Throw exception
    throw std::runtime_error("Error using spot light with renderer");
  }
}

// Binds a vector of spot lights to the renderer
void renderer::bind(const std::vector<spot_light> &spots, const std::string &name) throw(...) {
  // Iterate through each light, setting values as required
  unsigned int n = 0;
  for (auto &s : spots) {
    std::stringstream stream;
    stream << name << "[" << n << "]";
    auto spot_name = stream.str();
    // Check for light colour
    auto idx = _instance->_effect.get_uniform_location(spot_name + ".light_colour");
    if (idx != -1)
      glUniform4fv(idx, 1, glm::value_ptr(s.get_light_colour()));
    // Check for position
    idx = _instance->_effect.get_uniform_location(spot_name + ".position");
    if (idx != -1)
      glUniform3fv(idx, 1, glm::value_ptr(s.get_position()));
    // Check for direction
    idx = _instance->_effect.get_uniform_location(spot_name + ".direction");
    if (idx != -1)
      glUniform3fv(idx, 1, glm::value_ptr(s.get_direction()));
    // Check for constant
    idx = _instance->_effect.get_uniform_location(spot_name + ".constant");
    if (idx != -1)
      glUniform1f(idx, s.get_constant_attenuation());
    // Check for linear
    idx = _instance->_effect.get_uniform_location(spot_name + ".linear");
    if (idx != -1)
      glUniform1f(idx, s.get_linear_attenuation());
    // Check for quadratic
    idx = _instance->_effect.get_uniform_location(spot_name + ".quadratic");
    if (idx != -1)
      glUniform1f(idx, s.get_quadratic_attenuation());
    // Check for power
    idx = _instance->_effect.get_uniform_location(spot_name + ".power");
    if (idx != -1)
      glUniform1f(idx, s.get_power());
    // Increment light number
    ++n;
  }
  // Check for error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - binding vector of spot lights to renderer" << std::endl;
    std::cerr << "OpenGL could not set the uniforms" << std::endl;
    // Throw exception
    throw std::runtime_error("Error using spot light with renderer");
  }
}

// Renders a piece of geometry
void renderer::render(const geometry &geom) throw(...) {
  assert(geom.get_array_object() != 0);
  // Check renderer is running
  assert(_instance->_running);
  // Bind the vertex array object for the
  glBindVertexArray(geom.get_array_object());
  // Check for any OpenGL errors
  if (CHECK_GL_ERROR) {
    // Display error
    std::cerr << "ERROR - rendering geometry" << std::endl;
    std::cerr << "Could not bind vertex array object" << std::endl;
    // Throw exception
    throw std::runtime_error("Error rendering geometry");
  }
  // If there is an index buffer then use to render
  if (geom.get_idx_buffer() != 0) {
    // Bind index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom.get_idx_buffer());
    // Check for error
    if (CHECK_GL_ERROR) {
      std::cerr << "ERROR - rendering geometry" << std::endl;
      std::cerr << "Could not bind index buffer" << std::endl;
      // Throw exception
      throw std::runtime_error("Error rendering geometry");
    }
    // Draw elements
    glDrawElements(geom.get_type(), geom.get_index_count(), GL_UNSIGNED_INT, nullptr);
    // Check for error
    if (CHECK_GL_ERROR) {
      // Display error
      std::cerr << "ERROR - rendering geometry" << std::endl;
      std::cerr << "Could not draw elements from indices" << std::endl;
      // Throw exception
      throw std::runtime_error("Error rendering geometry");
    }
  } else {
    // Draw arrays
    glDrawArrays(geom.get_type(), 0, geom.get_vertex_count());
    // Check for error
    if (CHECK_GL_ERROR) {
      std::cerr << "ERROR - rendering geometry" << std::endl;
      std::cerr << "Could not draw arrays" << std::endl;
      // Throw exception
      throw std::runtime_error("Error rendering geometry");
    }
  }
}

// Renders a piece of geometry
void renderer::render(const mesh &m) throw(...) {
  // Render geometry
  render(m.get_geometry());
}

// Sets the render target of the renderer to the screen
void renderer::set_render_target() throw(...) {
  // Set framebuffer to screen (0)
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // Check for error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - setting render target" << std::endl;
    std::cerr << "Could not set render target to screen!" << std::endl;
    // Throw exception
    throw std::runtime_error("Error setting render target");
  }
}

// Sets the render target of the renderer to a shadow map
void renderer::set_render_target(const shadow_map &shadow) throw(...) {
  // Set framebuffer to shadow map's depth buffer
  glBindFramebuffer(GL_FRAMEBUFFER, shadow.buffer->get_buffer());
  // Check for error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - setting render target" << std::endl;
    std::cerr << "Could not set render target to shadow map buffer" << std::endl;
    // Throw exception
    throw std::runtime_error("Error setting render target");
  }
}

// Sets the render target of the renderer to a depth buffer
void renderer::set_render_target(const depth_buffer &depth) throw(...) {
  // Set framebuffer to internal buffer
  glBindFramebuffer(GL_FRAMEBUFFER, depth.get_buffer());
  // Check for error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - setting render target" << std::endl;
    std::cerr << "Could not set render target to depth buffer" << std::endl;
    // Throw exception
    throw std::runtime_error("Error setting render target");
  }
}

// Sets the render target of the renderer to a depth buffer
void renderer::set_render_target(const frame_buffer &frame) throw(...) {
  // Set framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, frame.get_buffer());
  // Check for error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - setting render target" << std::endl;
    std::cerr << "Could not set render target to frame buffer" << std::endl;
    // Throw exception
    throw std::runtime_error("Error setting render target");
  }
}

void renderer::setClearColour(const float r, const float g, const float b) {
  _clear_r = r;
  _clear_g = g;
  _clear_b = b;
  glClearColor(_clear_r, _clear_g, _clear_b, 1.0f);
}
}
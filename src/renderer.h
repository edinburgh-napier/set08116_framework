#pragma once

#include "cubemap.h"
#include "directional_light.h"
#include "effect.h"
#include "frame_buffer.h"
#include "geometry.h"
#include "mesh.h"
#include "point_light.h"
#include "shadow_map.h"
#include "spot_light.h"
#include "stdafx.h"
#include "texture.h"

namespace graphics_framework {
// Forward declaration of app class
class app;

/*
Static class used to control rendering
*/
class renderer {
  // Declare friend class
  friend class app;

private:
  // GLFW window object used by the renderer
  GLFWwindow *_window;
  // Flag determining if the renderer is running
  bool _running;
  // The width of the window used by the renderer
  unsigned int _width;
  // The height of the window used by the renderer
  unsigned int _height;
  // The currently bound effect to the renderer
  effect _effect;
  // The singleton instance of the renderer
  static renderer *_instance;
  // Creates a renderer object.  Should not be called.  Singleton instance
  renderer() {}
  // Copy constructor and assignment operator deleted
  renderer(const renderer &) = delete;
  void operator=(renderer &) = delete;
  float static _clear_r;
  float static _clear_g;
  float static _clear_b;

public:
  enum ScreenMode { windowed, borderless, fullscreen };

  // Destroys the renderer object.  Calls shutdown
  ~renderer() { shutdown(); }
  // Gets the GLFW window
  static GLFWwindow *get_window() { return _instance->_window; }
  // Gets if the renderer is currently running
  static bool is_running() { return _instance->_running; }
  // Sets the running flag on the renderer
  static void set_running(bool value) { _instance->_running = value; }
  // Gets the width of the window used by the renderer
  static unsigned int get_screen_width() { return _instance->_width; }
  // Gets the height of the window used by the renderer
  static unsigned int get_screen_height() { return _instance->_height; }
  // Gets the effect currently bound by the renderer
  static const effect &get_bound_effect() { return _instance->_effect; }
  // Initialises the renderer
  static bool initialise(const std::string &title, renderer::ScreenMode sm = renderer::windowed,
                         unsigned int width = 1280, unsigned int height = 720);
  static void set_screen_dimensions(unsigned int w, unsigned int h);
  static void set_screen_mode(ScreenMode sm);
  // Begins a render
  static bool begin_render();
  // Ends a render
  static void end_render();
  // Clears the current render buffer
  static void clear();
  // Swaps the screen buffers
  static void swap_buffers();
  // enalbe or disable vertical sync
  static void toggle_vsync(const bool toggle);
  // Shuts down the renderer
  static void shutdown();
  // Binds an effect with the renderer
  static void bind(const effect &eff) throw(...);
  // Binds a texture with the renderer
  static void bind(const texture &tex, int index) throw(...);
  // Binds a cubemap with the renderer
  static void bind(const cubemap &tex, int index) throw(...);
  // Binds a material with the renderer
  static void bind(const material &mat, const std::string &name) throw(...);
  // Binds a directional light with the renderer
  static void bind(const directional_light &light, const std::string &name) throw(...);
  // Binds a point light with the renderer
  static void bind(const point_light &point, const std::string &name) throw(...);
  // Binds a vector of point lights to the renderer
  static void bind(const std::vector<point_light> &points, const std::string &name) throw(...);
  // Binds a spot light with the renderer
  static void bind(const spot_light &spot, const std::string &name) throw(...);
  // Binds a vector of spot lights to the renderer
  static void bind(const std::vector<spot_light> &spots, const std::string &name) throw(...);
  // Renders a piece of geometry
  static void render(const geometry &geom) throw(...);
  // Renders a mesh object
  static void render(const mesh &m) throw(...);
  // Sets the render target of the renderer to the screen
  static void set_render_target() throw(...);
  // Sets the render target of the renderer to a shadow map
  static void set_render_target(const shadow_map &shadow) throw(...);
  // Sets the render target of the renderer to a depth buffer
  static void set_render_target(const depth_buffer &depth) throw(...);
  // Sets the render target of the renderer to a frame buffer
  static void set_render_target(const frame_buffer &frame) throw(...);

  static void setClearColour(const float r, const float g, const float b);
};
}
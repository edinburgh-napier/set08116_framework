#pragma once

#include "renderer.h"
#include "stdafx.h"

namespace graphics_framework {
/*
The application class.  Used to run the render framework
*/
class app {
private:
  // The initialisation function
  std::function<bool()> _init_func;
  // The load content function
  std::function<bool()> _load_content_func;
  // The update function
  std::function<bool(float)> _update_func;
  // The render function
  std::function<bool()> _render_func;
  // The shutdown function
  std::function<void()> _shutdown_func;

public:
  // Creates rendering application.  Initialises the renderer
  app(const std::string& title = "Render Framework", renderer::ScreenMode sm = renderer::windowed, unsigned int width = 1980, unsigned int height = 1024);
  // Deleted copy, move constructor and assignment operator
  app(const app &other) = delete;
  app(app &&other) = delete;
  app &operator=(const app &rhs) = delete;
  // Destroys the rendering application
  ~app() {
    // Delete the renderer
    delete renderer::_instance;
  }
  // Sets the initialisation function
  void set_initialise(const std::function<bool()> &f) { _init_func = f; }
  // Sets the load content function
  void set_load_content(const std::function<bool()> &f) { _load_content_func = f; }
  // Sets the update function
  void set_update(const std::function<bool(float)> &f) { _update_func = f; }
  // Sets the render function
  void set_render(const std::function<bool()> &f) { _render_func = f; }
  // Sets the shutdown function
  void set_shutdown(const std::function<void()> &f) { _shutdown_func = f; }
  // Sets the keyboard callback function.  This is handled by GLFW
  void set_keyboard_callback(GLFWkeyfun f) const { glfwSetKeyCallback(renderer::get_window(), f); }
  // Sets the mouse button callback function.  This is handled by GLFW
  void set_mousebutton_callback(GLFWmousebuttonfun f) const { glfwSetMouseButtonCallback(renderer::get_window(), f); }
  // Sets the mouse position callback function.  This is handled by GLFW
  void set_mouseposition_callback(GLFWcursorposfun f) const { glfwSetCursorPosCallback(renderer::get_window(), f); }
  // Sets the mouse wheel scroll callback function.  This is handled by GLFW
  void set_scroll_callback(GLFWscrollfun f) const { glfwSetScrollCallback(renderer::get_window(), f); }
  // Runs the application
  void run();
};
}
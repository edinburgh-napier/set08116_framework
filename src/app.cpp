#include "stdafx.h"

#include "app.h"

namespace graphics_framework {
// Runs the main application
void app::run() {
  // Check if renderer intialised
  if (!renderer::is_running()) {
    std::cerr << "ERROR - renderer did not initialise" << std::endl;
    return;
  }
  // Initialise the application if required
  if (_init_func && !_init_func()) {
    std::cerr << "ERROR - could not initialise application" << std::endl;
    return;
  }
  // Load any content if required
  if (_load_content_func && !_load_content_func()) {
    // Don't exit - not considered fatal
    std::cerr << "ERROR - loading content" << std::endl;
  }

  // Check there is a render - if not no point running
  if (!_render_func) {
    std::cerr << "ERROR - no render function defined" << std::endl;
    return;
  }

  // Monitor the elapsed time per frame
  auto current_time_stamp = std::chrono::system_clock::now();
  auto prev_time_stamp = std::chrono::system_clock::now();

  // Main render loop
  while (renderer::is_running()) {
    // Get current time
    current_time_stamp = std::chrono::system_clock::now();
    // Calculate elapsed time
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time_stamp - prev_time_stamp).count();
    auto seconds = static_cast<float>(elapsed) / 1000.0f;

    // Check if escape is pressed or window should be closing
    if (glfwGetKey(renderer::get_window(), GLFW_KEY_ESCAPE) || glfwWindowShouldClose(renderer::get_window())) {
      // Display message
      std::clog << "LOG - escape pressed or window closed.  Exiting" << std::endl;
      break;
    }

    // Update the application if required
    if (_update_func) {
      // Call update
      if (!_update_func(seconds)) {
        // Log update exit
        std::clog << "LOG - update returned false.  Exiting" << std::endl;
        break;
      }
    }

    // Begin rendering
    if (!renderer::begin_render()) {
      // Display error and exit
      std::cerr << "ERROR - could not begin render" << std::endl;
      break;
    }
    // Call render function
    if (!_render_func()) {
      // Display error only
      std::cerr << "ERROR - problem during render" << std::endl;
    }
    // End render
    renderer::end_render();
    // Update previous time stamp
    prev_time_stamp = current_time_stamp;
  }
  // Call shutdown function
  if (_shutdown_func) {
    _shutdown_func();
  }

  // Application should now be exiting
}
}
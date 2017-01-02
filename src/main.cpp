#include "graphics_framework.h"
#include <glm/glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

geometry geom;
geometry geom2;
geometry geom3;
effect eff;
effect teff;
texture tpng;
texture tjpg;
target_camera cam;
float theta = 0.0f;

bool load_content() {
  // Create triangle data
  // Positions
  vector<vec3> positions{vec3(0.0f, 1.0f, 0.0f), vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f)};
  // Colours
  vector<vec4> colours{vec4(1.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)};
  // Add to the geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);

  geom2 = geometry_builder::create_plane(10, 10);

  // Load in model
  auto src = "../../assimp-src/test/models/OBJ/box.obj";
  geom3 = geometry(src);

  src = "../../assimp-src/test/models/LWO/LWO2/uvtest.png";
  tpng = texture(src, false, false);
  src = "../../assimp-src/test/models/OBJ/engineflare1.jpg";
  tjpg = texture(src, false, false);

  // Load in shaders
  eff.add_shader("shaders/basic.vert", GL_VERTEX_SHADER);
  eff.add_shader("shaders/basic.frag", GL_FRAGMENT_SHADER);
  eff.build();
  teff.add_shader("shaders/basic_textured.vert", GL_VERTEX_SHADER);
  teff.add_shader("shaders/basic_textured.frag", GL_FRAGMENT_SHADER);
  teff.build();
  // Set camera properties
  cam.set_position(vec3(10.0f, 10.0f, 10.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  // Update the angle - half rotation per second
  theta += pi<float>() * delta_time;
  // Update the camera
  cam.update(delta_time);

  if (glfwGetKey(renderer::get_window(), GLFW_KEY_F)) {
    renderer::set_screen_dimensions(1280, 720);
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
  }

  if (glfwGetKey(renderer::get_window(), GLFW_KEY_G)) {
    renderer::set_screen_dimensions(800, 600);
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
  }

  if (glfwGetKey(renderer::get_window(), GLFW_KEY_H)) {
    renderer::set_screen_mode(renderer::borderless);
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_J)) {
    renderer::set_screen_mode(renderer::fullscreen);
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
  }

  return true;
}

bool render() {
  renderer::bind(eff);
  mat4 R;
  R = rotate(mat4(1.0f), theta, vec3(0.0f, 0.0f, 1.0f));
  mat4 M = R;
  auto V = cam.get_view();
  auto P = cam.get_projection();
  auto MVP = P * V * M;
  // Set MVP matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), // Location of uniform
                     1,                               // Number of values - 1 mat4
                     GL_FALSE,                        // Transpose the matrix?
                     value_ptr(MVP));                 // Pointer to matrix data
                                                      // Render geometry
  renderer::render(geom);
  renderer::render(geom3);
  //

  renderer::bind(teff);
  renderer::bind(tjpg, 0);
  renderer::bind(tpng, 1);
  glUniform1i(teff.get_uniform_location("tex"), 0);
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE,
                     value_ptr(P * V * translate(mat4(), vec3(3.0f, -5.0f, -8.0f))));
  renderer::render(geom2);
  glUniform1i(teff.get_uniform_location("tex"), 1);
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE,
                     value_ptr(P * V * translate(mat4(), vec3(-8.0f, -5.0f, -8.0f))));
  renderer::render(geom2);
  return true;
}

int main() {
  // Create application
  app application(renderer::windowed);
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
  return 0;
}
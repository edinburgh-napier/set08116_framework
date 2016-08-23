#pragma once

#include "stdafx.h"

namespace graphics_framework {
/*
Object that describes a directional light
*/
class directional_light {
private:
  // Ambient intensity of the light
  glm::vec4 _ambient;
  // The colour of the light
  glm::vec4 _colour;
  // The direction the light is facing
  glm::vec3 _direction;

public:
  // Creates a directional light with some default colour values
  directional_light()
      : _ambient(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)), _colour(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)),
        _direction(glm::vec3(1.0f, 0.0f, 0.0f)) {}
  // Creates a directional light with provided values
  directional_light(const glm::vec4 &ambient_intensity, const glm::vec4 &light_colour, const glm::vec3 &direction)
      : _ambient(ambient_intensity), _colour(light_colour), _direction(direction) {}
  // Default copy constructor and assignment operator
  directional_light(const directional_light &other) = default;
  directional_light &operator=(const directional_light &rhs) = default;
  // Gets the ambient intensity of the light
  glm::vec4 get_ambient_intensity() const { return _ambient; }
  // Sets the ambient intensity of the light
  void set_ambient_intensity(const glm::vec4 &value) { _ambient = value; }
  // Gets the colour of the light
  glm::vec4 get_light_colour() const { return _colour; }
  // Sets the colour of the light
  void set_light_colour(const glm::vec4 &value) { _colour = value; }
  // Gets the direction of the light
  glm::vec3 get_direction() const { return _direction; }
  // Sets the direction of the light
  void set_direction(const glm::vec3 &value) { _direction = value; }
  // Rotates the light using the given Euler angles
  void rotate(const glm::vec3 &rotation);
  // Rotates the light using the given quaternion
  void rotate(const glm::quat &rotation);
};
}
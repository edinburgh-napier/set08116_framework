#pragma once

#include "stdafx.h"

namespace graphics_framework {
/*
An object representing point light data
*/
class point_light {
private:
  // The colour of the point light
  glm::vec4 _colour;
  // The position of the point light
  glm::vec3 _position;
  // The constant factor of the attenuation
  float _constant;
  // The linear factor of the attenuation
  float _linear;
  // The quadratic factor of the attenuation
  float _quadratic;

public:
  // Creates a point light with a default colour
  point_light()
      : _colour(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)), _position(glm::vec3(0.0f, 0.0f, 0.0f)), _constant(0.5f),
        _linear(0.2f), _quadratic(0.01f) {}
  // Creates a point light with provided properties
  point_light(const glm::vec4 &colour, const glm::vec3 &pos, float constant, float linear, float quadratic)
      : _colour(colour), _position(pos), _constant(constant), _linear(linear), _quadratic(quadratic) {}
  // Default copy constructor and assignment operator
  point_light(const point_light &other) = default;
  point_light &operator=(const point_light &rhs) = default;
  // Gets the light colour of the point light
  glm::vec4 get_light_colour() const { return _colour; }
  // Sets the light colour of the point light
  void set_light_colour(const glm::vec4 &value) { _colour = value; }
  // Gets the position of the point light
  glm::vec3 get_position() const { return _position; }
  // Sets the position of the point light
  void set_position(const glm::vec3 &value) { _position = value; }
  // Gets the constant factor of the light attenuation
  float get_constant_attenuation() const { return _constant; }
  // Sets the constant factor of the light attenuation
  void set_constant_attenuation(float value) { _constant = value; }
  // Gets the linear factor of the light attenuation
  float get_linear_attenuation() const { return _linear; }
  // Sets the linear factor of the light attenuation
  void set_linear_attenuation(float value) { _linear = value; }
  // Gets the quadratic factor of the light attenuation
  float get_quadratic_attenuation() const { return _quadratic; }
  // Sets the quadratic factor of the light attenuation
  void set_quadratic_attenuation(float value) { _quadratic = value; }
  // Sets the range of the point light
  void set_range(float range) {
    _linear = 2.0f / range;
    _quadratic = 1.0f / (powf(range, 2.0f));
  }
  // Moves the light by the given vector
  void move(const glm::vec3 &translation) { _position += translation; }
};
}
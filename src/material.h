#pragma once

#include "stdafx.h"

namespace graphics_framework {
/*
An object representing material data
*/
class material {
private:
  // The emissive colour of the material
  glm::vec4 _emissive;
  // The diffuse reflection colour of the material
  glm::vec4 _diffuse_reflection;
  // The specular reflection colour of the material
  glm::vec4 _specular_reflection;
  // The shininess of the material
  float _shininess;

public:
  // Creates a material object providing a default colour
  material()
      : _emissive(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), _diffuse_reflection(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)),
        _specular_reflection(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), _shininess(10.0f) {}
  // Creates a material object by providing the colour properties
  material(const glm::vec4 &emissive, const glm::vec4 &diffuse, const glm::vec4 &specular, float shininess)
      : _emissive(emissive), _diffuse_reflection(diffuse), _specular_reflection(specular), _shininess(shininess) {}
  // Default copy constructor and assignment operator
  material(const material &other) = default;
  material &operator=(const material &rhs) = default;
  // Destroys the material object
  ~material() {}
  // Gets the emissive colour value of the material
  glm::vec4 get_emissive() const { return _emissive; }
  // Sets the emissive colour value of the material
  void set_emissive(const glm::vec4 &value) { _emissive = value; }
  // Gets the diffuse reflection colour value of the material
  glm::vec4 get_diffuse() const { return _diffuse_reflection; }
  // Sets the diffuse reflection colour value of the material
  void set_diffuse(const glm::vec4 &value) { _diffuse_reflection = value; }
  // Gets the specular reflection colour value of the material
  glm::vec4 get_specular() const { return _specular_reflection; }
  // Sets the specular reflection colour value of the material
  void set_specular(const glm::vec4 &value) { _specular_reflection = value; }
  // Gets the shininess value of the material
  float get_shininess() const { return _shininess; }
  // Sets the shininess value of the material
  void set_shininess(float value) { _shininess = value; }
};
}
#pragma once

#include "stdafx.h"

namespace graphics_framework {
/*
Utility class used to store transformation data
*/
struct transform {
  // The position of the 3D object
  glm::vec3 position;
  // The orientation of the 3D object
  glm::quat orientation;
  // The scale of the 3D object
  glm::vec3 scale;

  // Creates a transform object
  transform() : scale(glm::vec3(1.0f, 1.0f, 1.0f)), position(0.f), orientation(glm::quat()) {}

  // Translates the 3D object
  void translate(const glm::vec3 &translation) { position += translation; }

  // Rotates the 3D object using Euler angles
  void rotate(const glm::vec3 &rotation) {
    glm::quat rot(rotation);
    rotate(rot);
  }

  // Rotate the 3D object using the given quaternion
  void rotate(const glm::quat &q) {
    orientation = orientation * q;
    orientation = glm::normalize(orientation);
  }

  // Gets the transformation matrix representing the defined transform
  glm::mat4 get_transform_matrix() {
    auto T = glm::translate(glm::mat4(1.0f), position);
    auto S = glm::scale(glm::mat4(1.0f), scale);
    auto R = glm::mat4_cast(orientation);
    auto matrix = T * R * S;
    return matrix;
  }

  // Gets the normal matrix representing the defined transform
  glm::mat3 get_normal_matrix() { return glm::mat3_cast(orientation); }
};
}
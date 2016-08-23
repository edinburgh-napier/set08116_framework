#include "stdafx.h"

#include "directional_light.h"

namespace graphics_framework {
// Rotates the directional light using Euler angles
void directional_light::rotate(const glm::vec3 &rotation) {
  // Convert Euler angles to a quaternion
  glm::quat q(rotation);
  // Use quaternion based rotate
  rotate(q);
}

// Rotates the directional light using the quaternion
void directional_light::rotate(const glm::quat &rotation) {
  // Get matrix transform
  auto rot = glm::mat3_cast(rotation);
  // Transform the current direction
  _direction = rot * _direction;
}
}
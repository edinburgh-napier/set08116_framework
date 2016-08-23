#include "target_camera.h"
#include "stdafx.h"

namespace graphics_framework {
// Updates the target camera
void target_camera::update(float delta_time) {
  // Calculate forward and side vectors
  glm::vec3 forward = _target - _position;
  glm::vec3 side = glm::cross(_up, forward);
  // Use forward and side to calculate up vector
  glm::vec3 up = glm::cross(forward, side);
  up = glm::normalize(up);
  // Set view matrix accordingly
  _view = glm::lookAt(_position, _target, up);
}
}
#include "arc_ball_camera.h"
#include "stdafx.h"

namespace graphics_framework {
// Updates the arc_ball_camera
void arc_ball_camera::update(float delta_time) {
  // Generate quaternion from the rotation
  glm::quat rotation(glm::vec3(_rot_X, _rot_Y, 0.0f));
  // Multiply the rotation by translation vector to generate position
  _position = _target + (rotation * glm::vec3(0.0f, 0.0f, _distance));

  // Up is standard up multiplied by rotation
  _up = rotation * glm::vec3(0.0f, 1.0f, 0.0f);

  // Calculate the view matrix
  _view = glm::lookAt(_position, _target, _up);
}

// Rotates the camera around the target
void arc_ball_camera::rotate(float delta_X, float delta_Y) {
  _rot_X += delta_X;
  _rot_Y += delta_Y;
}
}
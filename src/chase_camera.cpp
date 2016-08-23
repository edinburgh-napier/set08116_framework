#include "chase_camera.h"
#include "stdafx.h"

namespace graphics_framework {
// Updates the chase camera
void chase_camera::update(float delta_time) {
  // Calculate the combined rotation as a quaternion
  glm::quat rotation(_target_rotation + _relative_rotation);

  // Now calculate the desired position
  glm::vec3 desired_position = _target_pos + (rotation * _pos_offset);
  // Our actual position lies somewhere between our current position and the
  // desired position
  _position = glm::mix(_position, desired_position, _springiness);

  // Calculate new target offset based on rotation
  _target_offset = rotation * _target_offset;
  // Target is then the target position plus this offset
  _target = _target_pos + _target_offset;

  // Calculate up vector based on rotation
  _up = rotation * glm::vec3(0.0f, 1.0f, 0.0f);

  // Calculate view matrix
  _view = glm::lookAt(_position, _target, _up);
}

// Moves the camera by updating the target position and rotation
void chase_camera::move(const glm::vec3 &new_target_pos, const glm::vec3 &new_target_rotation) {
  _target_pos = new_target_pos;
  _target_rotation = new_target_rotation;
}

// Rotates the camera around the target
void chase_camera::rotate(const glm::vec3 &delta_rotation) { _relative_rotation += delta_rotation; }
}
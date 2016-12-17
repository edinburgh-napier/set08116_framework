#include "free_camera.h"
#include "stdafx.h"

namespace graphics_framework {
/*
Updates the free camera.  Delta time is not used
*/
void free_camera::update(float delta_time) {
  // Calculate the forward direction - spherical coordinates to Cartesian

  // Normalize forward
  auto forward = get_forward();

  // Calculate standard right.  Rotate right vector by yaw
  glm::vec3 right = glm::vec3(glm::eulerAngleY(_yaw) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  // Normalize right
  right = glm::normalize(right);

  // Up vector is up rotated by pitch
  _up = glm::cross(right, forward);
  // Normalize up
  _up = glm::normalize(_up);

  // We can now update position based on forward, up and right
  glm::vec3 trans = _translation.x * right;
  trans += _translation.y * _up;
  trans += _translation.z * forward;
  _position += trans;

  // Target vector is just our position vector plus forward vector
  _target = _position + forward;

  // Set the translation vector to zero for the next frame
  _translation = glm::vec3(0.0f, 0.0f, 0.0f);

  // We can now calculate the view matrix
  _view = glm::lookAt(_position, _target, _up);
}

/*
Rotates the camera round the y-axis (yaw) and x-axis (pitch)
*/
void free_camera::rotate(float delta_yaw, float delta_pitch) {
  // We simply add rotation values and calculate new orientation
  _pitch += delta_pitch;
  _yaw -= delta_yaw;
}

/*
Moves the free camera.  This is used in the update with the orientation to
calculate actual movement
*/
void free_camera::move(const glm::vec3 &translation) {
  // Just add translation vector to current translation
  _translation += translation;
}
glm::vec3 free_camera::get_forward() const {
  return glm::normalize(glm::vec3(cosf(_pitch) * -sinf(_yaw), sinf(_pitch), -cosf(_yaw) * cosf(_pitch)));
}
}
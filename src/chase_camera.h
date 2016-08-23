#pragma once

#include "camera.h"
#include "stdafx.h"

namespace graphics_framework {
/*
A camera that will "chase" an object around the world.  Essentially a
third person camera
*/
class chase_camera : public camera {
private:
  // The position of the target being chased
  glm::vec3 _target_pos;
  // The offset of the camera from its desired position
  glm::vec3 _pos_offset;
  // The rotation of the target being chased
  glm::vec3 _target_rotation;
  // The offset of the camera relative to the target
  glm::vec3 _target_offset;
  // The rotation of the camera relative to the target
  glm::vec3 _relative_rotation;
  // Springiness factor of the camera
  float _springiness = 5.0f;

public:
  // Creates a chase camera
  chase_camera() {}
  // Chase camera destructor
  ~chase_camera() {}
  // Gets the position of the target
  glm::vec3 get_target_pos() const { return _target_pos; }
  // Sets the position of the target
  void set_target_pos(const glm::vec3 &value) { _target_pos = value; }
  // Gets the position offset
  glm::vec3 get_pos_offset() const { return _pos_offset; }
  // Sets the position offset
  void set_pos_offset(const glm::vec3 &value) { _pos_offset = value; }
  // Gets the rotation of the target
  glm::vec3 get_target_rotation() const { return _target_rotation; }
  // Sets the rotation of the target
  void set_target_rotation(const glm::vec3 &value) { _target_rotation = value; }
  // Gets the offset from the target
  glm::vec3 get_target_offset() const { return _target_offset; }
  // Sets the offset of the target
  void set_target_offset(const glm::vec3 &value) { _target_offset = value; }
  // Gets the camera's rotation relative to the target
  glm::vec3 get_relative_rotation() const { return _relative_rotation; }
  // Sets the camera's rotation relative to the target
  void set_relative_rotation(const glm::vec3 &value) { _relative_rotation = value; }
  // Gets the springiness coefficient for the camera
  float get_springiness() const { return _springiness; }
  // Sets the springiness coefficient for the camera
  void set_springiness(float value) { _springiness = value; }
  // Updates the camera
  void update(float delta_time);
  // Moves the camera by updating the target position and rotation
  void move(const glm::vec3 &new_target_pos, const glm::vec3 &new_target_rotation);
  // Rotates the camera around the target
  void rotate(const glm::vec3 &delta_rotation);
};
}
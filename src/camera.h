#pragma once

#include "stdafx.h"

namespace graphics_framework {
/*
Base class of other camera types
*/
class camera {
protected:
  // The current position of the camera
  glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
  // The current target the camera is looking at
  glm::vec3 _target = glm::vec3(0.0f, 0.0f, -1.0f);
  // The current orientation of the camera
  glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);
  // The currently built view matrix since the last frame update
  glm::mat4 _view;
  // The currently built projection matrix since the last call to set_projection
  glm::mat4 _projection;

public:
  // Creates a new camera.  Called by the sub-classes
  camera() = default;
  // Destroys the camera.  Virtual destructor as this is an abstract class
  virtual ~camera() {}
  // Gets the current position of the camera
  const glm::vec3 &get_position() const { return _position; }
  // Sets the position of the camera
  void set_position(const glm::vec3 &value) { _position = value; }
  // Gets the current target of the camera
  const glm::vec3 &get_target() const { return _target; }
  // Sets the target of the camera
  void set_target(const glm::vec3 &value) { _target = value; }
  // Gets the current up direction of the camera
  const glm::vec3 &get_up() const { return _up; }
  // Sets the up direction of the camera
  void set_up(const glm::vec3 &value) { _up = value; }
  // Gets the current view matrix for the camera
  const glm::mat4 &get_view() const { return _view; }
  // Gets the current projection matrix for the camera
  glm::mat4 get_projection() const { return _projection; }
  // Builds projection matrix
  void set_projection(float fov, float aspect, float near, float far) {
    // Call GLM's perspective function
    _projection = glm::perspective(fov, aspect, near, far);
  }
  // Updates the camera.  This is a pure virtual function.
  virtual void update(float delta_time) = 0;
};
}
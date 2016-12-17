#pragma once

#include "camera.h"
#include "stdafx.h"

namespace graphics_framework {
class free_camera : public camera {
private:
  // Current rotation on the x-axis (pitch)
  float _pitch = 0.0f;
  // Current rotation on the y-axis (yaw)
  float _yaw = 0.0f;
  // The translation of the camera since the last frame
  glm::vec3 _translation;

public:
  // Creates a new free camera
  free_camera() : camera() {}
  // Destroys the free camera
  ~free_camera() {}
  // Updates the camera
  void update(float delta_time);
  // Get the current rotations of the camera
  float get_pitch() const { return _pitch; }
  float get_yaw() const { return _yaw; }
  // Set the current rotations of the camera
  void set_pitch(float value) { _pitch = value; }
  void set_yaw(float value) { _yaw = value; }
  // Rotates the camera by the given yaw (y-axis) and pitch (x-axis)
  void rotate(float delta_yaw, float delta_pitch);
  // Moves the camera
  void move(const glm::vec3 &translation);
  //
  glm::vec3 get_forward() const;
};
}
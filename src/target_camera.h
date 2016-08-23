#pragma once

#include "camera.h"
#include "stdafx.h"

namespace graphics_framework {
/*
An object representing a target camera
*/
class target_camera : public camera {
public:
  // Creates a target camera
  target_camera() : camera() {}
  // Default copy constructor and assignment operator
  target_camera(const target_camera &other) = default;
  target_camera &operator=(const target_camera &rhs) = default;
  // Destroys the target camera
  ~target_camera() {}
  // Updates the camera
  void update(float delta_time);
};
}
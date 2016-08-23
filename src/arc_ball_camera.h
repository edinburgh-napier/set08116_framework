#pragma once

#include "camera.h"
#include "stdafx.h"

namespace graphics_framework {
/*
A camera that targets a particular object and allows rotation around it.
A distance value dicatates how far from the target the camera is placed.
*/
class arc_ball_camera : public camera {
private:
  // Rotation around the x-axis of the target
  float _rot_X = 0.0f;
  // Rotation around the y-axis of the target
  float _rot_Y = 0.0f;
  // Distance of the camera from the target
  float _distance = 5.0f;

public:
  // Creates a new arc_ball_camera
  arc_ball_camera() {}
  // Destroys the arc ball camera
  ~arc_ball_camera() {}
  // Gets the rotation of the camera around the x-axis
  float get_rot_X() const { return _rot_X; }
  // Sets the rotation of the camera around the x-axis
  void set_rot_X(float value) { _rot_X = value; }
  // Gets the rotation of the camera around the y-axis
  float get_rot_Y() const { return _rot_Y; }
  // Sets the rotation of the camera around the y-axis
  void set_rot_Y(float value) { _rot_Y = value; }
  // Gets the distance of the camera from the target
  float get_distance() const { return _distance; }
  // Sets the distance of the camera from the target
  void set_distance(float value) { _distance = value; }
  // Updates the camera
  void update(float delta_time);
  // Move the camera relative to the target
  void move(float magnitude) { _distance += magnitude; }
  // Rotates the camera around the target
  void rotate(float delta_X, float delta_Y);
  // Moves the target position by the given vector
  void translate(const glm::vec3 &translation) { _target += translation; }
};
}
#pragma once

#include "stdafx.h"
#include "texture.h"

namespace graphics_framework {
/*
An object representing a captured frame during a render pass
*/
class frame_buffer {
private:
  // The OpenGL ID of the frame buffer
  GLuint _buffer;
  // The texture representing the captured frame
  texture _frame;
  // The texture representing the captured depth buffer
  texture _depth;
  // The width of the frame buffer
  GLuint _width;
  // The height of the frame buffer
  GLuint _height;

public:
  // Default constructor - doesn't initialise buffer
  frame_buffer() {}
  // Creates a frame buffer
  frame_buffer(GLuint width, GLuint height) throw(...);
  // Default copy constructor and assignment operator
  frame_buffer(const frame_buffer &other) = default;
  frame_buffer &operator=(const frame_buffer &rhs) = default;
  // Destroys the frame buffer
  ~frame_buffer() {}
  // Gets the OpenGL ID of the frame buffer
  GLuint get_buffer() const { return _buffer; }
  // Gets the texture representing the captured frame
  const texture &get_frame() const { return _frame; }
  // Gets the texture representing the captured depth
  const texture &get_depth() const { return _depth; }
  // Gets the width of the frame buffer
  GLuint get_width() const { return _width; }
  // Gets the height of the frame buffer
  GLuint get_height() const { return _height; }
  // Saves the frame buffer
  void save(const std::string &filename) const;
};
}
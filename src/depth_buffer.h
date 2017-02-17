#pragma once

#include "stdafx.h"
#include "texture.h"

namespace graphics_framework {
/*
Class used to describe a buffer that holds depth information from a render
pass
*/
class depth_buffer {
private:
  // The underlying OpenGL buffer
  GLuint _buffer;
  // The texture object holding the depth information
  texture _depth;
  // Width of the depth buffer
  GLuint _width;
  // Height of the depth buffer
  GLuint _height;

public:
  // Default constructor - doesn't initialise depth buffer
  depth_buffer() {}
  // Creates a new depth buffer
  depth_buffer(GLuint width, GLuint height) throw(...);
  // Default copy constructor and asignment operator
  depth_buffer(const depth_buffer &other) = default;
  depth_buffer &operator=(const depth_buffer &rhs) = default;
  // Destroys the depth buffer
  ~depth_buffer() {}
  // Gets the OpenGL buffer for the depth buffer
  GLuint get_buffer() const { return _buffer; }
  // Gets the texture that is storing the depth information
  const texture &get_depth() const { return _depth; }
  // Gets the width of the depth buffer
  GLuint get_width() const { return _width; }
  // Gets the height of the depth buffer
  GLuint get_height() const { return _height; }
  // Saves the depth buffer
  void save(const std::string &filename, const bool linear = true) const;
};
}
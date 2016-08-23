#pragma once

#include "stdafx.h"

namespace graphics_framework {
/*
An object that contains shader effect information
*/
class effect {
private:
  // The OpenGL ID of the shader object
  GLuint _program;
  // The IDs of the shaders bound to this effect
  std::vector<GLuint> _shaders;

public:
  // Creates an effect object
  effect() throw(...);
  // Default copy and assignment constructors
  effect(const effect &other) = default;
  effect &operator=(const effect &rhs) = default;
  // Destroys an effect object
  ~effect() {}
  // Gets the OpenGL program ID
  GLuint get_program() const { return _program; }
  // Adds a shader to the effect object
  void add_shader(const std::string &filename, GLenum type);
  // Adds a collection of shaders of a given type to the effect
  void add_shader(const std::vector<std::string> &filenames, GLenum type);
  // Builds the effect object
  void build() throw(...);
  // Gets the location of the uniform in the shader
  GLint get_uniform_location(const std::string &name) const;
};
}
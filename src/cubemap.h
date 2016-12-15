#pragma once

#include "stdafx.h"

namespace graphics_framework {
/*
A class representing a cubemap texture
*/
class cubemap {
private:
  // The id for the cubemap texture in OpenGL
  GLuint _id;

public:
  // Creates a new cubemap
  cubemap() {}
  // Creates a new cubemap given six filenames
  explicit cubemap(const std::array<std::string, 6> &filenames) throw(...);
  // Default copy constructor and assignment operator
  cubemap(const cubemap &other) = default;
  cubemap &operator=(const cubemap &rhs) = default;
  // Destroys the cubemap
  ~cubemap() {}
  // Gets the OpenGL id for the cubemap texture
  GLuint get_id() const { return _id; }
  // Sets a texture for one part of the cubemap
  bool set_texture(GLenum target, const std::string &filename) throw(...);
};
}
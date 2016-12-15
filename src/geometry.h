#pragma once

#include "stdafx.h"

namespace graphics_framework {
/*
Enumeration describing the default buffer locations used by the render
framework
*/
enum BUFFER_INDEXES {
  // The position data
  POSITION_BUFFER = 0,
  // The colour data
  COLOUR_BUFFER = 1,
  // The surface normals
  NORMAL_BUFFER = 2,
  // The binormals for the surfaces
  BINORMAL_BUFFER = 3,
  // The tangents for the surfaces
  TANGENT_BUFFER = 4,
  // Texture coordinates 0
  TEXTURE_COORDS_0 = 10,
  // Texture coordinates 1
  TEXTURE_COORDS_1 = 11,
  // Texture coordinates 2
  TEXTURE_COORDS_2 = 12,
  // Texture coordinates 3
  TEXTURE_COORDS_3 = 13,
  // Texture coordinates 4
  TEXTURE_COORDS_4 = 14,
  // Texture coordinates 5
  TEXTURE_COORDS_5 = 15
};

/*
Object describing a piece of geometry
*/
class geometry {
private:
  // The primitive geometry type used by the geometry
  GLenum _type;
  // The OpenGL ID of the vertex array object
  GLuint _vao;
  // The OpenGL IDs of the buffers used within the vertex array object
  std::map<GLuint, GLuint> _buffers;
  // The OpenGL ID of the index buffer
  GLuint _index_buffer;
  // The number of vertices in the geometry
  GLuint _vertices;
  // The number of indices in the index buffer
  GLuint _indices;
  // The minimal point of the geometry
  glm::vec3 _minimal = glm::vec3(0.0f, 0.0f, 0.0f);
  // The maximal point of the geometry
  glm::vec3 _maximal = glm::vec3(0.0f, 0.0f, 0.0f);

public:
  // Creates a geometry object
  geometry() throw(...);
  // Creates a geometry object from a model file
  explicit geometry(const std::string &filename) throw(...);
  // Move constructor
  geometry(geometry &&other);
  // Default copy constructor and assignment operator
  geometry(const geometry &other) = default;
  geometry &operator=(const geometry &rhs) = default;
  // Destroys the geometry object
  ~geometry() {}
  // Gets the type of the geometry
  GLenum get_type() const { return _type; }
  // Sets the type of the geometry
  void set_type(GLenum value) { _type = value; }
  // Gets the OpenGL ID of the vertex array object
  GLuint get_array_object() const { return _vao; }
  // Gets the OpenGL ID of the buffer with the given index in the geometry
  GLuint get_buffer(const GLuint idx) const { return _buffers.at(idx); }
  // Gets the OpenGL ID of the index buffer
  GLuint get_idx_buffer() const { return _index_buffer; }
  // Gets the number of vertices in the geometry object
  GLuint get_vertex_count() const { return _vertices; }
  // Gets the number of indices in the index buffer
  GLuint get_index_count() const { return _indices; }
  // Adds a buffer of vec2 data to the geometry object
  bool add_buffer(const std::vector<glm::vec2> &buffer, GLuint index, GLenum buffer_type = GL_STATIC_DRAW);
  // Adds a buffer of vec3 data to the geometry object
  bool add_buffer(const std::vector<glm::vec3> &buffer, GLuint index, GLenum buffer_type = GL_STATIC_DRAW);
  // Adds a buffer of vec4 data to the geometry object
  bool add_buffer(const std::vector<glm::vec4> &buffer, GLuint index, GLenum buffer_type = GL_STATIC_DRAW);
  // Adds an index buffer to the geometry object
  bool add_index_buffer(const std::vector<GLuint> &buffer);
  // Gets the minimal point of the geometry
  glm::vec3 get_minimal_point() const { return _minimal; }
  // Sets the minimal point of the geometry
  void set_minimal_point(const glm::vec3 &value) { _minimal = value; }
  // Gets the maximal point of the geometry
  glm::vec3 get_maximal_point() const { return _maximal; }
  // Sets the maximal point of the geometry
  void set_maximal_point(const glm::vec3 &value) { _maximal = value; }
};
}
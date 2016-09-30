#include "stdafx.h"

#include "geometry.h"
#include "util.h"

namespace graphics_framework {
/*
Creates a new geometry object
*/
geometry::geometry() throw(...) : _type(GL_TRIANGLES), _vao(0), _index_buffer(0), _vertices(0), _indices(0) {}

/*
Creates a piece of geometry by loading in a model
*/
geometry::geometry(const std::string &filename) throw(...) : geometry() {
	/*
  // Check that file exists
  assert(check_file_exists(filename));
  // Create model importer
  Assimp::Importer model_importer;
  // Read in the model data
  auto sc = model_importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                                  aiProcess_ValidateDataStructure | aiProcess_FindInvalidData);
  // Check that data has been read in correctly
  if (!sc) {
    // Display error
    std::cerr << "ERROR - loading geometry " << filename << std::endl;
    std::cerr << model_importer.GetErrorString() << std::endl;
    // Throw exception
    throw std::runtime_error("Error reading in model file");
  }
  // TODO - read in multiple texture coordinates
  // TODO - mesh hierarchy?
  // TODO - bones
  // TODO - multiple colour values
  // Declare vectors to store data
  std::vector<glm::vec3> positions;
  std::vector<glm::vec4> colours;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> tex_coords;
  std::vector<GLuint> indices;
  unsigned int vertex_begin = 0;
  // Iterate through each sub-mesh in the model
  for (unsigned int n = 0; n < sc->mNumMeshes; ++n) {
    // Get the sub-mesh
    auto mesh = sc->mMeshes[n];
    // Iterate through all the vertices in the sub-mesh
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
      // Get position vertex
      auto pos = mesh->mVertices[i];
      // Add to positions data
      positions.push_back(glm::vec3(pos.x, pos.y, pos.z));
    }
    // If we have colour data then iterate through them
    if (mesh->HasVertexColors(0))
      // Iterate through colour data
      for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        // Get the colour data from the mesh
        auto col = mesh->mColors[0][i];
        // Add to colour data vector
        colours.push_back(glm::vec4(col.r, col.g, col.b, col.a));
      }
    // Otherwise just push back grey
    else
      for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        colours.push_back(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
    // If we have normals, then add to normal data
    if (mesh->HasNormals())
      for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        auto norm = mesh->mNormals[i];
        normals.push_back(glm::vec3(norm.x, norm.y, norm.z));
      }
    // If we have texture coordinates then add to texture coordinate data
    if (mesh->HasTextureCoords(0))
      for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        auto tex_coord = mesh->mTextureCoords[0][i];
        tex_coords.push_back(glm::vec2(tex_coord.x, tex_coord.y));
      }
    // If we have face information, then add to index buffer
    if (mesh->HasFaces())
      for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
        auto face = mesh->mFaces[f];
        for (auto i = 0; i < 3; ++i)
          indices.push_back(vertex_begin + face.mIndices[i]);
      }
    vertex_begin += mesh->mNumVertices;
  }

  // Calculate the minimal and maximal
  for (auto &v : positions) {
    _minimal = glm::min(_minimal, v);
    _maximal = glm::max(_maximal, v);
  }

  // Add the buffers to the geometry
  add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);
  if (normals.size() != 0)
    add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
  if (tex_coords.size() != 0)
    add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
  if (indices.size() != 0)
    add_index_buffer(indices);

  // Log success
  std::clog << "LOG - geometry " << filename << " loaded" << std::endl;
  */
}

// Move constructor
geometry::geometry(geometry &&other) {
  _type = other._type;
  _vao = other._vao;
  _buffers = std::move(other._buffers);
  _index_buffer = other._index_buffer;
  _vertices = other._vertices;
  _indices = other._indices;
  _minimal = other._minimal;
  _maximal = other._maximal;
  other._buffers = std::map<GLuint, GLuint>();
}

// Adds a buffer to the geometry object
bool geometry::add_buffer(const std::vector<glm::vec2> &buffer, GLuint index, GLenum buffer_type) {
  // Check that index is viable
  assert(index < 16);
  // Check that buffer is not empty
  assert(buffer.size() > 0);
  // Check if geometry initialised
  if (_vao == 0) {
    // Create the vertex array object
    glGenVertexArrays(1, &_vao);
    // Check for any OpenGL error
    if (CHECK_GL_ERROR) {
      // Display error
      std::cerr << "ERROR - creating geometry" << std::endl;
      std::cerr << "Could not generate vertex array object" << std::endl;
      // Set vertex array object to 0
      _vao = 0;
      // Throw exception
      throw std::runtime_error("Error creating vertex array object with OpenGL");
    }
  }
  // If we have no vertices yet, set the vertices to the size of this buffer
  if (_vertices == 0)
    _vertices = static_cast<GLuint>(buffer.size());
  // Otherwise ensure that the number of vertices matches
  else if (_vertices != buffer.size()) {
    std::cerr << "ERROR - adding buffer to geometry object" << std::endl;
    std::cerr << "Buffer does not contain correct amount of vertices" << std::endl;
    return false;
  }
  // Now add buffer to the vertex array object.  Bind the vertex array object
  glBindVertexArray(_vao);
  // Generate buffer with OpenGL
  GLuint id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  // Set the buffer data
  glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(glm::vec2), &buffer[0], buffer_type);
  // Set the vertex pointer and enable
  glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(index);
  // Check for OpenGL error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - adding buffer to geometry object" << std::endl;
    std::cerr << "Could not create buffer with OpenGL" << std::endl;
    return false;
  }
  // Add buffer to map
  _buffers[index] = id;
  return true;
}

// Adds a buffer to the geometry object
bool geometry::add_buffer(const std::vector<glm::vec3> &buffer, GLuint index, GLenum buffer_type) {
  // Check that index is viable
  assert(index < 16);
  // Check that buffer is not empty
  assert(buffer.size() > 0);
  // Check if geometry initialised
  if (_vao == 0) {
    // Create the vertex array object
    glGenVertexArrays(1, &_vao);
    // Check for any OpenGL error
    if (CHECK_GL_ERROR) {
      // Display error
      std::cerr << "ERROR - creating geometry" << std::endl;
      std::cerr << "Could not generate vertex array object" << std::endl;
      // Set vertex array object to 0
      _vao = 0;
      // Throw exception
      throw std::runtime_error("Error creating vertex array object with OpenGL");
    }
  }
  // If we have no vertices yet, set the vertices to the size of this buffer
  if (_vertices == 0)
    _vertices = static_cast<GLuint>(buffer.size());
  // Otherwise ensure that the number of vertices matches
  else if (_vertices != buffer.size()) {
    std::cerr << "ERROR - adding buffer to geometry object" << std::endl;
    std::cerr << "Buffer does not contain correct amount of vertices" << std::endl;
    return false;
  }
  // Now add buffer to the vertex array object.  Bind the vertex array object
  glBindVertexArray(_vao);
  // Generate buffer with OpenGL
  GLuint id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  // Set the buffer data
  glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(glm::vec3), &buffer[0], buffer_type);
  // Set the vertex pointer and enable
  glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(index);
  // Check for OpenGL error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - adding buffer to geometry object" << std::endl;
    std::cerr << "Could not create buffer with OpenGL" << std::endl;
    return false;
  }
  // Add buffer to map
  _buffers[index] = id;
  return true;
}

// Adds a buffer to the geometry object
bool geometry::add_buffer(const std::vector<glm::vec4> &buffer, GLuint index, GLenum buffer_type) {
  // Check that index is viable
  assert(index < 16);
  // Check that buffer is not empty
  assert(buffer.size() > 0);
  // Check if geometry initialised
  if (_vao == 0) {
    // Create the vertex array object
    glGenVertexArrays(1, &_vao);
    // Check for any OpenGL error
    if (CHECK_GL_ERROR) {
      // Display error
      std::cerr << "ERROR - creating geometry" << std::endl;
      std::cerr << "Could not generate vertex array object" << std::endl;
      // Set vertex array object to 0
      _vao = 0;
      // Throw exception
      throw std::runtime_error("Error creating vertex array object with OpenGL");
    }
  }
  // If we have no vertices yet, set the vertices to the size of this buffer
  if (_vertices == 0)
    _vertices = static_cast<GLuint>(buffer.size());
  // Otherwise ensure that the number of vertices matches
  else if (_vertices != buffer.size()) {
    std::cerr << "ERROR - adding buffer to geometry object" << std::endl;
    std::cerr << "Buffer does not contain correct amount of vertices" << std::endl;
    return false;
  }
  // Now add buffer to the vertex array object.  Bind the vertex array object
  glBindVertexArray(_vao);
  // Generate buffer with OpenGL
  GLuint id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  // Set the buffer data
  glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(glm::vec4), &buffer[0], buffer_type);
  // Set the vertex pointer and enable
  glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(index);
  // Check for OpenGL error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - adding buffer to geometry object" << std::endl;
    std::cerr << "Could not create buffer with OpenGL" << std::endl;
    return false;
  }
  // Add buffer to map
  _buffers[index] = id;
  return true;
}

// Adds an index buffer to the geometry
bool geometry::add_index_buffer(const std::vector<GLuint> &buffer) {
  // Check that buffer is not empty
  assert(buffer.size() > 0);
  // Check if vertex array object is valid
  assert(_vao != 0);
  // Set indices to buffer size
  _indices = static_cast<GLuint>(buffer.size());
  // Bind vertex array object
  glBindVertexArray(_vao);
  // Add buffer
  glGenBuffers(1, &_index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.size() * sizeof(GLuint), &buffer[0], GL_STATIC_DRAW);
  // Check for error
  if (CHECK_GL_ERROR) {
    std::cerr << "Error - adding index buffer to geometry object" << std::endl;
    std::cerr << "Could not create buffer with OpenGL" << std::endl;
    return false;
  }
  return true;
}
}
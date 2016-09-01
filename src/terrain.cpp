#include "stdafx.h"

#include "terrain.h"
#include "texture.h"

namespace graphics_framework {
terrain::terrain(const std::string &heightmap) : terrain() {
  // Load texture
  texture tex(heightmap);

  // Contains our position data
  std::vector<glm::vec3> positions;
  // Contains our normal data
  std::vector<glm::vec3> normals;
  // Contains our texture coordinate data
  std::vector<glm::vec2> tex_coords;
  // Contains our texture weights
  std::vector<glm::vec4> tex_weights;
  // Contains our index data
  std::vector<unsigned int> indices;

  // Extract the texture data from the image
  glBindTexture(GL_TEXTURE_2D, tex.get_id());
  auto data = new glm::vec4[tex.get_width() * tex.get_height()];
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, (void *)data);

  // Determine ratio of height map to geometry
  float width_point = 1.0f;
  float depth_point = 1.0f;

  // Point to work on
  glm::vec3 point;

  // Iterate through each point, calculate vertex and add to vector
  for (GLuint x = 0; x < tex.get_width(); ++x) {
    // Calculate x position of point
    point.x = -(tex.get_width() / 2.0f) + (width_point * static_cast<float>(x));

    for (GLuint z = 0; z < tex.get_height(); ++z) {
      // Calculate z position of point
      point.z = -(tex.get_height() / 2.0f) + (depth_point * static_cast<float>(z));
      // Y position based on red component of height map data
      point.y = data[(z * tex.get_width()) + x].y * 10.0f;
      // Add point to position data
      positions.push_back(point);
    }
  }

  // Add index data
  for (unsigned int x = 0; x < tex.get_width() - 1; ++x) {
    for (unsigned int y = 0; y < tex.get_height() - 1; ++y) {
      // Get four corners of patch
      unsigned int top_left = (y * tex.get_width()) + x;
      unsigned int top_right = (y * tex.get_width()) + x + 1;
      unsigned int bottom_left = ((y + 1) * tex.get_width()) + x;
      unsigned int bottom_right = ((y + 1) * tex.get_width()) + x + 1;
      // Push back indices for triangle 1
      indices.push_back(top_left);
      indices.push_back(bottom_right);
      indices.push_back(bottom_left);
      // Push back indices for triangle 2
      indices.push_back(top_left);
      indices.push_back(top_right);
      indices.push_back(bottom_right);
    }
  }

  // Resize the normals buffer
  normals.resize(positions.size());

  // Calculate normals for the height map
  for (unsigned int i = 0; i < indices.size() / 3; ++i) {
    // Get indices for the triangle
    auto i1 = indices[i * 3];
    auto i2 = indices[i * 3 + 1];
    auto i3 = indices[i * 3 + 2];

    // Calculate two sides of the triangle
    glm::vec3 side1 = positions[i1] - positions[i3];
    glm::vec3 side2 = positions[i1] - positions[i2];

    // Normal is cross product of these two sides
    glm::vec3 normal = glm::normalize(glm::cross(side2, side1));

    // Set the normals in the normal buffer using the indices for the triangle
    normals[i1] = normal;
    normals[i2] = normal;
    normals[i3] = normal;
  }

  // Add texture coordinates for geometry
  for (unsigned int x = 0; x < tex.get_width(); ++x) {
    for (unsigned int z = 0; z < tex.get_height(); ++z) {
      tex_coords.push_back(glm::vec2(width_point * x, depth_point * z));
    }
  }

  // Part 4 - Calculate texture weights for each vertex
  for (unsigned int x = 0; x < tex.get_width(); ++x) {
    for (unsigned int z = 0; z < tex.get_height(); ++z) {
      // Calculate tex weight
      glm::vec4 tex_weight(glm::clamp(1.0f - std::abs(data[(tex.get_width() * z) + x].y - 0.0f) / 0.25f, 0.0f, 1.0f),
                           glm::clamp(1.0f - std::abs(data[(tex.get_width() * z) + x].y - 0.15f) / 0.25f, 0.0f, 1.0f),
                           glm::clamp(1.0f - std::abs(data[(tex.get_width() * z) + x].y - 0.5f) / 0.25f, 0.0f, 1.0f),
                           glm::clamp(1.0f - std::abs(data[(tex.get_width() * z) + x].y - 0.9f) / 0.25f, 0.0f, 1.0f));

      // Sum the components of the vector
      auto total = tex_weight.x + tex_weight.y + tex_weight.z + tex_weight.w;

      // Divide weight by sum
      tex_weight /= total;

      // Add tex weight to weights
      tex_weights.push_back(tex_weight);
    }
  }

  // Add necessary buffers to the geometry
  _geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  _geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
  _geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
  _geom.add_buffer(tex_weights, BUFFER_INDEXES::TEXTURE_COORDS_1);
  _geom.add_index_buffer(indices);

  // Delete data
  delete[] data;
}
}
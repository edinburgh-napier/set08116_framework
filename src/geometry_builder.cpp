#include "stdafx.h"

#include "geometry_builder.h"

namespace graphics_framework {
// Generates tangents and binormals for geometry
void generate_tb(geometry &geom, const std::vector<glm::vec3> &normals) {
  // Declare tangent and binormal buffers
  std::vector<glm::vec3> tangent_data;
  std::vector<glm::vec3> binormal_data;
  // Iterate through each normal and generate
  for (unsigned int i = 0; i < geom.get_vertex_count(); ++i) {
    // Determine if tangent value.  Get orthogonal with forward and up vectors
    // Orthogonal to forward vector
    glm::vec3 c1 = glm::cross(normals[i], glm::vec3(0.0f, 0.0f, 1.0f));
    // Orthogonal to up vector
    glm::vec3 c2 = glm::cross(normals[i], glm::vec3(0.0f, 1.0f, 0.0f));
    // Determine which vector has greater length.  This will be the tangent
    if (glm::length(c1) > glm::length(c2))
      tangent_data.push_back(glm::normalize(c1));
    else
      tangent_data.push_back(glm::normalize(c2));

    // Generate binormal from tangent and normal
    binormal_data.push_back(glm::normalize(glm::cross(normals[i], tangent_data[i])));
  }

  // Add the new buffers to the geometry
  geom.add_buffer(tangent_data, BUFFER_INDEXES::TANGENT_BUFFER);
  geom.add_buffer(binormal_data, BUFFER_INDEXES::BINORMAL_BUFFER);
}

// Data required for box geometry
glm::vec3 box_positions[24] = {
    // Front
    glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),
    glm::vec3(0.5f, -0.5f, 0.5f),

    // Right side
    glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f),
    glm::vec3(0.5f, -0.5f, -0.5f),

    // Back
    glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),

    // Left side
    glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(-0.5f, -0.5f, 0.5f),

    // Top
    glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
    glm::vec3(0.5f, 0.5f, 0.5f),

    // Bottom
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f),
    glm::vec3(-0.5f, -0.5f, 0.5f)};

// Normals for the box geometry
glm::vec3 box_normals[6] = {glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                            glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)};

// Box texture coordinates
glm::vec2 box_texcoords[4] = {glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f),
                              glm::vec2(1.0f, 0.0f)};

// Creates box geometry
geometry geometry_builder::create_box(const glm::vec3 &dims) {
  // Standard minimal and maximal
  glm::vec3 minimal(0.0f, 0.0f, 0.0f);
  glm::vec3 maximal(0.0f, 0.0f, 0.0f);

  // Type of geometry generated will be quads
  geometry geom;
  geom.set_type(GL_QUADS);
  // Declare required buffers - positions, normals, texture coordinates and colour
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> tex_coords;
  std::vector<glm::vec4> colours;

  // Iterate through each position and add to buffer
  for (unsigned int i = 0; i < 24; ++i) {
    // Calculate position
    glm::vec3 pos = box_positions[i] * dims;
    // Add the position data.  Multiply by dimension
    positions.push_back(pos);
    // Recalculate minimal and maximal
    minimal = glm::min(minimal, pos);
    maximal = glm::max(maximal, pos);
    // Normal is one of the six defined.  Divide index by 4 to get the value
    normals.push_back(box_normals[i / 4]);
    // Set the colour to be light grey
    colours.push_back(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  }
  // Set minimal and maximal
  geom.set_minimal_point(minimal);
  geom.set_maximal_point(maximal);
  // Texture coordinates based on side
  // Front
  for (unsigned int i = 0; i < 4; ++i)
    tex_coords.push_back(box_texcoords[i] * glm::vec2(dims));
  // Right
  for (unsigned int i = 0; i < 4; ++i)
    tex_coords.push_back(box_texcoords[i] * glm::vec2(dims.z, dims.y));
  // Back
  for (unsigned int i = 0; i < 4; ++i)
    tex_coords.push_back(box_texcoords[i] * glm::vec2(dims));
  // Left
  for (unsigned int i = 0; i < 4; ++i)
    tex_coords.push_back(box_texcoords[i] * glm::vec2(dims.z, dims.y));
  // Top
  for (unsigned int i = 0; i < 4; ++i)
    tex_coords.push_back(box_texcoords[i] * glm::vec2(dims.x, dims.z));
  // Bottom
  for (unsigned int i = 0; i < 4; ++i)
    tex_coords.push_back(box_texcoords[i] * glm::vec2(dims.x, dims.z));

  // Add buffers to geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
  geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);
  geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);

  // Generate tangents and binormals
  generate_tb(geom, normals);

  // Return geometry
  return std::move(geom);
}

// Tetrahedron data
glm::vec3 tetra_positions[12] = {
    // Side 1
    glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(-0.5f, -0.5f, 0.5f),

    // Side 2
    glm::vec3(0.0f, -0.5f, -0.5f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.5f),

    // Side 3
    glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, -0.5f, -0.5f),

    // Bottom
    glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f)};

// Texture coordinates for the tetrahedron geometry
glm::vec2 tetra_texcoords[3] = {glm::vec2(1.0f, 0.0f), glm::vec2(0.5f, 1.0f), glm::vec2(0.0f, 0.0f)};

// Creates tetrahedron geometry
geometry geometry_builder::create_tetrahedron(const glm::vec3 &dims) {
  // Type of geometry generated will be triangles
  geometry geom;
  geom.set_type(GL_TRIANGLES);
  // Declare required buffers - positions, normals, texture coordinates and colour
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> tex_coords;
  std::vector<glm::vec4> colours;

  // The minimal and maximal values
  glm::vec3 minimal(0.0f, 0.0f, 0.0f);
  glm::vec3 maximal(0.0f, 0.0f, 0.0f);

  // Iterate through each position and add to buffers
  for (unsigned int i = 0; i < 12; ++i) {
    // Calculate position. Multiply by dimensions.
    auto pos = tetra_positions[i] * dims;
    // Add the position ot the position data.
    positions.push_back(pos);
    // Calculate minmal and maximal
    minimal = glm::min(minimal, pos);
    maximal = glm::max(maximal, pos);
    // Set colour to light grey
    colours.push_back(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  }

  // Set minimal and maximal
  geom.set_minimal_point(minimal);
  geom.set_maximal_point(maximal);

  // For normals use cross product (dimensional scaling)
  for (unsigned int i = 0; i < 12; i += 3) {
    // Calculate normal from position data
    auto v1 = positions[i + 1] - positions[i];
    auto v2 = positions[i + 2] - positions[i];
    auto norm = glm::normalize(glm::cross(v1, v2));
    // Add to normals
    for (unsigned int j = 0; j < 3; ++j)
      normals.push_back(norm);
  }
  // Texture coordinates done seperately, based on side
  for (unsigned int i = 0; i < 3; ++i)
    tex_coords.push_back(tetra_texcoords[i] * glm::vec2(dims));
  for (unsigned int i = 0; i < 3; ++i)
    tex_coords.push_back(tetra_texcoords[i] * glm::vec2(dims.z, dims.y));
  for (unsigned int i = 0; i < 3; ++i)
    tex_coords.push_back(tetra_texcoords[i] * glm::vec2(dims.z, dims.y));
  for (unsigned int i = 0; i < 3; ++i)
    tex_coords.push_back(tetra_texcoords[i] * glm::vec2(dims.z, dims.x));

  // Add buffers to geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
  geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);
  geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);

  // Generate tangent and binormal data
  generate_tb(geom, normals);

  // Return geometry
  return std::move(geom);
}

// Pyramid data
glm::vec3 pyramid_positions[18] = {
    // Front
    glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(-0.5f, -0.5f, 0.5f),
    // Right
    glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.5f),
    // Back
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, -0.5f),
    // Left
    glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(-0.5f, -0.5f, -0.5f),
    // Bottom 1
    glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, -0.5f),
    // Bottom 2
    glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f)};

// Creates pyramid geometry
geometry geometry_builder::create_pyramid(const glm::vec3 &dims) {
  // Type of geometry generated will be triangles
  geometry geom;
  geom.set_type(GL_TRIANGLES);
  // Declare required buffers - positions, normals, texture coordinates and colour
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> tex_coords;
  std::vector<glm::vec4> colours;

  // The minimal and maximal points
  glm::vec3 minimal(0.0f, 0.0f, 0.0f);
  glm::vec3 maximal(0.0f, 0.0f, 0.0f);

  // Iterate through each position and add to geometry
  for (unsigned int i = 0; i < 18; ++i) {
    // Calculate position.  Multiply by dimensions.
    auto pos = pyramid_positions[i] * dims;
    // Add the position to position data.
    positions.push_back(pos);
    // Calculate minimal and maximal
    minimal = glm::min(minimal, pos);
    maximal = glm::max(maximal, pos);
    // Set the colour to be light grey
    colours.push_back(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  }
  // Set minimal and maximal
  geom.set_minimal_point(minimal);
  geom.set_maximal_point(maximal);
  // For normals use the cross product because of dimensional scaling
  for (unsigned int i = 0; i < 18; i += 3) {
    auto v1 = positions[i + 1] - positions[i];
    auto v2 = positions[i + 2] - positions[i];
    auto norm = glm::normalize(glm::cross(v1, v2));
    for (unsigned int j = 0; j < 3; ++j)
      normals.push_back(norm);
  }
  // Texture coordinates done seperately based on side.
  // Front, right, back and left based on tetrahedron
  for (unsigned int i = 0; i < 3; ++i)
    tex_coords.push_back(tetra_texcoords[i] * glm::vec2(dims));
  for (unsigned int i = 0; i < 3; ++i)
    tex_coords.push_back(tetra_texcoords[i] * glm::vec2(dims.z, dims.y));
  for (unsigned int i = 0; i < 3; ++i)
    tex_coords.push_back(tetra_texcoords[i] * glm::vec2(dims));
  for (unsigned int i = 0; i < 3; ++i)
    tex_coords.push_back(tetra_texcoords[i] * glm::vec2(dims.z, dims.y));
  // Bottom based on box
  // Bottom
  for (int i = 0; i < 3; ++i)
    tex_coords.push_back(box_texcoords[i] * glm::vec2(dims.x, dims.z));
  tex_coords.push_back(box_texcoords[0] * glm::vec2(dims.x, dims.z));
  tex_coords.push_back(box_texcoords[2] * glm::vec2(dims.x, dims.z));
  tex_coords.push_back(box_texcoords[3] * glm::vec2(dims.x, dims.z));

  // Add buffers to geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
  geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);
  geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);

  // Generate tangent and binormal data
  generate_tb(geom, normals);

  // Return geometry
  return std::move(geom);
}

// Creates disk geometry
geometry geometry_builder::create_disk(const unsigned int slices, const glm::vec2 &dims) {
  // Type of geometry generated will be triangles
  geometry geom;
  geom.set_type(GL_TRIANGLE_FAN);
  // Declare required buffers - positions, normals, texture coordinates and colour
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> tex_coords;
  std::vector<glm::vec4> colours;

  // Minimal and maximal points
  glm::vec3 minimal(0.0f, 0.0f, 0.0f);
  glm::vec3 maximal(0.0f, 0.0f, 0.0f);

  // Values to work with - centre point
  // Push centre
  positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
  // Push normal
  normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
  // Texture coordinate
  tex_coords.push_back(glm::vec2(0.5f, 0.5f));
  // Colour
  colours.push_back(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));

  // Working position
  glm::vec3 pos(0.5f * dims.x, 0.0f, 0.0f);
  // Recalculate minimal and maximal
  minimal = glm::min(minimal, pos);
  maximal = glm::max(maximal, pos);
  // Push first point into buffers
  positions.push_back(pos);
  normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
  tex_coords.push_back(glm::vec2(tex_coords[0].x + pos.x, tex_coords[0].y - pos.z));
  colours.push_back(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  // Angle per slice
  auto delta_angle = (2.0 * glm::pi<double>()) / static_cast<double>(slices);
  // Loop round each slice
  for (unsigned int i = 1; i < slices + 1; ++i) {
    // Calculate next position
    pos = glm::vec3(cos(i * delta_angle) * dims.x / 2.0f, 0.0f, -sin(i * delta_angle) * dims.y / 2.0f);
    // Recalculate minimal and maximal
    minimal = glm::min(minimal, pos);
    maximal = glm::max(maximal, pos);
    // Push position, normal, tex coord and colour
    positions.push_back(pos);
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    tex_coords.push_back(glm::vec2(tex_coords[0].x + pos.x, tex_coords[0].y - pos.z));
    colours.push_back(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
  }
  // Set minimal and maximal points
  geom.set_minimal_point(minimal);
  geom.set_maximal_point(maximal);

  // Add buffers to geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
  geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);
  geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);

  // Generate tangent and binormal data
  generate_tb(geom, normals);

  // Return geometry
  return std::move(geom);
}

// Creates cylinder geometry
geometry geometry_builder::create_cylinder(const unsigned int stacks, const unsigned int slices,
                                           const glm::vec3 &dims) {
  // Type of geometry generated will be triangles
  geometry geom;
  geom.set_type(GL_TRIANGLES);
  // Declare required buffers - positions, normals, texture coordinates and colour
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> tex_coords;
  std::vector<glm::vec4> colours;

  // Minimal and maximal points
  glm::vec3 minimal(0.0f, 0.0f, 0.0f);
  glm::vec3 maximal(0.0f, 0.0f, 0.0f);

  // Create top - similar to disk but now using triangles
  glm::vec3 centre(0.0f, 0.5f * dims.y, 0.0f);
  // Recalculate minimal and maximal
  minimal = glm::min(minimal, centre);
  maximal = glm::max(maximal, centre);
  auto prev_vert = glm::vec3(0.5f, 0.5f, 0.0f) * dims;
  // Recalculate minimal and maximal
  minimal = glm::min(minimal, prev_vert);
  maximal = glm::max(maximal, prev_vert);
  glm::vec3 curr_vert;
  glm::vec2 tex_coord(0.5f, 0.5f);
  // Angle per slice
  auto delta_angle = (2.0f * glm::pi<float>()) / static_cast<float>(slices);
  // Iterate through each slice
  for (unsigned int i = 1; i <= slices; ++i) {
    // Calculate unit length vertex
    curr_vert = glm::vec3(cos(i * delta_angle), 1.0f, -sin(i * delta_angle));
    // We want radius to be 1
    curr_vert /= 2.0f;
    // Multiply by dimensions
    curr_vert *= dims;
    // Recalculate minimal and maximal
    // Recalculate minimal and maximal
    minimal = glm::min(minimal, curr_vert);
    maximal = glm::max(maximal, curr_vert);
    // Push back vertices
    positions.push_back(centre);
    positions.push_back(prev_vert);
    positions.push_back(curr_vert);
    // Push back normals and colours
    for (unsigned int j = 0; j < 3; ++j) {
      normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
      colours.push_back(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
    }
    // Push back tex coordinates
    tex_coords.push_back(tex_coord);
    tex_coords.push_back(glm::vec2(tex_coord.x + prev_vert.x, tex_coord.y - prev_vert.z));
    tex_coords.push_back(glm::vec2(tex_coord.x + curr_vert.x, tex_coord.y - curr_vert.z));
    // Set previous as current
    prev_vert = curr_vert;
  }

  // Create bottom - same process as top
  centre = glm::vec3(0.0f, -0.5f * dims.y, 0.0f);
  // Recalculate minimal and maximal
  minimal = glm::min(minimal, centre);
  maximal = glm::max(maximal, centre);

  prev_vert = glm::vec3(0.5f, -0.5f, 0.0f) * dims;
  // Recalculate minimal and maximal
  minimal = glm::min(minimal, prev_vert);
  maximal = glm::max(maximal, prev_vert);
  // Iterate for each slice
  for (unsigned int i = 1; i <= slices; ++i) {
    // Calculate unit length vertex
    curr_vert = glm::vec3(cos(i * delta_angle), -1.0f, sin(i * delta_angle));
    // We want radius to be 1
    curr_vert /= 2.0f;
    // Multiply by dimensions
    curr_vert *= dims;
    // Recalculate minimal and maximal
    minimal = glm::min(minimal, curr_vert);
    maximal = glm::max(maximal, curr_vert);
    // Push back vertices
    positions.push_back(centre);
    positions.push_back(prev_vert);
    positions.push_back(curr_vert);
    // Push back normals and colours
    for (unsigned int j = 0; j < 3; ++j) {
      normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
      colours.push_back(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
    }
    // Push back texture coordinates
    tex_coords.push_back(tex_coord);
    tex_coords.push_back(glm::vec2(tex_coord.x - prev_vert.x, tex_coord.y - prev_vert.z));
    tex_coords.push_back(glm::vec2(tex_coord.x - curr_vert.x, tex_coord.y - curr_vert.z));
    // Set previous as current
    prev_vert = curr_vert;
  }

  // Create stacks
  std::array<glm::vec3, 4> verts;
  std::array<glm::vec2, 4> coords;
  // Delta height - scaled during vertex creation
  auto delta_height = 2.0f / static_cast<float>(stacks);
  // Calculate circumference - could be ellipitical
  auto circ =
      glm::pi<float>() * ((3.0f * (dims.x + dims.z)) - (sqrtf((3.0f * dims.x + dims.z) * (dims.x + 3.0f * dims.z))));
  // Delta width is the circumference divided into slices
  auto delta_width = circ / static_cast<float>(slices);
  // Iterate through each stack
  for (unsigned int i = 0; i < stacks; ++i) {
    // Iterate through each slice
    for (unsigned int j = 0; j < slices; ++j) {
      // Caculate vertices
      verts[0] = glm::vec3(cos(j * delta_angle), 1.0f - (delta_height * i), sin(j * delta_angle));
      verts[1] = glm::vec3(cos((j + 1) * delta_angle), 1.0f - (delta_height * i), sin((j + 1) * delta_angle));
      verts[2] = glm::vec3(cos(j * delta_angle), 1.0f - (delta_height * (i + 1)), sin(j * delta_angle));
      verts[3] = glm::vec3(cos((j + 1) * delta_angle), 1.0f - (delta_height * (i + 1)), sin((j + 1) * delta_angle));
      // Scale by 0.5 * dims
      for (auto &v : verts)
        v *= dims * 0.5f;
      // Recalculate minimal and maximal
      for (auto &v : verts) {
        minimal = glm::min(minimal, v);
        maximal = glm::max(maximal, v);
      }

      // Calculate texture coordinates
      coords[0] = glm::vec2((-delta_width * j) / glm::pi<float>(), dims.y - ((delta_height * i * dims.y) / 2.0f));
      coords[1] = glm::vec2((-delta_width * (j + 1)) / glm::pi<float>(), dims.y - ((delta_height * i * dims.y) / 2.0f));
      coords[2] = glm::vec2((-delta_width * j) / glm::pi<float>(), dims.y - ((delta_height * (i + 1) * dims.y) / 2.0f));
      coords[3] =
          glm::vec2((-delta_width * (j + 1)) / glm::pi<float>(), dims.y - ((delta_height * (i + 1) * dims.y) / 2.0f));

      // Triangle 1
      positions.push_back(verts[0]);
      normals.push_back(glm::normalize(glm::vec3(verts[0].x, 0.0f, verts[0].z)));
      tex_coords.push_back(coords[0]);
      positions.push_back(verts[3]);
      normals.push_back(glm::normalize(glm::vec3(verts[3].x, 0.0f, verts[3].z)));
      tex_coords.push_back(coords[3]);
      positions.push_back(verts[2]);
      normals.push_back(glm::normalize(glm::vec3(verts[2].x, 0.0f, verts[2].z)));
      tex_coords.push_back(coords[2]);
      // Triangle 2
      positions.push_back(verts[0]);
      normals.push_back(glm::normalize(glm::vec3(verts[0].x, 0.0f, verts[0].z)));
      tex_coords.push_back(coords[0]);
      positions.push_back(verts[1]);
      normals.push_back(glm::normalize(glm::vec3(verts[1].x, 0.0f, verts[1].z)));
      tex_coords.push_back(coords[1]);
      positions.push_back(verts[3]);
      normals.push_back(glm::normalize(glm::vec3(verts[3].x, 0.0f, verts[3].z)));
      tex_coords.push_back(coords[3]);

      // Colours
      for (unsigned int k = 0; k < 6; ++k)
        colours.push_back(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
    }
  }

  // Set minimal and maximal values
  geom.set_minimal_point(minimal);
  geom.set_maximal_point(maximal);

  // Add buffers to geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
  geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);
  geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);

  // Generate tangent and binormal data
  generate_tb(geom, normals);

  return std::move(geom);
}

// Creates sphere geometry
geometry geometry_builder::create_sphere(const unsigned int stacks, const unsigned int slices, const glm::vec3 &dims) {
  // Type of geometry generated will be triangles
  geometry geom;
  geom.set_type(GL_TRIANGLES);
  // Declare required buffers - positions, normals, texture coordinates and colour
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> tex_coords;
  std::vector<glm::vec4> colours;
  // Minimal and maximal points
  glm::vec3 minimal(0.0f, 0.0f, 0.0f);
  glm::vec3 maximal(0.0f, 0.0f, 0.0f);
  // Working values
  float delta_rho = glm::pi<float>() / static_cast<float>(stacks);
  float delta_theta = 2.0f * glm::pi<float>() / static_cast<float>(slices);
  float delta_T = dims.y / static_cast<float>(stacks);
  float delta_S = dims.x / static_cast<float>(slices);
  float t = dims.y;
  float s = 0.0f;

  // Iterate through each stack
  for (unsigned int i = 0; i < stacks; ++i) {
    // Set starting values for stack
    float rho = i * delta_rho;
    s = 0.0f;
    // Vertex data generated
    std::array<glm::vec3, 4> verts;
    std::array<glm::vec2, 4> coords;
    // Iterate through each slice
    for (unsigned int j = 0; j < slices; ++j) {
      // Vertex 0
      float theta = j * delta_theta;
      verts[0] = glm::vec3(dims.x * -sin(theta) * sin(rho), dims.y * cos(theta) * sin(rho), dims.z * cos(rho));
      coords[0] = glm::vec2(s, t);
      // Vertex 1
      verts[1] = glm::vec3(dims.x * -sin(theta) * sin(rho + delta_rho), dims.y * cos(theta) * sin(rho + delta_rho),
                           dims.z * cos(rho + delta_rho));
      coords[1] = glm::vec2(s, t - delta_T);
      // Vertex 2
      theta = ((j + 1) == slices) ? 0.0f : (j + 1) * delta_theta;
      s += delta_S;
      verts[2] = glm::vec3(dims.x * -sin(theta) * sin(rho), dims.y * cos(theta) * sin(rho), dims.z * cos(rho));
      coords[2] = glm::vec2(s, t);
      // Vertex 3
      verts[3] = glm::vec3(dims.x * -sin(theta) * sin(rho + delta_rho), dims.y * cos(theta) * sin(rho + delta_rho),
                           dims.z * cos(rho + delta_rho));
      coords[3] = glm::vec2(s, t - delta_T);

      // Recalculate minimal and maximal
      for (auto &v : verts) {
        minimal = glm::min(minimal, v);
        maximal = glm::max(maximal, v);
      }

      // Triangle 1
      positions.push_back(verts[0]);
      normals.push_back(glm::normalize(verts[0]));
      tex_coords.push_back(coords[0]);
      positions.push_back(verts[1]);
      normals.push_back(glm::normalize(verts[1]));
      tex_coords.push_back(coords[1]);
      positions.push_back(verts[2]);
      normals.push_back(glm::normalize(verts[2]));
      tex_coords.push_back(coords[2]);

      // Triangle 2
      positions.push_back(verts[1]);
      normals.push_back(glm::normalize(verts[1]));
      tex_coords.push_back(coords[1]);
      positions.push_back(verts[3]);
      normals.push_back(glm::normalize(verts[3]));
      tex_coords.push_back(coords[3]);
      positions.push_back(verts[2]);
      normals.push_back(glm::normalize(verts[2]));
      tex_coords.push_back(coords[2]);
    }
    t -= delta_T;
  }

  // Add minimal and maximal points
  geom.set_minimal_point(minimal);
  geom.set_maximal_point(maximal);

  // Add colour data
  for (auto &v : positions)
    colours.push_back(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));

  // Add buffers to geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
  geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);
  geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);

  // Generate tangent and binormal data
  generate_tb(geom, normals);

  return std::move(geom);
}

// Creates torus geometry
geometry geometry_builder::create_torus(const unsigned int stacks, const unsigned int slices, const float ring_radius,
                                        const float outer_radius) {
  // Type of geometry generated will be triangles
  geometry geom;
  geom.set_type(GL_TRIANGLES);
  // Declare required buffers - positions, normals, texture coordinates and colour
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> tex_coords;
  std::vector<glm::vec4> colours;

  // The minimal and maximal points
  glm::vec3 minimal(0.0f, 0.0f, 0.0f);
  glm::vec3 maximal(0.0f, 0.0f, 0.0f);

  // Calculate delta for stack and slice
  auto delta_stack = 2.0f * glm::pi<float>() / static_cast<float>(stacks);
  auto delta_slice = 2.0f * glm::pi<float>() / static_cast<float>(slices);

  // Calculate outer circumference
  auto outer_circ = 2.0f * glm::pi<float>() * outer_radius;
  auto ring_circ = 2.0f * glm::pi<float>() * ring_radius;

  // Create stacks for torus
  for (unsigned int i = 0; i < stacks; ++i) {
    // Working values for stack
    auto a0 = i * delta_stack;
    auto a1 = a0 + delta_stack;
    std::array<glm::vec3, 4> verts;
    std::array<glm::vec3, 4> norms;
    std::array<glm::vec2, 4> coords;

    // Iterate through each slice for stack
    for (unsigned int j = 0; j <= slices; ++j) {
      // Working values for slice
      auto b = j * delta_slice;
      auto c = cos(j * delta_slice) * ring_radius;
      auto r = c + outer_radius;

      // Vertex 0
      verts[0] = glm::vec3(sin(a0) * r, sin(j * delta_slice) * ring_radius, cos(a0) * r);
      norms[0] = glm::normalize(glm::vec3(sin(a0) * c, cos(a0) * c, sin(b)));
      coords[0] = glm::vec2((static_cast<float>(i) / static_cast<float>(stacks)) * outer_circ / glm::pi<float>(),
                            (static_cast<float>(j) / static_cast<float>(slices)) * ring_circ / glm::pi<float>());
      // Vertex 1
      verts[1] = glm::vec3(sin(a1) * r, sin(j * delta_slice) * ring_radius, cos(a1) * r);
      norms[1] = glm::normalize(glm::vec3(sin(a1) * c, cos(a1) * c, sin(b)));
      coords[1] = glm::vec2((static_cast<float>(i + 1) / static_cast<float>(stacks)) * outer_circ / glm::pi<float>(),
                            (static_cast<float>(j) / static_cast<float>(slices)) * ring_circ / glm::pi<float>());
      // Vertex 2
      c = cos((j + 1) * delta_slice) * ring_radius;
      r = c + outer_radius;
      verts[2] = glm::vec3(sin(a0) * r, sin((j + 1) * delta_slice) * ring_radius, cos(a0) * r);
      norms[2] = glm::normalize(glm::vec3(sin(a0) * c, cos(a0) * c, sin(b)));
      coords[2] = glm::vec2((static_cast<float>(i) / static_cast<float>(stacks)) * outer_circ / glm::pi<float>(),
                            (static_cast<float>(j + 1) / static_cast<float>(slices)) * ring_circ / glm::pi<float>());
      // Vertex 3
      verts[3] = glm::vec3(sin(a1) * r, sin((j + 1) * delta_slice) * ring_radius, cos(a1) * r);
      norms[3] = glm::normalize(glm::vec3(sin(a1) * c, cos(a1) * c, sin(b)));
      coords[3] = glm::vec2((static_cast<float>(i + 1) / static_cast<float>(stacks)) * outer_circ / glm::pi<float>(),
                            (static_cast<float>(j + 1) / static_cast<float>(slices)) * ring_circ / glm::pi<float>());

      // Recalculate minimal and maximal
      for (auto &v : verts) {
        minimal = glm::min(minimal, v);
        maximal = glm::max(maximal, v);
      }

      // Triangle 1
      positions.push_back(verts[0]);
      positions.push_back(verts[1]);
      positions.push_back(verts[2]);
      normals.push_back(norms[0]);
      normals.push_back(norms[1]);
      normals.push_back(norms[2]);
      tex_coords.push_back(coords[0]);
      tex_coords.push_back(coords[1]);
      tex_coords.push_back(coords[2]);

      // Triangle 2
      positions.push_back(verts[1]);
      positions.push_back(verts[3]);
      positions.push_back(verts[2]);
      normals.push_back(norms[1]);
      normals.push_back(norms[3]);
      normals.push_back(norms[2]);
      tex_coords.push_back(coords[1]);
      tex_coords.push_back(coords[3]);
      tex_coords.push_back(coords[2]);

      // Push colours
      for (unsigned int k = 0; k < 6; ++k)
        colours.push_back(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
    }
  }

  // Set minimal and maximal values
  geom.set_minimal_point(minimal);
  geom.set_maximal_point(maximal);

  // Add buffers to geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
  geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);
  geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);

  // Generate tangent and binormal data
  generate_tb(geom, normals);

  return std::move(geom);
}

// Create plane geometry
geometry geometry_builder::create_plane(const unsigned int width, const unsigned int depth) {
  // Type of geometry generated will be triangles
  geometry geom;
  geom.set_type(GL_TRIANGLES);
  // Declare required buffers - positions, normals, texture coordinates and colour
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> tex_coords;
  std::vector<glm::vec4> colours;

  // Minimal and maximal points
  glm::vec3 minimal(0.0f, 0.0f, 0.0f);
  glm::vec3 maximal(0.0f, 0.0f, 0.0f);

  // Iterate through each vertex and add to geometry
  std::array<glm::vec3, 4> verts;
  for (unsigned int x = 0; x < width; ++x) {
    for (unsigned int z = 0; z < depth; ++z) {
      // Calculate vertex positions
      verts[0] = glm::vec3(-static_cast<float>(width) / 2.0f + x, 0.0f, static_cast<float>(depth) / 2.0f - z);
      verts[1] = glm::vec3(-static_cast<float>(width) / 2.0f + (x + 1), 0.0f, static_cast<float>(depth) / 2.0f - z);
      verts[2] = glm::vec3(-static_cast<float>(width) / 2.0f + x, 0.0f, static_cast<float>(depth) / 2.0f - (z + 1));
      verts[3] =
          glm::vec3(-static_cast<float>(width) / 2.0f + (x + 1), 0.0f, static_cast<float>(depth) / 2.0f - (z + 1));

      // Recalculate minimal and maximal
      for (auto &v : verts) {
        minimal = glm::min(minimal, v);
        maximal = glm::max(maximal, v);
      }

      // Triangle 1
      positions.push_back(verts[0]);
      tex_coords.push_back(glm::vec2(x, z) / 10.0f);
      positions.push_back(verts[3]);
      tex_coords.push_back(glm::vec2(x + 1, z + 1) / 10.0f);
      positions.push_back(verts[2]);
      tex_coords.push_back(glm::vec2(x, z + 1) / 10.0f);
      // Triangle 2
      positions.push_back(verts[0]);
      tex_coords.push_back(glm::vec2(x, z) / 10.0f);
      positions.push_back(verts[1]);
      tex_coords.push_back(glm::vec2(x + 1, z) / 10.0f);
      positions.push_back(verts[3]);
      tex_coords.push_back(glm::vec2(x + 1, z + 1) / 10.0f);

      // Add normals and colours
      for (unsigned int i = 0; i < 6; ++i) {
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        colours.push_back(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
      }
    }
  }

  // Set minimal and maximal values
  geom.set_minimal_point(minimal);
  geom.set_maximal_point(maximal);

  // Add buffers to geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
  geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);
  geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);

  // Generate tangent and binormal data
  generate_tb(geom, normals);

  return std::move(geom);
}
}
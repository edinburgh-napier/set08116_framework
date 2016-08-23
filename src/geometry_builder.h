#pragma once

#include "geometry.h"
#include "stdafx.h"

namespace graphics_framework {
/*
Utility class to build basic geometry types
*/
class geometry_builder {
public:
  // Creates box geometry
  static geometry create_box(const glm::vec3 &dims = glm::vec3(1.0f, 1.0f, 1.0f));
  // Creates tetrahedron geometry
  static geometry create_tetrahedron(const glm::vec3 &dims = glm::vec3(1.0f, 1.0f, 1.0f));
  // Creates pyramid geometry
  static geometry create_pyramid(const glm::vec3 &dims = glm::vec3(1.0f, 1.0f, 1.0f));
  // Creates disk geometry
  static geometry create_disk(const unsigned int slices = 10, const glm::vec2 &dims = glm::vec2(1.0f, 1.0f));
  // Creates cylinder geometry
  static geometry create_cylinder(const unsigned int stacks = 10, const unsigned int slices = 10,
                                  const glm::vec3 &dims = glm::vec3(1.0f, 1.0f, 1.0f));
  // Creates sphere geometry
  static geometry create_sphere(const unsigned int stacks = 10, const unsigned int slices = 10,
                                const glm::vec3 &dims = glm::vec3(1.0f, 1.0f, 1.0f));
  // Creates torus geometry
  static geometry create_torus(const unsigned int stacks = 10, const unsigned int slices = 10,
                               const float ring_radius = 1.0f, const float outer_radius = 3.0f);
  // Creates plane geometry
  static geometry create_plane(const unsigned int width = 100, const unsigned int depth = 100);
};
}
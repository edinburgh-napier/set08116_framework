#pragma once

#include "geometry.h"
#include "material.h"
#include "stdafx.h"
#include "transform.h"

namespace graphics_framework {
/*
An object combining a transform, geometry and material to create a renderable
mesh object
*/
class mesh {
private:
  // The transform object of the mesh
  transform _transform;
  // The geometry object of the mesh
  geometry _geometry;
  // The material object of the mesh
  material _material;
  // The minimal of the AABB defining the mesh
  glm::vec3 _minimal;
  // The maximal of the AABB defining the mesh
  glm::vec3 _maximal;

public:
  // Creates a mesh object
  mesh() {}
  // Creates a mesh object with the provided geometry
  mesh(geometry &geom) : _geometry(geom), _minimal(geom.get_minimal_point()), _maximal(geom.get_maximal_point()) {}
  // Creates a mesh object with the provided geometry and material
  mesh(geometry &geom, material &mat)
      : _geometry(geom), _minimal(geom.get_minimal_point()), _maximal(geom.get_maximal_point()) {}
  // Default copy constructor and assignment operator
  mesh(const mesh &other) = default;
  mesh &operator=(const mesh &rhs) = default;
  // Destroys the mesh object
  ~mesh() {}
  // Gets the transform object for the mesh
  transform &get_transform() { return _transform; }
  // Gets the geometry object for the mesh
  const geometry &get_geometry() const { return _geometry; }
  // Sets the geometry object for the mesh
  void set_geometry(const geometry &value) { _geometry = value; }
  // Gets the material object for the mesh
  material &get_material() { return _material; }
  // Sets the material object for the mesh
  void set_material(const material &value) { _material = value; }
  // Gets the minimal point of the AABB defining the mesh
  glm::vec3 get_minimal() const { return _minimal * _transform.scale; }
  // Gets the maximal point of the AABB defining the mes
  glm::vec3 get_maximal() const { return _maximal * _transform.scale; }
};
}
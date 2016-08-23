#pragma once

#include "effect.h"
#include "geometry.h"
#include "material.h"
#include "stdafx.h"

namespace graphics_framework {
/*
Represents a terrain mesh
*/
class terrain {
private:
  // The geometry object defining the terrain
  geometry _geom;
  // The material used by the terrain
  material _mat;

public:
  // Creates a terrain object
  terrain() {}
  // Creates a terrain object from the given heightmap file
  terrain(const std::string &heightmap);
  // Default copy constructor and assignment operator
  terrain(const terrain &other) = default;
  terrain &operator=(const terrain &rhs) = default;
  // Destroys the terrain object
  ~terrain() {}
  // Gets the geometry object representing the terrain
  const geometry &get_geometry() const { return _geom; }
  // Gets the material for the terrain
  const material &get_material() const { return _mat; }
  // Sets the material for the terrain
  void set_material(const material &value) { _mat = value; }
};
}
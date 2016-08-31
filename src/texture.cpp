#include "stdafx.h"

#include "texture.h"
#include "util.h"
//#include <FreeImage\FreeImage.h>

namespace graphics_framework {
// Creates a new texture object with the given dimensions
texture::texture(GLuint width, GLuint height) throw(...) : _width(width), _height(height) {
  // Initialise texture with OpenGL
  glGenTextures(1, &_id);
  _type = GL_TEXTURE_2D;
  // Check if error
  if (CHECK_GL_ERROR) {
    // Problem creating texture object
    std::cerr << "ERROR - creating texture" << std::endl;
    std::cerr << "Could not allocate texture with OpenGL" << std::endl;
    // Set id to 0
    _id = 0;
    // Throw exception
    throw std::runtime_error("Error creating texture");
  }
}

// Creates a new texture object from the given file
texture::texture(const std::string &filename) throw(...) : texture(filename, true, true) {}

// Creates a new texture object from the given file with mipmaps and anisotropic filtering defined
texture::texture(const std::string &filename, bool mipmaps, bool anisotropic) throw(...) {
  // Check if file exists
  assert(check_file_exists(filename));
  /*
  // Get format of image
  auto format = FreeImage_GetFileType(filename.c_str());
  // Load image data
  auto image = FreeImage_Load(format, filename.c_str(), 0);
  // Convert image to 32bit format
  auto temp = image;
  image = FreeImage_ConvertTo32Bits(image);
  FreeImage_Unload(temp);

  // Get image details
  auto width = FreeImage_GetWidth(image);
  auto height = FreeImage_GetHeight(image);

  // Get pixel data
  auto pixel_data = FreeImage_GetBits(image);
  */
  // Generate texture with OpenGL
  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);
  // Check for any errors with OpenGL
  if (CHECK_GL_ERROR) {
    // Problem creating texture object
    std::cerr << "ERROR - loading texture " << filename << std::endl;
    std::cerr << "Could not allocate texture with OpenGL" << std::endl;
    // Unload FreeImage data
    // FreeImage_Unload(image);
    // Set id to 0
    _id = 0;
    // Throw exception
    throw std::runtime_error("Error creating texture");
  }

  // Set texture parameters
  if (mipmaps) {
    // Turn on linear mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    CHECK_GL_ERROR; // Not considered fatal here
  } else {
    // Basic scaling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    CHECK_GL_ERROR; // Not considered fatal here
  }
  if (anisotropic) {
    // Turn on anisotropic filtering
    float max_anisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
    CHECK_GL_ERROR; // Non-fatal
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);
    CHECK_GL_ERROR; // Non-fatal
  }

  int width = 256;
  int height = 256;
  std::array<unsigned char, (256 * 256 * 4)> pixel_data;
  for (int i = 0; i < (256 * 256 * 4); i++) {
    if (i > 0 && (i % 3 == 0)) {
      continue;
    }
    pixel_data[i] = (unsigned char)rand();
  }

  // Now set texture data
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, &pixel_data[0]);

  // Check if error
  if (CHECK_GL_ERROR) {
    // Error loading texture data into OpenGL
    std::cerr << "ERROR - loading texture " << filename << std::endl;
    std::cerr << "Could not load texture data in OpenGL" << std::endl;
    // Unload FreeImage data
    // FreeImage_Unload(image);
    // Unallocate image with OpenGL
    glDeleteTextures(1, &_id);
    _id = 0;
    // Throw exception
    throw std::runtime_error("Error creating texture");
  }

  // Generate mipmaps
  if (mipmaps)
    glGenerateMipmap(GL_TEXTURE_2D);

  // Set attributes
  _height = height;
  _width = width;
  _type = GL_TEXTURE_2D;

  // Unload image data
  // FreeImage_Unload(image);
  CHECK_GL_ERROR; // Non-fatal - just info

  // Log
  std::clog << "LOG - texture " << filename << " loaded" << std::endl;
}

// Creates a new texture from the given colour data
texture::texture(const std::vector<glm::vec4> &data, GLuint width, GLuint height) throw(...)
    : texture(data, width, height, true, true) {}

// Creates a new texture from the given colour data and mipmap and anisotropic filtering defined
texture::texture(const std::vector<glm::vec4> &data, GLuint width, GLuint height, bool mipmaps,
                 bool anisotropic) throw(...) {
  // Check if dimensions are correct
  assert(data.size() == width * height);

  // Generate texture with OpenGL
  glGenTextures(1, &_id);
  // Check error
  if (CHECK_GL_ERROR) {
    // Display error
    std::cerr << "ERROR - building texture" << std::endl;
    std::cerr << "Could not allocate texture with OpenGL" << std::endl;
    // Throw exception
    throw std::runtime_error("Error creating texture");
  }

  // Determine type of texture by height
  if (height == 1) {
    // 1D texture
    glBindTexture(GL_TEXTURE_1D, _id);
    // Set parameters
    if (mipmaps) {
      // Set mipmap scaling
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
      // Normal scaling
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    if (anisotropic) {
      // Turn on anisotropic filtering
      float max_anisotropy;
      glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
      glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);
    }
    CHECK_GL_ERROR; // Non-fatal

    // Add texture data
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, width, 0, GL_RGBA, GL_FLOAT, &data[0]);
    // Check error
    if (CHECK_GL_ERROR) {
      // Display error
      std::cerr << "ERROR - building texture" << std::endl;
      std::cerr << "Could not allocate image data with OpenGL" << std::endl;
      // Delete texture
      glDeleteTextures(1, &_id);
      _id = 0;
      // Throw exception
      throw std::runtime_error("Error creating texture");
    }
    // Generate mipmaps
    if (mipmaps)
      glGenerateMipmap(GL_TEXTURE_1D);
  } else {
    // 2D texture
    glBindTexture(GL_TEXTURE_2D, _id);
    // Set parameters
    if (mipmaps) {
      // Set mipmap scaling
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
      // Normal scaling
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    if (anisotropic) {
      // Turn on anisotropic filtering
      float max_anisotropy;
      glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);
    }
    CHECK_GL_ERROR; // Non-fatal

    // Add texture data
    glTexImage1D(GL_TEXTURE_2D, 0, GL_RGBA, width, 0, GL_RGBA, GL_FLOAT, &data[0]);
    // Check error
    if (CHECK_GL_ERROR) {
      // Display error
      std::cerr << "ERROR - building texture" << std::endl;
      std::cerr << "Could not allocate image data with OpenGL" << std::endl;
      // Delete texture
      glDeleteTextures(1, &_id);
      _id = 0;
      // Throw exception
      throw std::runtime_error("Error creating texture");
    }
    // Generate mipmaps
    if (mipmaps)
      glGenerateMipmap(GL_TEXTURE_2D);
  }

  // Set parameters
  _height = height;
  _width = width;
  if (height == 1)
    _type = GL_TEXTURE_1D;
  else
    _type = GL_TEXTURE_2D;

  // Log
  std::clog << "LOG - texture built" << std::endl;
}
}
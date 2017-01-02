#include "stdafx.h"

#include "texture.h"
#include "util.h"
#include <IL/il.h>
#include <IL/ilu.h>

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

  ILuint ImgId = -1;
  // Generate the main image name to use.
  ilGenImages(1, &ImgId);

  // Bind this image name.
  ilBindImage(ImgId);

  auto success = ilLoadImage(filename.c_str());

  if (get_devil_error()) {
    throw std::runtime_error("Error creating texture");
  }

  {
    ILinfo ImageInfo;
    iluGetImageInfo(&ImageInfo);
    if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) {
      iluFlipImage();
    }
  }

  // Convert the image into a suitable format to work with
  success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
  const auto width = ilGetInteger(IL_IMAGE_WIDTH);
  const auto height = ilGetInteger(IL_IMAGE_HEIGHT);
  auto pixel_data = ilGetData();

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

  // Now set texture data
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, &pixel_data[0]);
  // clear out ddata from DevIL
  ilDeleteImage(ImgId);
  pixel_data = nullptr;

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
  std::clog << "LOG - texture " << filename << " loaded, " << width << 'x' << height << std::endl;
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
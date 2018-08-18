#include "stdafx.h"

#include "texture.h"
#include "util.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace graphics_framework {

  static auto stbi_load_smart = [](auto... args) {
  auto closer_lambda = [](stbi_uc *rgbd) { stbi_image_free(rgbd); };
  std::unique_ptr<stbi_uc, decltype(closer_lambda)> rgb(stbi_load(args...),
                                                        closer_lambda);
  return rgb;
};

// Creates a new texture object with the given dimensions
texture::texture(GLuint width, GLuint height) throw(...)
    : _width(width), _height(height) {
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
texture::texture(const std::string &filename) throw(...)
    : texture(filename, true, true) {}

// Creates a new texture object from the given file with mipmaps and anisotropic
// filtering defined
texture::texture(const std::string &filename, bool mipmaps,
                 bool anisotropic) throw(...) {
  // Check if file exists
  if (!check_file_exists(filename)) {
    // Failed to read file.  Display error
    std::cerr << "ERROR - could not load texture " << filename << std::endl;
    std::cerr << "File Does Not Exist" << std::endl;
    // Throw exception
    throw std::runtime_error("Error reading texture");
  }

  int width, height, bpp;
  auto rgb = stbi_load_smart(filename.c_str(), &width, &height, &bpp, 4);

  if (rgb == NULL || width == 0 || height == 0) {
    throw std::runtime_error("Error reading texture");
  }

  // Generate texture with OpenGL
  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);

  // Check for any errors with OpenGL
  if (CHECK_GL_ERROR) {
    _id = 0;
    // Problem creating texture object
    std::cerr << "ERROR - creating texture " << filename << std::endl;
    std::cerr << "Could not allocate texture with OpenGL" << std::endl;
    // Throw exception
    throw std::runtime_error("Error creating texture");
  }

  // Set texture parameters
  if (mipmaps) {
    // Turn on linear mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
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
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                    max_anisotropy);
    CHECK_GL_ERROR; // Non-fatal
  }

  // Now set texture data
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, rgb.get());

  // Check if error
  if (CHECK_GL_ERROR) {
    // Error loading texture data into OpenGL
    std::cerr << "ERROR - loading texture " << filename << std::endl;
    std::cerr << "Could not load texture data in OpenGL" << std::endl;
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
  std::clog << "LOG - texture " << filename << " loaded, " << width << 'x'
            << height << std::endl;
}

texture::texture(const std::vector<std::string> &filenames,
                 bool anisotropic) throw(...) {
  if (filenames.size() < 2) {
    throw std::runtime_error(
        "Use The standard Texture fucniton if you don't have any mip levels!");
  }
  // Generate texture with OpenGL
  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5);

  if (anisotropic) {
    // Turn on anisotropic filtering
    float max_anisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
    CHECK_GL_ERROR; // Non-fatal
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                    max_anisotropy);
    CHECK_GL_ERROR; // Non-fatal
  }

  // Check for any errors with OpenGL
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - loading mipped texture " << filenames[0] << std::endl;
    std::cerr << "Could not allocate texture with OpenGL" << std::endl;
    throw std::runtime_error("Error creating texture");
  }

  for (size_t i = 0; i < filenames.size(); i++) {
    if (!check_file_exists(filenames[i])) {
      // Failed to read file.  Display error
      std::cerr << "ERROR - could not load texture " << filenames[i]
                << std::endl;
      std::cerr << "File Does Not Exist" << std::endl;
      // Throw exception
      throw std::runtime_error("Error adding texture");
    }

    int width, height, bpp;
    auto rgb = stbi_load_smart(filenames[i].c_str(), &width, &height, &bpp, 4);

    glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, rgb.get());
  }

  // Check error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - loading mipped texture " << filenames[0] << std::endl;
    std::cerr << "Could not allocate texture with OpenGL" << std::endl;
    throw std::runtime_error("Error creating texture");
  }

  // Set attributes
  _type = GL_TEXTURE_2D;

  CHECK_GL_ERROR; // Non-fatal - just info

  // Log
  std::clog << "LOG - texture With Mips " << filenames[0] << " loaded"
            << std::endl;
}

// Creates a new texture from the given colour data
texture::texture(const std::vector<glm::vec4> &data, GLuint width,
                 GLuint height) throw(...)
    : texture(data, width, height, true, true) {}

// Creates a new texture from the given colour data and mipmap and anisotropic
// filtering defined
texture::texture(const std::vector<glm::vec4> &data, GLuint width,
                 GLuint height, bool mipmaps, bool anisotropic) throw(...) {
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
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
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
      glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                      max_anisotropy);
    }
    CHECK_GL_ERROR; // Non-fatal

    // Add texture data
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, width, 0, GL_RGBA, GL_FLOAT,
                 &data[0]);
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
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
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
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                      max_anisotropy);
    }
    CHECK_GL_ERROR; // Non-fatal

    // Add texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT,
                 &data[0]);
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
} // namespace graphics_framework
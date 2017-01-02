#include "stdafx.h"

#include "cubemap.h"
#include "util.h"
//#include <FreeImage\FreeImage.h>

namespace graphics_framework {
// The 6 targets of the the cubemap
std::array<GLenum, 6> targets = {GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                                 GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                                 GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

// Creates a cubemap object from an array of six file names
cubemap::cubemap(const std::array<std::string, 6> &filenames) throw(...) {
  // Ensure that filenames are valid
  for (auto &file : filenames)
    assert(check_file_exists(file));
  // Generate cubemap texture and bind
  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
  // Check if OpenGL error.
  if (CHECK_GL_ERROR) {
    // Display error
    std::cerr << "ERROR - creating cubemap" << std::endl;
    std::cerr << "Could not allocate texture with OpenGL" << std::endl;
    // Set ID to 0
    _id = 0;
    // Throw exception
    throw std::runtime_error("Error creating cubemap texture with OpenGL");
  }
  // Load in images using FreeImage
  /*std::array<FIBITMAP*, 6> images;

  for (auto i = 0; i < 6; ++i)
  {
          // Get the image format
          auto format = FreeImage_GetFileType(filenames[i].c_str());
          // Load the image
          images[i] = FreeImage_Load(format, filenames[i].c_str(), 0);
          // Store a temporary version
          auto temp = images[i];
          // Convert to a 32 bit image
          images[i] = FreeImage_ConvertTo32Bits(images[i]);
          // Unload temporary (non-converted) image
          FreeImage_Unload(temp);
// Set temp to converted image
temp = images[i];
// Rotate image - OpenGL is a bit silly here
images[i] = FreeImage_Rotate(images[i], 180.0f);
// Unload temporary (non-rotated) image
FreeImage_Unload(temp);
  }
  */

  int w = 256;
  int h = 256;
  std::array<unsigned char, (256 * 256 * 4)> img;
  for (int i = 0; i < (256 * 256 * 4); i++) {
    if (i > 0 && (i % 3 == 0)) {
      continue;
    }
    img[i] = (unsigned char)rand();
  }

  // Set magnification and minification filters
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  CHECK_GL_ERROR; // non-fatal
  // Set the anisotropic filtering at max
  float max_anisotropy;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);
  CHECK_GL_ERROR; // non-fatal
  // Load in each image to OpenGL and assign it to the cubemap texture
  for (auto i = 0; i < 6; ++i) {
    // Load the image into OpenGL
    glTexImage2D(targets[i], 0, GL_RGBA,
                 w, // FreeImage_GetWidth(images[i]),
                 h, // FreeImage_GetHeight(images[i]),
                 0, GL_BGRA, GL_UNSIGNED_BYTE, &img[0]);

    // Check if loaded OK
    if (CHECK_GL_ERROR) {
      // Display error
      std::cerr << "ERROR - loading cubemap textures" << std::endl;
      std::cerr << "Could not load texture data for file " << filenames[i] << std::endl;
      // Unload the FreeImage images
      for (auto i = 0; i < 6; ++i) {
        // FreeImage_Unload(images[i]);
      }
      // Delete the texture
      glDeleteTextures(1, &_id);
      _id = 0;
      // Throw an exception
      throw std::runtime_error("Error loading cubemap textures");
    }
  }
  // Generate the mipmaps
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  CHECK_GL_ERROR; // non-fatal
  // Unload the FreeImage images
  for (auto i = 0; i < 6; ++i) {
    //	FreeImage_Unload(images[i]);
  }
  // Log success
  std::clog << "LOG - cubemap created" << std::endl;
}

// Sets one of the textures in the cubemap
bool cubemap::set_texture(GLenum target, const std::string &filename) throw(...) {
  // Check that cubemap has been generated
  if (_id == 0) {
    // Generate texture with OpenGL
    glGenTextures(1, &_id);
    // Check error
    if (CHECK_GL_ERROR) {
      // Display error
      std::cerr << "ERROR - creating cubemap" << std::endl;
      std::cerr << "Could not allocate texture with OpenGL" << std::endl;
      // Set ID to 0
      _id = 0;
      // Throw exception
      throw std::runtime_error("Error creating cubemap texture with OpenGL");
    }
  }

  // Check that target is valid
  assert(std::find(std::begin(targets), std::end(targets), target) != std::end(targets));
  // Check that filename is valid
  assert(check_file_exists(filename));

  // Bind the cubemap texture
  glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
  // Check if OpenGL error
  if (CHECK_GL_ERROR) {
    // Display error
    std::cerr << "ERROR - adding a texture to cubemap" << std::endl;
    std::cerr << "Could not bind cubemap" << std::endl;
    // Throw exception
    throw std::runtime_error("Error binding cubemap");
  }
  //TODO
  // Get image format
  // auto format = FreeImage_GetFileType(filename.c_str());
  // Load the image
  // auto image = FreeImage_Load(format, filename.c_str(), 0);
  // Store temporary copy of image
  // auto temp = image;
  // Convert image to 32bits
  // image = FreeImage_ConvertTo32Bits(image);
  // Unload temporary (not converted) image
  // FreeImage_Unload(temp);
  // Set temp to converted image
  // temp = image;
  // Rotate image - OpenGL is a bit silly here
  // image = FreeImage_Rotate(image, 180.0f);
  // Unload temporary (non-rotated) image
  // FreeImage_Unload(temp);
  // Load the texture into OpenGL

  int w = 256;
  int h = 256;
  std::array<unsigned char, (256 * 256 * 4)> img;
  for (int i = 0; i < (256 * 256 * 4); i++) {
    if (i > 0 && (i % 3 == 0)) {
      continue;
    }
    img[i] = (unsigned char)rand();
  }
  /*
  glTexImage2D(target, 0, GL_RGBA, FreeImage_GetWidth(image),
               FreeImage_GetHeight(image), 0, GL_BGRA, GL_UNSIGNED_BYTE,
               FreeImage_GetBits(image));
                           */

  glTexImage2D(target, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, &img[0]);

  // Check if error
  if (CHECK_GL_ERROR) {
    // Display error
    std::cerr << "ERROR - adding a texture to cubemap" << std::endl;
    std::cerr << "Could not load texture data for file " << filename << std::endl;
    // Unload FreeImage image
    // FreeImage_Unload(image);
    // Throw exception
    throw std::runtime_error("Error loading texture");
  }
  // Generate mipmaps
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  CHECK_GL_ERROR; // Non-fatal
  // Unload OpenGL image
  // FreeImage_Unload(image);
  // Log and return
  std::clog << "LOG - texture added to cubemap" << std::endl;
  return true;
}
}
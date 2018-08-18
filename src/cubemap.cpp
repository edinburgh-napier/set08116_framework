#include "stdafx.h"

#include "cubemap.h"
#include "stb_image.h"
#include "util.h"

namespace graphics_framework {

static auto stbi_load_smart = [](auto... args) {
  auto closer_lambda = [](stbi_uc *rgbd) { stbi_image_free(rgbd); };
  std::unique_ptr<stbi_uc, decltype(closer_lambda)> rgb(stbi_load(args...),
                                                        closer_lambda);
  return rgb;
};

// The 6 targets of the the cubemap
std::array<GLenum, 6> targets = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

// Creates a cubemap object from an array of six file names
cubemap::cubemap(const std::array<std::string, 6> &filenames) throw(...) {
  // Ensure that filenames are valid
  for (auto &file : filenames) {
    if (!check_file_exists(file)) {
      // Failed to read file.  Display error
      std::cerr << "ERROR - could not load cubemap texture " << file
                << std::endl;
      std::cerr << "File Does Not Exist" << std::endl;
      // Throw exception
      throw std::runtime_error("Error adding cubemap texture");
    }
  }
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

  // Set magnification and minification filters
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  CHECK_GL_ERROR; // non-fatal
  // Set the anisotropic filtering at max
  float max_anisotropy;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                  max_anisotropy);
  CHECK_GL_ERROR; // non-fatal
  // Load in each image to OpenGL and assign it to the cubemap texture

  for (auto i = 0; i < 6; ++i) {
    // Todo Refactor this to a common image place
    int width, height, bpp;
    auto rgb = stbi_load_smart(filenames[i].c_str(), &width, &height, &bpp, 4);

    if (rgb == NULL || width == 0 || height == 0) {
      throw std::runtime_error("Error reading texture");
    }

    // Load the image into OpenGL
    glTexImage2D(targets[i], 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, rgb.get());

    // Check if loaded OK
    if (CHECK_GL_ERROR) {
      // Display error
      std::cerr << "ERROR - loading cubemap textures" << std::endl;
      std::cerr << "Could not load texture data for file " << filenames[i]
                << std::endl;
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

  // Log success
  std::clog << "LOG - cubemap created" << std::endl;
}

// Sets one of the textures in the cubemap
bool cubemap::set_texture(GLenum target,
                          const std::string &filename) throw(...) {
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
  assert(std::find(std::begin(targets), std::end(targets), target) !=
         std::end(targets));
  // Check that filename is valid

  if (!check_file_exists(filename)) {
    // Failed to read file.  Display error
    std::cerr << "ERROR - could not load cubemap texture " << filename
              << std::endl;
    std::cerr << "File Does Not Exist" << std::endl;
    // Throw exception
    throw std::runtime_error("Error adding cubemap texture");
  }

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

  int width, height, bpp;
  auto rgb = stbi_load_smart(filename.c_str(), &width, &height, &bpp, 4);

  if (rgb == NULL || width == 0 || height == 0) {
    throw std::runtime_error("Error reading texture");
  }

  // Load the image into OpenGL
  glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               rgb.get());

  // Check if error
  if (CHECK_GL_ERROR) {
    // Display error
    std::cerr << "ERROR - adding a texture to cubemap" << std::endl;
    std::cerr << "Could not load texture data for file " << filename
              << std::endl;
    // Throw exception
    throw std::runtime_error("Error loading texture");
  }
  // Generate mipmaps
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  CHECK_GL_ERROR; // Non-fatal

  // Log and return
  std::clog << "LOG - texture added to cubemap" << std::endl;
  return true;
}
} // namespace graphics_framework
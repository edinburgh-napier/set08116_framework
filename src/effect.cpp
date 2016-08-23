#include "stdafx.h"

#include "effect.h"
#include "util.h"

namespace graphics_framework {
// Helper function used to read in a file
bool read_file(const std::string &filename, std::string &content) {
  // Create filestream
  std::ifstream file(filename, std::ios_base::in);
  // Check that file exists.  If not, return false
  if (file.bad())
    return false;

  // File is good.  Read contents
  std::stringstream buffer;
  buffer << file.rdbuf();

  // Get contents from the file
  content = buffer.str();

  // Close file and return true
  file.close();
  return true;
}

// Creates an effect object
effect::effect() {}

// Adds a shader to the effect object
void effect::add_shader(const std::string &filename, GLenum type) throw(...) {
  // Check file exists
  assert(check_file_exists(filename));
  // String holding the contents of the shader file
  std::string content;
  // Read in contents - check if file read is OK
  if (!read_file(filename, content)) {
    // Failed to read file.  Display error
    std::cerr << "ERROR - could not load shader " << filename << std::endl;
    std::cerr << "Could not read shader file" << std::endl;
    // Throw exception
    throw std::runtime_error("Error adding shader to effect");
  }
  // Create shader with OpenGL
  auto id = glCreateShader(type);
  // Check if error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - loading shader " << filename << std::endl;
    std::cerr << "Could not create shader object with OpenGL" << std::endl;
    // Throw exception
    throw std::runtime_error("Error adding shader to effect");
  }
  // Get shader source and attach to shader object
  auto source = content.c_str();
  glShaderSource(id, 1, &source, 0);
  // Compile
  glCompileShader(id);
  // Check if error
  if (CHECK_GL_ERROR) {
    // Display error
    std::cerr << "ERROR - could not load shader " << filename << std::endl;
    std::cerr << "Problem attaching and compiling source" << std::endl;
    // Throw exception
    throw std::runtime_error("Error adding shader to effect");
  }
  // We have tried to compile the shader.  Now check if compilation was
  // successful.
  // Get compile status
  GLint compiled;
  glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
  CHECK_GL_ERROR; // Not considered fatal here
  // Check if compiled
  if (!compiled) {
    // Shader not compiled.  Get log and display
    // Get the length of the shader compile log
    GLsizei length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    // Create vector to store log file
    std::vector<char> log(length);
    // Get the log
    glGetShaderInfoLog(id, length, &length, &log[0]);
    // Display error
    std::cerr << "ERROR - could not load shader " << filename << std::endl;
    std::cerr << "Could not compile shader file" << std::endl;
    std::cerr << &log[0] << std::endl;
    CHECK_GL_ERROR; // Not considered fatal here
    // Remove shader object from OpenGL
    glDeleteShader(id);
    // Throw exception
    throw std::runtime_error("Error adding shader to effect");
  }

  // Log and add to the effect shaders
  std::clog << "LOG - " << filename << " added to effect" << std::endl;
  _shaders.push_back(id);
}

// Adds a collection of shaders of a given type to the effect
void effect::add_shader(const std::vector<std::string> &filenames, GLenum type) {
  // Check that there is at least one filename
  assert(filenames.size() > 0);
  // Check that each file exists
  for (auto &name : filenames)
    check_file_exists(name);
  // Holds the contents of the file
  std::vector<std::string> file_contents;
  // Read in the file contents
  for (auto &name : filenames) {
    std::string content;
    // Read in contents - check if file read is OK
    if (!read_file(name, content)) {
      // Failed to read file.  Display error
      std::cerr << "ERROR - could not load shader " << name << std::endl;
      std::cerr << "Could not read shader file" << std::endl;
      // Throw exception
      throw std::runtime_error("Error adding shader to effect");
    } else
      // Add to vector
      file_contents.push_back(content);
  }
  // Create shader with OpenGL
  auto id = glCreateShader(type);
  // Check if error
  if (CHECK_GL_ERROR) {
    std::cerr << "ERROR - loading shader:" << std::endl;
    for (auto &name : filenames)
      std::cerr << "\t" << name << std::endl;
    std::cerr << "Could not create shader object with OpenGL" << std::endl;
    // Throw exception
    throw std::runtime_error("Error adding shader to effect");
  }
  // Get shader source and attach to shader object
  const char **source = new const char *[file_contents.size()];
  for (unsigned int i = 0; i < file_contents.size(); ++i)
    source[i] = file_contents[i].c_str();
  // Add source to shader
  glShaderSource(id, static_cast<GLsizei>(file_contents.size()), source, 0);
  // Compile
  glCompileShader(id);
  // Delete the sources
  delete[] source;
  // Check if error
  if (CHECK_GL_ERROR) {
    // Display error
    std::cerr << "ERROR - loading shader:" << std::endl;
    for (auto &name : filenames)
      std::cerr << "\t" << name << std::endl;
    std::cerr << "Problem attaching and compiling source" << std::endl;
    // Throw exception
    throw std::runtime_error("Error adding shader to effect");
  }
  // We have tried to compile the shader.  Now check if compilation was
  // successful.
  // Get compile status
  GLint compiled;
  glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
  CHECK_GL_ERROR; // Not considered fatal here
  // Check if compiled
  if (!compiled) {
    // Shader not compiled.  Get log and display
    // Get the length of the shader compile log
    GLsizei length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    // Create vector to store log file
    std::vector<char> log(length);
    // Get the log
    glGetShaderInfoLog(id, length, &length, &log[0]);
    // Display error
    std::cerr << "ERROR - loading shader:" << std::endl;
    for (auto &name : filenames)
      std::cerr << "\t" << name << std::endl;
    std::cerr << "Could not compile shader file" << std::endl;
    std::cerr << &log[0] << std::endl;
    CHECK_GL_ERROR; // Not considered fatal here
    // Remove shader object from OpenGL
    glDeleteShader(id);
    // Throw exception
    throw std::runtime_error("Error adding shader to effect");
  }

  // Log and add to the effect shaders
  std::clog << "LOG - " << std::endl;
  for (auto &name : filenames)
    std::clog << "\t" << name << std::endl;
  std::clog << " added to effect" << std::endl;
  _shaders.push_back(id);
}

// Builds the effect program
void effect::build() throw(...) {
  // Create program
  _program = glCreateProgram();
  // Check if error
  if (CHECK_GL_ERROR) {
    // Display error
    std::cerr << "ERROR - building effect" << std::endl;
    std::cerr << "Could not create program object with OpenGL" << std::endl;
    // Throw exception
    throw std::runtime_error("Error creating effect with OpenGL");
  }

  for (auto &id : _shaders) {
    // Shader has been loaded.  Add to effect
    glAttachShader(_program, id);
    // Check if error
    if (CHECK_GL_ERROR) {
      // Display error
      std::cerr << "ERROR - adding shader to effect" << std::endl;
      std::cerr << "Problem attaching shader to program" << std::endl;
      // Throw exception
      throw std::runtime_error("Error adding shader to effect");
    }
  }

  // Attempt to link program
  glLinkProgram(_program);
  // Check if error
  if (CHECK_GL_ERROR) {
    // Display error
    std::cerr << "ERROR - building effect " << std::endl;
    std::cerr << "Problem linking program" << std::endl;
    // Detach and delete shaders
    for (auto &s : _shaders) {
      glDetachShader(_program, s);
      glDeleteShader(s);
    }
    // Delete program
    glDeleteProgram(_program);
    // Throw exception
    throw std::runtime_error("Error building effect");
  }

  // Check if linked successfully
  // Get link status
  GLint linked;
  glGetProgramiv(_program, GL_LINK_STATUS, &linked);
  // Check if linked OK
  if (!linked) {
    // Program did not link.  Get info log and display error
    // Get length of info log
    GLsizei length;
    glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &length);
    // Allocate buffer
    std::vector<char> log(length);
    // Get info log
    glGetProgramInfoLog(_program, length, &length, &log[0]);
    // Display error
    std::cerr << "ERROR - building effect" << std::endl;
    std::cerr << "Problem linking program" << std::endl;
    std::cerr << &log[0] << std::endl;
    // Detach shaders
    for (auto &s : _shaders) {
      glDetachShader(_program, s);
      glDeleteShader(s);
    }
    // Delete program
    glDeleteProgram(_program);
    // Throw exception
    throw std::runtime_error("Error building effect");
  }

  // Effect built sucessfully.  Log
  std::clog << "LOG - effect built" << std::endl;
}

// Gets the uniform location of the named
GLint effect::get_uniform_location(const std::string &name) const {
  // Try and get the location from OpenGL
  auto loc = glGetUniformLocation(_program, name.c_str());
  // Return location
  return loc;
}
}
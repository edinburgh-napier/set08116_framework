#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/geometric.hpp> // glm::cross, glm::normalize
#include <glm/vec3.hpp>      // glm::vec3
#include <iostream>

using namespace std;

int main() {
  cout << "Hello, World!" << endl;

  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    return -1;
  }

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    auto const &a = glm::vec3(rand(), rand(), rand());
    auto const &b = glm::vec3(rand(), rand(), rand());
    auto const &c = glm::vec3(rand(), rand(), rand());
    auto const Normal = glm::normalize(glm::cross(c - a, b - a));
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
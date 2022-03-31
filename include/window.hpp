#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <utils.hpp>

#include <glad/glad.h>
#include <glfw3.h>

class Window{
public:
  Window(int width, int height, char *title);
  ~Window();

  void setSize(Size size);
  void setTitle(char* title);

  Size getSize();
  char* getTitle();

  void initGLFW();
  void initGLAD();
  static void processInput(GLFWwindow* window);
  static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

  int genData(float** vertices, unsigned int** indices);
  void setUpData(float** vertices, unsigned int** indices, int vertices_size_memory, int indices_size_memory);

  void run();

private:
  GLFWwindow* window;

  unsigned int vao;
  unsigned int vbo;
  unsigned int ebo;

  int width;
  int height;
  char* title;
};


#endif

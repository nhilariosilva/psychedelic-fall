#include <window.hpp>

#include <iostream>
#include <cmath>
#include <vector>

#include <glad/glad.h>
#include <glfw3.h>

#include <utils.hpp>
#include <shader.hpp>

Window::Window(int width, int height, char* title){
  this->width = width;
  this->height = height;
  this->title = title;

  this->initGLFW();

  this->window = glfwCreateWindow(width, height, title, NULL, NULL);
  if(window == NULL){
    std::cout << "Failed to create GLFW Window." << std::endl;
    glfwTerminate();

    this->~Window();
  }
}

Window::~Window(){

}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void Window::processInput(GLFWwindow* window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
  }
}

void Window::setSize(Size size){
  this->width = size.getWidth();
  this->height = size.getHeight();

  framebufferSizeCallback(window, width, height);
}

void Window::setTitle(char* title){
  this->title = title;

  glfwSetWindowTitle(window, title);
}

void Window::initGLFW(){
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::initGLAD(){
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
 		std::cout << "Failed to initialize GLAD" << std::endl;
		this->~Window();
	}
}

int Window::genData(float** vertices, unsigned int** indices){
  float t = 0;
  float rad = 2;
  Point point;
  std::vector<Point> points;

  int num_points = 0;

  points.push_back( Point(0.0f, 0.0f, 0.0f, false) );

  while(t <= 2*M_PI){
    point = Point(rad*cos(t), rad*sin(t), 0, true);
    //point.printPoint();
    points.push_back( point );
    t += 0.05;
  }

  num_points = points.size();
  (*vertices) = (float*) malloc(6 * num_points * sizeof(float));
  // num_points - 1 because of the center
  (*indices) = (unsigned int*) malloc(3 * (num_points-1) * sizeof(unsigned int));

  int i = 0;
  int j = 0;

  for (i = 0; i < num_points; i++){
    point = points[i];
    (*vertices)[j] = point.x;
    (*vertices)[j+1] = point.y;
    (*vertices)[j+2] = point.z;
    (*vertices)[j+3] = point.r;
    (*vertices)[j+4] = point.g;
    (*vertices)[j+5] = point.b;
    j += 6;
  }

  for (i = 0; i < 3 * (num_points-1); i++){
    if(i % 3 == 0){
      (*indices)[i] = 0;
    }else if(i % 3 == 1){
      (*indices)[i] = (int) (i+2) / 3;
    }else{
      (*indices)[i] = (int) (i+4) / 3;
      if ( (*indices)[i] == num_points )
        (*indices)[i] = 1;
    }
  }

  return num_points;
}

void Window::setUpData(float** vertices, unsigned int** indices, int vertices_size_memory, int indices_size_memory){
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices_size_memory, (*vertices), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size_memory, (*indices), GL_STATIC_DRAW);

  // Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
  // Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Window::run(){
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  initGLAD();

  Shader shader = Shader("shaders/vertex_shader", "shaders/fragment_shader");
  //shader.use();

  int num_points, vertices_size_memory, indices_size_memory;
  float* vertices;
  unsigned int* indices;

  num_points = genData(&vertices, &indices);

  vertices_size_memory = ( num_points * 6 ) * sizeof(float);
  indices_size_memory = ( 3 * (num_points-1) ) * sizeof(unsigned int);
  setUpData(&vertices, &indices, vertices_size_memory, indices_size_memory);

  int interval_change = 4;
  float r_speed, g_speed, b_speed;
  float time;

  r_speed = (float) rand() / (RAND_MAX);
  g_speed = (float) rand() / (RAND_MAX);
  b_speed = (float) rand() / (RAND_MAX);

  while(!glfwWindowShouldClose(window)){
    processInput(window);

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    time = glfwGetTime();
    shader.setFloat("inc_r", sin(r_speed * time) );
    shader.setFloat("inc_g", sin(g_speed * time) );
    shader.setFloat("inc_b", sin(b_speed * time) );

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices_size_memory/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

}

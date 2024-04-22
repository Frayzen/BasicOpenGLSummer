#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>

#define SIZE 8

std::string get_file_content(const char *filename) {
  std::ifstream file(filename);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}
void checkGLError(const char *message) {
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cerr << "OpenGL Error (" << message
              << "): " << glewGetErrorString(error);
    exit(1);
  }
}

GLuint setData(int pos, void *data) {
  GLuint ssbo;
  glGenBuffers(1, &ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, SIZE * sizeof(float), data, GL_DYNAMIC_COPY);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, pos, ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  return ssbo;
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  GLFWwindow *win = glfwCreateWindow(100, 100, "Test", nullptr, nullptr);
  glfwMakeContextCurrent(win);
  glewInit();

  GLuint program = glCreateProgram();

  GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
  auto content = get_file_content("shader.glsl");
  auto c = content.c_str();
  glShaderSource(shader, 1, &c, nullptr);
  glCompileShader(shader);
  // Check for compilation errors
  checkGLError("Failed to compile shader");
  std::cout << "Compilation succesful !" << std::endl;

  glAttachShader(program, shader);
  glLinkProgram(program);
  glUseProgram(program);

  float a[SIZE];
  float b[SIZE];

  for (int i = 0; i < SIZE; i++) {
    a[i] = 1.0f * i;
    b[i] = 1.0f * i * i;
  }
  GLuint abuff = setData(1, a);
  GLuint bbuff = setData(2, b);

  glDispatchCompute(SIZE, 1, 1);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, bbuff);
  float *res = (float *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
  for (int i = 0; i < SIZE; i++) 
    std::cout << b[i] << ' ';
  std::cout << "" << '\n';
  for (int i = 0; i < SIZE; i++) 
    std::cout << res[i] << ' ';
  std::cout << "" << '\n';

  glDeleteProgram(program);

  return 0;
}

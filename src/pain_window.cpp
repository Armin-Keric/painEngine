#include "pain_window.h"
#include <GLFW/glfw3.h>

namespace Pain {

PainWindow::PainWindow(int w, int h, const char* n) 
                    : width(w), height(h), title(n) 
{
  init();
}

void PainWindow::init() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
}

void PainWindow::inputs() {
  if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
  }
}

PainWindow::~PainWindow() {
  // VULKAN

  // GLFW
  glfwDestroyWindow(m_Window);
  glfwTerminate();
}

} // pain

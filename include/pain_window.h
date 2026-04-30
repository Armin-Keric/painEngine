#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Pain {
  class PainWindow {
    public:
      PainWindow(int w, int h, const char* n);

      PainWindow(const PainWindow &) = delete;
      PainWindow &operator=(const PainWindow &) = delete;

      const int getWidth() {return width;}
      const int getHeight() {return height;}
      void getFramebufferSize(int* w, int* h) {glfwGetFramebufferSize(m_pWindow, w, h);}

      bool shouldClose() { return glfwWindowShouldClose(this->m_pWindow); }
      void inputs();

      void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

      ~PainWindow();
    private:
      void init();

      GLFWwindow* m_pWindow = nullptr;

      const int width;
      const int height;
      const char* title;
  };
}

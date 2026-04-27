#pragma once

#include "pain_window.h"

#include <vector>
#include <iostream>
#include <optional>

namespace Pain {

  struct QueueInfo {
    std::optional<uint32_t> FamilyIndex;
    std::vector<float> Priorities;

    bool isComplete() { return FamilyIndex.has_value(); }
  };

  struct WindowParams {
  #ifdef VK_USE_PLATFORM_WIN32_KHR
    HINSTANCE HInstance;
    HWND HWnd;
  #elif defined VK_USE_PLATFORM_XLIB_KHR
    Display* Dpy;
    Window Window;
  #elif defined VK_USE_PLATFORM_XCB_KHR
    xcb_connection_t* Connection;
    xcb_window_t window;
  #endif
  };

  class PainDevice {
    public:
      PainDevice(PainWindow& window);
      ~PainDevice();
    private:
      void createInstance();

      void pickPhysicalDevice();
      QueueInfo pickQueueFamily();

      void setupDebugMessenger();
      void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

      void createLogicalDevice();
      void createCommandPool();
      void createSurface();

      // DBG
      static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* UserData
      ) 
      {
        std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
      }

      VkResult CreateDebugUtilsMessengerEXT(
          VkInstance instance,
          const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
          const VkAllocationCallbacks* pAllocator,
          VkDebugUtilsMessengerEXT* pDebugMessenger
      );

      void DestroyDebugUtilsMessengerEXT(
          VkInstance instance,
          VkDebugUtilsMessengerEXT debugMessenger,
          const VkAllocationCallbacks* pAllocator
      );

// === Members ===
      PainWindow& m_Window;

      // DBG
      VkDebugUtilsMessengerEXT debugMessenger;

      // Vulkan Context members
      const char* m_Layers[1] = {"VK_LAYER_KHRONOS_validation"};
      const char* m_DesiredDeviceExtensions[1];
      VkInstance m_Instance{};
      VkPhysicalDevice m_PhysicalDevice{}; // The device which vulkan is going to use.
      VkDevice m_Device{};
      VkQueue m_GraphicsQueue{};
      VkSurfaceKHR m_Surface{};
      VkCommandPool m_CommandPool{};
  };
}

#include "pain_device.h"
#include "pain_debug.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <iostream>

namespace Pain {

PainDevice::PainDevice(PainWindow& window) : m_Window(window) {
  createInstance();
  setupDebugMessenger();
  createSurface();
  pickPhysicalDevice();
  createLogicalDevice();
  createCommandPool();
}

void PainDevice::createInstance() {
  VkResult result;
  VkApplicationInfo appCreateInfo;
  appCreateInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appCreateInfo.pNext = nullptr;
  appCreateInfo.pApplicationName = "Vulkan woo";
  appCreateInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
  appCreateInfo.pEngineName = "Pain Engine";
  appCreateInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
  appCreateInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo instanceCreateInfo{};
  instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceCreateInfo.pNext = nullptr;
  instanceCreateInfo.flags = 0;
  instanceCreateInfo.pApplicationInfo = &appCreateInfo;
  instanceCreateInfo.enabledLayerCount = 0;
  instanceCreateInfo.ppEnabledLayerNames = nullptr;
  instanceCreateInfo.enabledExtensionCount = 0;
  instanceCreateInfo.ppEnabledExtensionNames = nullptr;

  // needed extensions from glfw
  uint32_t glfwExtensionsCount = 0;
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

  // dbug print
  std::cout << "Required GLFW Extensions: " << glfwExtensions << std::endl;

  std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  if (glfwExtensionsCount) {
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
    std::cout << "Registered extension\n";
  }
  
  uint32_t propertyCount = 0;
  vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
 
  // Enabling validation Layers
  // TODO make it optional (Release/Debug)
  uint32_t layerCount = 0;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  if(layerCount) {
    instanceCreateInfo.enabledLayerCount = (uint32_t)1;
    instanceCreateInfo.ppEnabledLayerNames = m_Layers;   

    populateDebugMessengerCreateInfo(debugCreateInfo);
    instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    std::cout << "Registered layer\n";
  }
  
  // TODO Check if the extension is supported by our Vulkan drivers
  
  result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance);
  ensure(result, "Failed to create Instance");

  std::cout << "Succesfully created instance!\n";
}

void PainDevice::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
     createInfo = {};
     createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
     createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
     createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
     createInfo.pfnUserCallback = debugCallback;
}


void PainDevice::setupDebugMessenger() {
  VkDebugUtilsMessengerCreateInfoEXT dbgMessengerCreateInfo = {
    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
    .pNext = nullptr,
    .flags = 0,
    .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
    .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
    .pfnUserCallback = debugCallback,
    .pUserData = nullptr
  };

  if (CreateDebugUtilsMessengerEXT(m_Instance, &dbgMessengerCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
    throw std::runtime_error("Failed to setup debug Messenger!");
  }
}

VkResult PainDevice::CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger)
{
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void PainDevice::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

void PainDevice::createSurface() {
  // UNIX
  VkResult result;

  result = glfwCreateWindowSurface(m_Instance, m_Window.m_Window, nullptr, &m_Surface);
  ensure(result, "Failed to create Window surface");
  std::cout << "Succesfully created Window Surface!\n";
}

void PainDevice::pickPhysicalDevice() {
  VkResult result;
  uint32_t deviceCount = 0;
  result = vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
  ensure(result, "Failed to get Device count");
  
  if (!deviceCount) {
    throw std::runtime_error("There are no devices that support vulkan!");
  }

  std::cout << "Found " << deviceCount << " device(s)\n";

  std::vector<VkPhysicalDevice> availableDevices(deviceCount);
  result = vkEnumeratePhysicalDevices(m_Instance, &deviceCount, availableDevices.data());
  ensure(result, "Failed to Enumerate Devices!");

  VkPhysicalDeviceProperties deviceProps{};
  VkPhysicalDeviceMemoryProperties deviceMemProps{};
  for (uint32_t i = 0; i < deviceCount; i++)  {
    vkGetPhysicalDeviceProperties(availableDevices[i], &deviceProps);
    vkGetPhysicalDeviceMemoryProperties(availableDevices[i], &deviceMemProps);

    // I don't like these lines below this comment for some reason. Maybe make it better in future??
    if (deviceCount == 1 && deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      m_PhysicalDevice = availableDevices[i];
    }

    std::cout << "Found Device: " << deviceProps.deviceName << "\n";
    std::cout << "Type: " << deviceProps.deviceType << "\n";
    std::cout << "ID: " << deviceProps.deviceID << "\n";
    std::cout << "Memory Type Count: " << deviceMemProps.memoryTypeCount << "\n";
    std::cout << "Memory Heap Count: " << deviceMemProps.memoryHeapCount << "\n";
  }
}

void PainDevice::createLogicalDevice() {
  VkResult result;

  QueueInfo indices = pickQueueFamily();

  float prio = 1.0f;
  VkDeviceQueueCreateInfo queueCreateInfo = {
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    queueCreateInfo.pNext = nullptr,
    queueCreateInfo.flags = (uint32_t)0,
    queueCreateInfo.queueFamilyIndex = indices.FamilyIndex.value(),
    queueCreateInfo.queueCount = 1,
    queueCreateInfo.pQueuePriorities = &prio,
  };

  VkDeviceCreateInfo deviceCreateInfo = {
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    deviceCreateInfo.pNext = nullptr,
    deviceCreateInfo.flags = 0,
    deviceCreateInfo.queueCreateInfoCount = 1,
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo,
    deviceCreateInfo.enabledLayerCount = 1,
    deviceCreateInfo.ppEnabledLayerNames = m_Layers,
    deviceCreateInfo.enabledExtensionCount = 0,
    deviceCreateInfo.ppEnabledExtensionNames = nullptr,
    deviceCreateInfo.pEnabledFeatures = nullptr
  };

  result = vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_Device);
  ensure(result, "Failed to create Logical Device!");
  std::cout << "Created Logical Device!\n";

  vkGetDeviceQueue(m_Device, indices.FamilyIndex.value(), 0, &m_GraphicsQueue);
  std::cout << "Queued device Queue!\n";
}

QueueInfo PainDevice::pickQueueFamily() {
  VkResult result;
  QueueInfo indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilyProps(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilyProps.data());

  VkQueueFamilyProperties pickedQueueFamily{};

  int i = 0;
  for (const VkQueueFamilyProperties& queueFamily : queueFamilyProps) {

    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.FamilyIndex = i;
      pickedQueueFamily = queueFamily;
    }

    if (indices.isComplete()) {
      break;
    }

    i++;
  }

// === Information ===
  //int k = 0;
  //std::cout << "\n" << "Queue Family information: " << std::endl;
  //for (const VkQueueFamilyProperties& queueFamily : queueFamilyProps) {
  //  std::cout << "Queue Index: " << k << "\n";

  //  std::cout << "  Compute: ";
  //  if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
  //    std::cout << "True\n";
  //  else
  //    std::cout << "False\n";

  //  std::cout << "  Transfer: ";
  //  if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
  //    std::cout << "True\n";
  //  else
  //    std::cout << "False\n";

  //  std::cout << "  Graphics: ";
  //  if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
  //    std::cout << "True\n";
  //  else
  //    std::cout << "False\n";

  //  std::cout << "\n";
  //  k++;
  //}
  return indices;
}

void PainDevice::createCommandPool() {
  // TODO Implement
  VkResult result;
  QueueInfo indices = pickQueueFamily();

  VkCommandPoolCreateInfo poolInfo = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = nullptr,
    .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    .queueFamilyIndex = indices.FamilyIndex.value(),
  };

  result = vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool);
  ensure(result, "Failed to create Command Pool!");
  std::cout << "Succesfully created command pool!\n";
}

PainDevice::~PainDevice() {
  vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
  vkDestroyDevice(m_Device, nullptr);
  DestroyDebugUtilsMessengerEXT(m_Instance, debugMessenger, nullptr);
  vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
  vkDestroyInstance(m_Instance, nullptr);
}

}

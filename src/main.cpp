#include "pain_app.h"

#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <vulkan/vulkan_core.h>

void printSupportedVulkanLayers() {
  uint32_t layerCount = 0;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> layerProps(layerCount);
  if(vkEnumerateInstanceLayerProperties(&layerCount, layerProps.data()) != VK_SUCCESS) {
    std::cerr << "Failed to enumerate Layer props\n";
  }
  std::cout << layerCount << " Layers supported\n";
  for (const auto& layer : layerProps) {
    std::cout << "NAME: " << layer.layerName << std::endl;
    std::cout << layer.description << "\n" << std::endl;
  }
}

int main() {
  // printSupportedVulkanLayers();

  Pain::FirstApp app{};

  try {
    app.run();
  } catch(std::exception& e) {
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

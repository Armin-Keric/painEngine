#include "pain_pipeline.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <filesystem>

namespace Pain {

PainPipeline::PainPipeline(const char* vertPath, const char* fragPath) {
  createGraphicsPipeline(vertPath, fragPath);
  // std::cout << "Skipping PainPipeline creation\n";
}

std::vector<char> PainPipeline::readFile(const char* filePath) {
  std::cout << "Trying to open: " << filePath << std::endl;
  std::cout << "Exists? " << std::filesystem::exists(filePath) << std::endl;
  std::cout << std::filesystem::current_path() << std::endl;
  std::ifstream file(filePath, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open File");
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();
  
  return buffer;
}

void PainPipeline::createGraphicsPipeline(const char* vertPath, const char* fragPath) {
  auto vertCode = readFile(vertPath);
  auto fragCode = readFile(fragPath);

  std::cout << "Vertex Shader size: " << vertCode.size() << std::endl;
  std::cout << "Frag Shader size: " << fragCode.size() << std::endl;
}

}

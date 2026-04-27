#pragma once

#include <vector>
namespace Pain {
  class PainPipeline {
    public:
      PainPipeline(const char* vertFile, const char* fragPath);

    private:
      static std::vector<char> readFile(const char* filePath);

      void createGraphicsPipeline(const char* vertPath, const char* fragPath);
  };
} // pain

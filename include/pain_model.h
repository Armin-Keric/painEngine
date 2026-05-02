#pragma once

#include  "pain_device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
#include <vector>

namespace Pain {
class PainModel {
  public:

    struct Vertex {
      glm::vec2 pos;

      static std::vector<VkVertexInputBindingDescription> getBindingDescription();
      static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
    };

    PainModel(PainDevice& device, const std::vector<Vertex> &vertices);
    ~PainModel();

    PainModel(const PainModel&) = delete;
    PainModel &operator=(const PainModel&) = delete;

    void bind(VkCommandBuffer cmdBuff);
    void draw(VkCommandBuffer cmdBuff);

  private:
    void createVertexBuffers(const std::vector<Vertex> &vertices);

    PainDevice& m_PainDevice;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    uint32_t vertexCount;
};
}
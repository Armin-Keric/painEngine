#include "pain_model.h"
#include "pain_debug.h"
#include <cstring>

namespace Pain {
PainModel::PainModel(PainDevice& device, const std::vector<Vertex>& vertices)
  : m_PainDevice(device)
{
  createVertexBuffers(vertices);
}

void PainModel::createVertexBuffers(const std::vector<Vertex>& vertices) {
  vertexCount = static_cast<uint32_t>(vertices.size());
  pain_assert(vertexCount >= 3, "Vertex count must be at least 3!");
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
  m_PainDevice.createBuffer(
    bufferSize,
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    vertexBuffer,
    vertexBufferMemory
  );

  void* data;
  vkMapMemory(m_PainDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(m_PainDevice.device(), vertexBufferMemory);
}

void PainModel::bind(VkCommandBuffer cmdBuffer) {
  VkBuffer buffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(cmdBuffer, 0, 1, buffers, offsets);
}

std::vector<VkVertexInputBindingDescription> PainModel::Vertex::getBindingDescription() {
  std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
  bindingDescriptions[0].binding = 0;
  bindingDescriptions[0].stride = sizeof(Vertex);
  bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> PainModel::Vertex::getAttributeDescription() {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions{1};
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[0].offset = 0;

  return attributeDescriptions;
}

void PainModel::draw(VkCommandBuffer cmdBuffer) {
  vkCmdDraw(cmdBuffer, vertexCount, 1, 0, 0);
}

PainModel::~PainModel() {
  vkDestroyBuffer(m_PainDevice.device(), vertexBuffer, nullptr);
  vkFreeMemory(m_PainDevice.device(), vertexBufferMemory, nullptr);
}
}

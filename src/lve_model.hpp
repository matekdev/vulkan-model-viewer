#pragma once

#include "lve_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace lve
{
    class LveModel
    {
    public:
        struct Vertex
        {
            glm::vec3 position;
            glm::vec3 color;
            static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
        };

        LveModel(LveDevice &device, const std::vector<Vertex> &vertices);
        ~LveModel();

        LveModel(const LveModel &) = delete;
        LveModel &operator=(const LveModel &) = delete;

        void Bind(VkCommandBuffer commandBuffer);
        void Draw(VkCommandBuffer commandBuffer);

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        LveDevice &_lveDevice;
        VkBuffer _vertexBuffer;
        VkDeviceMemory _vertexBufferMemory;
        uint32_t _vertexCount;
    };
}
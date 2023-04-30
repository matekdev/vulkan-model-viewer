#pragma once

#include "lve_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
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
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
        };

        struct Builder
        {
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};

            void LoadModel(const std::string &path);
        };

        LveModel(LveDevice &device, const Builder &builder);
        ~LveModel();

        LveModel(const LveModel &) = delete;
        LveModel &operator=(const LveModel &) = delete;

        static std::unique_ptr<LveModel> CreateModelFromFile(LveDevice &device, const std::string &path);

        void Bind(VkCommandBuffer commandBuffer);
        void Draw(VkCommandBuffer commandBuffer);

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);
        void createIndexBuffers(const std::vector<uint32_t> &indices);

        LveDevice &_lveDevice;

        VkBuffer _vertexBuffer;
        VkDeviceMemory _vertexBufferMemory;
        uint32_t _vertexCount;

        bool _hasIndexBuffer = false;
        VkBuffer _indexBuffer;
        VkDeviceMemory _indexBufferMemory;
        uint32_t _indexCount;
    };
}
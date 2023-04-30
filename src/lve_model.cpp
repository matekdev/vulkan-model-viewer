#include "lve_model.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <iostream>
#include <cassert>
#include <cstring>

namespace lve
{
    LveModel::LveModel(LveDevice &device, const Builder &builder)
        : _lveDevice{device}
    {
        createVertexBuffers(builder.vertices);
        createIndexBuffers(builder.indices);
    }

    LveModel::~LveModel()
    {
        vkDestroyBuffer(_lveDevice.device(), _vertexBuffer, nullptr);
        vkFreeMemory(_lveDevice.device(), _vertexBufferMemory, nullptr);

        if (_hasIndexBuffer)
        {
            vkDestroyBuffer(_lveDevice.device(), _indexBuffer, nullptr);
            vkFreeMemory(_lveDevice.device(), _indexBufferMemory, nullptr);
        }
    }

    void LveModel::createVertexBuffers(const std::vector<Vertex> &vertices)
    {
        _vertexCount = static_cast<uint32_t>(vertices.size());
        assert(_vertexCount >= 3 && "Vertex count must be >= 3.");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * _vertexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        _lveDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory);

        void *data;
        vkMapMemory(_lveDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(_lveDevice.device(), stagingBufferMemory);

        _lveDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            _vertexBuffer,
            _vertexBufferMemory);

        _lveDevice.copyBuffer(stagingBuffer, _vertexBuffer, bufferSize);

        vkDestroyBuffer(_lveDevice.device(), stagingBuffer, nullptr);
        vkFreeMemory(_lveDevice.device(), stagingBufferMemory, nullptr);
    }

    void LveModel::createIndexBuffers(const std::vector<uint32_t> &indices)
    {
        _indexCount = static_cast<uint32_t>(indices.size());
        _hasIndexBuffer = _indexCount > 0;

        if (!_hasIndexBuffer)
            return;

        VkDeviceSize bufferSize = sizeof(indices[0]) * _indexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        _lveDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory);

        void *data;
        vkMapMemory(_lveDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(_lveDevice.device(), stagingBufferMemory);

        _lveDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            _indexBuffer,
            _indexBufferMemory);

        _lveDevice.copyBuffer(stagingBuffer, _indexBuffer, bufferSize);

        vkDestroyBuffer(_lveDevice.device(), stagingBuffer, nullptr);
        vkFreeMemory(_lveDevice.device(), stagingBufferMemory, nullptr);
    }

    std::unique_ptr<LveModel> LveModel::CreateModelFromFile(LveDevice &device, const std::string &path)
    {
        Builder builder{};
        builder.LoadModel(path);

        std::cout << "Vertex count:" << builder.vertices.size() << "\n";

        return std::make_unique<LveModel>(device, builder);
    }

    void LveModel::Bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = {_vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

        if (_hasIndexBuffer)
            vkCmdBindIndexBuffer(commandBuffer, _indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }

    void LveModel::Draw(VkCommandBuffer commandBuffer)
    {
        if (_hasIndexBuffer)
            vkCmdDrawIndexed(commandBuffer, _indexCount, 1, 0, 0, 0);
        else
            vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0);
    }

    std::vector<VkVertexInputBindingDescription> LveModel::Vertex::GetBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> LveModel::Vertex::GetAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        return attributeDescriptions;
    }

    void LveModel::Builder::LoadModel(const std::string &path)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
        {
            throw std::runtime_error(warn + err);
        }

        vertices.clear();
        indices.clear();

        for (const auto &shape : shapes)
        {
            for (const auto &index : shape.mesh.indices)
            {
                Vertex vertex{};

                if (index.vertex_index >= 0)
                {
                    vertex.position = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2],
                    };

                    auto colorIndex = 3 * index.vertex_index + 2;
                    if (colorIndex < attrib.colors.size())
                    {
                        vertex.color = {
                            attrib.colors[colorIndex - 2],
                            attrib.colors[colorIndex - 1],
                            attrib.colors[colorIndex - 0],
                        };
                    }
                    else
                    {
                        vertex.color = {1.f, 1.f, 1.f}; // Default otherwise
                    }
                }

                if (index.normal_index >= 0)
                {
                    vertex.normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2],
                    };
                }

                if (index.texcoord_index >= 0)
                {
                    vertex.uv = {
                        attrib.texcoords[3 * index.texcoord_index + 0],
                        attrib.texcoords[3 * index.texcoord_index + 1],
                    };
                }

                vertices.push_back(vertex);
            }
        }
    }
}

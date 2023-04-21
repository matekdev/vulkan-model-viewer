#include "lve_pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace lve
{
    LvePipeline::LvePipeline(LveDevice &device, const std::string &vertFilePath, const std::string &fragFileMath, const PipelineConfigInfo &configInfo)
        : _lveDevice{device}
    {
        createGraphicsPipeline(vertFilePath, fragFileMath, configInfo);
    }

    PipelineConfigInfo LvePipeline::DefaultPipelineConfigInfo(uint32_t width, uint32_t height)
    {
        auto configInfo = PipelineConfigInfo{};
        return configInfo;
    }

    std::vector<char> LvePipeline::readFile(const std::string &filePath)
    {
        auto file = std::ifstream{filePath, std::ios::ate | std::ios::binary};

        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file: " + filePath);
        }

        auto fileSize = static_cast<size_t>(file.tellg());
        auto buffer = std::vector<char>(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }

    void LvePipeline::createGraphicsPipeline(const std::string &vertFilePath, const std::string &fragFileMath, const PipelineConfigInfo &configInfo)
    {
        auto vertCode = readFile(vertFilePath);
        auto fragCode = readFile(fragFileMath);

        std::cout << "Vertex shader code size " << vertCode.size() << "\n";
        std::cout << "Fragment shader code size " << fragCode.size() << "\n";
    }

    void LvePipeline::createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule)
    {
        auto createInfo = VkShaderModuleCreateInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

        if (vkCreateShaderModule(_lveDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create shader module.");
        }
    }
}
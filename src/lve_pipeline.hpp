#pragma once

#include "lve_device.hpp"

#include <string>
#include <vector>

namespace lve
{
    struct PipelineConfigInfo
    {
    };

    class LvePipeline
    {
    public:
        LvePipeline(LveDevice &device, const std::string &vertFilePath, const std::string &fragFileMath, const PipelineConfigInfo &configInfo);
        ~LvePipeline() {}

        LvePipeline(const LvePipeline &) = delete;
        void operator=(const LvePipeline &) = delete;

        static PipelineConfigInfo DefaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
        static std::vector<char> readFile(const std::string &filePath);
        void createGraphicsPipeline(const std::string &vertFilePath, const std::string &fragFileMath, const PipelineConfigInfo &configInfo);
        void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        LveDevice &_lveDevice;
        VkPipeline _graphicsPipeline;
        VkShaderModule _vertShaderModule;
        VkShaderModule _fragShaderModule;
    };
}
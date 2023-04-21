#include "lve_pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace lve
{
    LvePipeline::LvePipeline(const std::string &vertFilePath, const std::string &fragFileMath)
    {
        createGprahicsPipeline(vertFilePath, fragFileMath);
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

    void LvePipeline::createGprahicsPipeline(const std::string &vertFilePath, const std::string &fragFileMath)
    {
        auto vertCode = readFile(vertFilePath);
        auto fragCode = readFile(fragFileMath);

        std::cout << "Vertex shader code size " << vertCode.size() << "\n";
        std::cout << "Fragment shader code size " << fragCode.size() << "\n";
    }
}
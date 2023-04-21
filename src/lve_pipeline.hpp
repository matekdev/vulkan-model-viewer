#pragma once

#include <string>
#include <vector>

namespace lve
{
    class LvePipeline
    {
    public:
        LvePipeline(const std::string &vertFilePath, const std::string &fragFileMath);

    private:
        static std::vector<char> readFile(const std::string &filePath);
        void createGprahicsPipeline(const std::string &vertFilePath, const std::string &fragFileMath);
    };
}
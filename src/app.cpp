#include "app.hpp"

#include <stdexcept>

namespace lve
{
    App::App()
    {
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    App::~App()
    {
        vkDestroyPipelineLayout(_lveDevice.device(), _pipelineLayout, nullptr);
    }

    void App::Run()
    {
        while (!_lveWindow.ShouldClose())
        {
            glfwPollEvents();
        }
    }

    void App::createPipelineLayout()
    {
        auto pipelineLayoutInfo = VkPipelineLayoutCreateInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(_lveDevice.device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create pipeline layout.");
        }
    }

    void App::createPipeline()
    {
        auto pipelineConfig = LvePipeline::DefaultPipelineConfigInfo(_lveSwapChain.width(), _lveSwapChain.height());
        pipelineConfig.RenderPass = _lveSwapChain.getRenderPass();
        pipelineConfig.PipelineLayout = _pipelineLayout;
        _lvePipeline = std::make_unique<LvePipeline>(_lveDevice, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", pipelineConfig);
    }

    void App::createCommandBuffers() {}
    void App::drawFrame() {}
}
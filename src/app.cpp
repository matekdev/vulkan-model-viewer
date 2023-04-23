#include "app.hpp"

#include <stdexcept>
#include <array>

namespace lve
{
    App::App()
    {
        loadModels();
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
            drawFrame();
        }

        vkDeviceWaitIdle(_lveDevice.device());
    }

    void App::loadModels()
    {
        std::vector<LveModel::Vertex> vertices{
            {{0.0f, -0.5f}},
            {{0.5f, 0.5f}},
            {{-0.5f, 0.5f}},
        };

        _lveModel = std::make_unique<LveModel>(_lveDevice, vertices);
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

    void App::createCommandBuffers()
    {
        _commandBuffers.resize(_lveSwapChain.imageCount());
        auto allocInfo = VkCommandBufferAllocateInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = _lveDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

        if (vkAllocateCommandBuffers(_lveDevice.device(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate command buffers.");
        }

        for (int i = 0; i < _commandBuffers.size(); ++i)
        {
            auto beginInfo = VkCommandBufferBeginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to begin recording command buffer.");
            }

            auto renderPassInfo = VkRenderPassBeginInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = _lveSwapChain.getRenderPass();
            renderPassInfo.framebuffer = _lveSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = _lveSwapChain.getSwapChainExtent();

            auto clearValues = std::array<VkClearValue, 2>{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            _lvePipeline->Bind(_commandBuffers[i]);
            _lveModel->Bind(_commandBuffers[i]);
            _lveModel->Draw(_commandBuffers[i]);

            vkCmdEndRenderPass(_commandBuffers[i]);

            if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to record command buffer.");
            }
        }
    }

    void App::drawFrame()
    {
        uint32_t imageIndex;
        auto result = _lveSwapChain.acquireNextImage(&imageIndex);

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("Failed to acquire swap chain image.");
        }

        result = _lveSwapChain.submitCommandBuffers(&_commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to present swap chain image.");
        }
    }
}
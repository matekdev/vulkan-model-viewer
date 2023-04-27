#include "app.hpp"

#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace lve
{
    struct SimplePushConstantData
    {
        glm::vec2 Offset;
        alignas(16) glm::vec3 Color;
    };

    App::App()
    {
        loadModels();
        createPipelineLayout();
        recreateSwapChain();
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
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        };

        _lveModel = std::make_unique<LveModel>(_lveDevice, vertices);
    }

    void App::createPipelineLayout()
    {
        auto pushConstantRange = VkPushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        auto pipelineLayoutInfo = VkPipelineLayoutCreateInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(_lveDevice.device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create pipeline layout.");
        }
    }

    void App::createPipeline()
    {
        assert(_lveSwapChain != nullptr && "Cannot create pipeline before swap chain.");
        assert(_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout.");

        auto pipelineConfig = PipelineConfigInfo{};
        LvePipeline::DefaultPipelineConfigInfo(pipelineConfig);

        pipelineConfig.RenderPass = _lveSwapChain->getRenderPass();
        pipelineConfig.PipelineLayout = _pipelineLayout;
        _lvePipeline = std::make_unique<LvePipeline>(_lveDevice, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", pipelineConfig);
    }

    void App::recreateSwapChain()
    {
        auto extent = _lveWindow.GetExtent();
        while (extent.width == 0 || extent.height == 0)
        {
            extent = _lveWindow.GetExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(_lveDevice.device());

        if (_lveSwapChain == nullptr)
        {
            _lveSwapChain = std::make_unique<LveSwapChain>(_lveDevice, extent);
        }
        else
        {
            _lveSwapChain = std::make_unique<LveSwapChain>(_lveDevice, extent, std::move(_lveSwapChain));
            if (_lveSwapChain->imageCount() != _commandBuffers.size())
            {
                freeCommandBuffers();
                createCommandBuffers();
            }
        }

        createPipeline();
    }

    void App::createCommandBuffers()
    {
        _commandBuffers.resize(_lveSwapChain->imageCount());
        auto allocInfo = VkCommandBufferAllocateInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = _lveDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

        if (vkAllocateCommandBuffers(_lveDevice.device(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate command buffers.");
        }
    }

    void App::freeCommandBuffers()
    {
        vkFreeCommandBuffers(_lveDevice.device(), _lveDevice.getCommandPool(), static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
        _commandBuffers.clear();
    }

    void App::recordCommandBuffer(int imageIndex)
    {
        static int frame = 0;
        frame = (frame + 1) % 500;

        auto beginInfo = VkCommandBufferBeginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(_commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording command buffer.");
        }

        auto renderPassInfo = VkRenderPassBeginInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = _lveSwapChain->getRenderPass();
        renderPassInfo.framebuffer = _lveSwapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = _lveSwapChain->getSwapChainExtent();

        auto clearValues = std::array<VkClearValue, 2>{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(_commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        auto viewport = VkViewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(_lveSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(_lveSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, _lveSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(_commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(_commandBuffers[imageIndex], 0, 1, &scissor);

        _lvePipeline->Bind(_commandBuffers[imageIndex]);
        _lveModel->Bind(_commandBuffers[imageIndex]);

        for (int i = 0; i < 4; ++i)
        {
            auto push = SimplePushConstantData{};
            push.Offset = {-0.5f + frame * 0.02f, -0.4f + i * 0.25f};
            push.Color = {0.0f, 0.0f, 0.2f + 0.2f * i};

            vkCmdPushConstants(
                _commandBuffers[imageIndex],
                _pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData), &push);

            _lveModel->Draw(_commandBuffers[imageIndex]);
        }

        vkCmdEndRenderPass(_commandBuffers[imageIndex]);

        if (vkEndCommandBuffer(_commandBuffers[imageIndex]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to record command buffer.");
        }
    }

    void App::drawFrame()
    {
        uint32_t imageIndex;
        auto result = _lveSwapChain->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("Failed to acquire swap chain image.");
        }

        recordCommandBuffer(imageIndex);
        result = _lveSwapChain->submitCommandBuffers(&_commandBuffers[imageIndex], &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _lveWindow.WasWindowResized())
        {
            _lveWindow.ResetWindowResizedFlag();
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to present swap chain image.");
        }
    }
}
#include "lve_renderer.hpp"

#include <stdexcept>
#include <array>

namespace lve
{
    LveRenderer::LveRenderer(LveWindow &window, LveDevice &device) : _lveWindow{window}, _lveDevice{device}
    {
        recreateSwapChain();
        createCommandBuffers();
    }

    LveRenderer::~LveRenderer()
    {
        freeCommandBuffers();
    }

    void LveRenderer::recreateSwapChain()
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
            std::shared_ptr<LveSwapChain> oldSwapChain = std::move(_lveSwapChain);
            _lveSwapChain = std::make_unique<LveSwapChain>(_lveDevice, extent, oldSwapChain);

            if (!oldSwapChain->compareSwapFormats(*_lveSwapChain.get()))
            {
                throw std::runtime_error("Swap chain image (or depth) format has changed.");
            }
        }

        // We'll come back to this in just a cmoment
    }

    void LveRenderer::createCommandBuffers()
    {
        _commandBuffers.resize(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
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

    void LveRenderer::freeCommandBuffers()
    {
        vkFreeCommandBuffers(_lveDevice.device(), _lveDevice.getCommandPool(), static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
        _commandBuffers.clear();
    }

    VkCommandBuffer LveRenderer::BeginFrame()
    {
        assert(!_isFrameStarted && "Can't call BeginFrame() while already in progress.");

        auto result = _lveSwapChain->acquireNextImage(&_currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("Failed to acquire swap chain image.");
        }

        _isFrameStarted = true;

        auto commandBuffer = GetCurrentCommandBuffer();
        auto beginInfo = VkCommandBufferBeginInfo{};

        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording command buffer.");
        }

        return commandBuffer;
    }

    void LveRenderer::EndFrame()
    {
        assert(_isFrameStarted && "Can't call EndFrame() while frame is not in progress.");
        auto commandBuffer = GetCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to record command buffer.");
        }

        auto result = _lveSwapChain->submitCommandBuffers(&commandBuffer, &_currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _lveWindow.WasWindowResized())
        {
            _lveWindow.ResetWindowResizedFlag();
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to present swap chain image.");
        }

        _isFrameStarted = false;
        _currentFrameIndex = (_currentFrameIndex + 1) % LveSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void LveRenderer::BeginSwapChainRenderPass(VkCommandBuffer commandbuffer)
    {
        assert(_isFrameStarted && "Can't call BeginSwapChainRenderPass(...) if frame is not in progress");
        assert(commandbuffer == GetCurrentCommandBuffer() && "Can't begin render pass on command buffer from different frame.");

        auto renderPassInfo = VkRenderPassBeginInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = _lveSwapChain->getRenderPass();
        renderPassInfo.framebuffer = _lveSwapChain->getFrameBuffer(_currentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = _lveSwapChain->getSwapChainExtent();

        auto clearValues = std::array<VkClearValue, 2>{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandbuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        auto viewport = VkViewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(_lveSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(_lveSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, _lveSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandbuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandbuffer, 0, 1, &scissor);
    }

    void LveRenderer::EndSwapChainRenderPass(VkCommandBuffer commandbuffer)
    {
        assert(_isFrameStarted && "Can't call EndSwapChainRenderPass(...) if frame is not in progress");
        assert(commandbuffer == GetCurrentCommandBuffer() && "Can't end render pass on command buffer from different frame.");

        vkCmdEndRenderPass(commandbuffer);
    }
}

#pragma once

#include "lve_window.hpp"
#include "lve_device.hpp"
#include "lve_swap_chain.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace lve
{
    class LveRenderer
    {
    public:
        LveRenderer(LveWindow &window, LveDevice &device);
        ~LveRenderer();

        LveRenderer(const LveRenderer &) = delete;
        LveRenderer &operator=(const LveRenderer &) = delete;

        VkRenderPass GetSwapChainRenderPass() const { return _lveSwapChain->getRenderPass(); }
        bool IsFrameInProgress() const { return _isFrameStarted; }

        VkCommandBuffer GetCurrentCommandBuffer() const
        {
            assert(_isFrameStarted && "Cannot get command buffer when frame is not in progress.");
            return _commandBuffers[_currentFrameIndex];
        }

        int GetFrameIndex() const
        {
            assert(_isFrameStarted && "Cannot get frame index when frame is not in progress.");
            return _currentFrameIndex;
        }

        VkCommandBuffer BeginFrame();
        void EndFrame();
        void BeginSwapChainRenderPass(VkCommandBuffer commandbuffer);
        void EndSwapChainRenderPass(VkCommandBuffer commandbuffer);

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        LveWindow &_lveWindow;
        LveDevice &_lveDevice;
        std::unique_ptr<LveSwapChain> _lveSwapChain;
        std::vector<VkCommandBuffer> _commandBuffers;

        uint32_t _currentImageIndex{0};
        int _currentFrameIndex{0};
        bool _isFrameStarted{false};
    };
}

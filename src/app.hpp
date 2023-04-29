#pragma once

#include "lve_window.hpp"
#include "lve_pipeline.hpp"
#include "lve_device.hpp"
#include "lve_swap_chain.hpp"
#include "lve_object.hpp"

#include <memory>
#include <vector>

namespace lve
{
    class App
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        App();
        ~App();

        App(const App &) = delete;
        App &operator=(const App &) = delete;

        void Run();

    private:
        void loadObjects();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);
        void renderObjects(VkCommandBuffer commandBuffer);

        LveWindow _lveWindow{WIDTH, HEIGHT, "Vulkan Model Viewer"};
        LveDevice _lveDevice{_lveWindow};
        std::unique_ptr<LveSwapChain> _lveSwapChain;
        std::unique_ptr<LvePipeline> _lvePipeline;
        VkPipelineLayout _pipelineLayout;
        std::vector<VkCommandBuffer> _commandBuffers;
        std::vector<LveObject> _objects;
    };
}
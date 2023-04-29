#pragma once

#include "lve_pipeline.hpp"
#include "lve_device.hpp"
#include "lve_object.hpp"

#include <memory>
#include <vector>

namespace lve
{
    class SimpleRenderSystem
    {
    public:
        SimpleRenderSystem(LveDevice &device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

        void renderObjects(VkCommandBuffer commandBuffer, std::vector<LveObject> &objects);

    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        LveDevice &_lveDevice;

        std::unique_ptr<LvePipeline> _lvePipeline;
        VkPipelineLayout _pipelineLayout;
    };
}
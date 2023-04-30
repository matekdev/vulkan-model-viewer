#include "simple_render_system.hpp"

#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lve
{
    struct SimplePushConstantData
    {
        glm::mat4 Transform{1.f};
        alignas(16) glm::vec3 Color;
    };

    SimpleRenderSystem::SimpleRenderSystem(LveDevice &device, VkRenderPass renderPass) : _lveDevice{device}
    {
        createPipelineLayout();
        createPipeline(renderPass);
    }

    SimpleRenderSystem::~SimpleRenderSystem()
    {
        vkDestroyPipelineLayout(_lveDevice.device(), _pipelineLayout, nullptr);
    }

    void SimpleRenderSystem::createPipelineLayout()
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

    void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
    {
        assert(_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout.");

        auto pipelineConfig = PipelineConfigInfo{};
        LvePipeline::DefaultPipelineConfigInfo(pipelineConfig);

        pipelineConfig.RenderPass = renderPass;
        pipelineConfig.PipelineLayout = _pipelineLayout;
        _lvePipeline = std::make_unique<LvePipeline>(_lveDevice, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv", pipelineConfig);
    }

    void SimpleRenderSystem::renderObjects(VkCommandBuffer commandBuffer, std::vector<LveObject> &objects, const LveCamera &camera)
    {
        _lvePipeline->Bind(commandBuffer);

        auto projectionView = camera.GetProjection() * camera.GetView();

        for (auto &obj : objects)
        {
            obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.01f, glm::two_pi<float>());
            obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.005f, glm::two_pi<float>());

            auto push = SimplePushConstantData{};
            push.Color = obj.color;
            push.Transform = projectionView * obj.transform.Mat4();

            vkCmdPushConstants(
                commandBuffer,
                _pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData), &push);

            obj.model->Bind(commandBuffer);
            obj.model->Draw(commandBuffer);
        }
    }
}
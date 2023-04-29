#include "app.hpp"

#include "simple_render_system.hpp"

#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lve
{
    App::App()
    {
        loadObjects();
    }

    App::~App() {}

    void App::Run()
    {
        auto simpleRenderSystem = SimpleRenderSystem{_lveDevice, _lveRenderer.GetSwapChainRenderPass()};

        while (!_lveWindow.ShouldClose())
        {
            glfwPollEvents();

            if (auto commandBuffer = _lveRenderer.BeginFrame())
            {
                _lveRenderer.BeginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderObjects(commandBuffer, _objects);
                _lveRenderer.EndSwapChainRenderPass(commandBuffer);
                _lveRenderer.EndFrame();
            }
        }

        vkDeviceWaitIdle(_lveDevice.device());
    }

    void App::loadObjects()
    {
        std::vector<LveModel::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        };

        auto lveModel = std::make_shared<LveModel>(_lveDevice, vertices);

        auto triangle = LveObject::CreateObject();
        triangle.model = lveModel;
        triangle.color = {.1f, .8f, .1f};
        triangle.transform2d.Translation.x = .2f;
        triangle.transform2d.scale = {2.f, .5f};
        triangle.transform2d.rotation = .25f * glm::two_pi<float>();

        _objects.push_back(std::move(triangle));
    }
}

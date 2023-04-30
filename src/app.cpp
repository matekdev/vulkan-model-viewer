#include "app.hpp"

#include "keyboard_movement_controller.hpp"
#include "lve_camera.hpp"
#include "simple_render_system.hpp"

#include <stdexcept>
#include <array>
#include <chrono>

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
        auto camera = LveCamera{};

        auto viewerObject = LveObject::CreateObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!_lveWindow.ShouldClose())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            auto frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.MoveInPlaneXZ(_lveWindow.GetGLFWWindow(), frameTime, viewerObject);
            camera.SetViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            auto aspect = _lveRenderer.GetAspectRatio();
            camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

            if (auto commandBuffer = _lveRenderer.BeginFrame())
            {
                _lveRenderer.BeginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderObjects(commandBuffer, _objects, camera);
                _lveRenderer.EndSwapChainRenderPass(commandBuffer);
                _lveRenderer.EndFrame();
            }
        }

        vkDeviceWaitIdle(_lveDevice.device());
    }

    void App::loadObjects()
    {
        std::shared_ptr<LveModel> lveModel = LveModel::CreateModelFromFile(_lveDevice, "../models/bunny.obj");

        auto obj = LveObject::CreateObject();
        obj.model = lveModel;
        obj.transform.translation = {.0f, .0f, 2.5f};
        obj.transform.scale = glm::vec3(3.f);
        _objects.push_back(std::move(obj));
    }
}

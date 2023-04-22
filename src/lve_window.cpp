#include "lve_window.hpp"

#include <stdexcept>

namespace lve
{
    LveWindow::LveWindow(int w, int h, std::string name) : _width{w}, _height{h}, _windowName{name}
    {
        initWindow();
    }

    LveWindow::~LveWindow()
    {
        glfwDestroyWindow(_window);
        glfwTerminate();
    }

    bool LveWindow::ShouldClose()
    {
        return glfwWindowShouldClose(_window);
    }

    VkExtent2D LveWindow::GetExtent()
    {
        return {static_cast<uint32_t>(_width), static_cast<uint32_t>(_height)};
    }

    void LveWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, _window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create window surface.");
        }
    }

    void LveWindow::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        _window = glfwCreateWindow(_width, _height, _windowName.c_str(), nullptr, nullptr);
    }
}
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace lve
{
    class LveWindow
    {
    public:
        LveWindow(int w, int h, std::string name);
        ~LveWindow();

        LveWindow(const LveWindow &) = delete;
        LveWindow &operator=(const LveWindow &) = delete;

        bool ShouldClose();
        VkExtent2D GetExtent();
        bool WasWindowResized();
        void ResetWindowResizedFlag();

        void CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
        void initWindow();
        int _width;
        int _height;
        bool _framebufferResized = false;

        std::string _windowName;
        GLFWwindow *_window;
    };
}
#include "lve_window.hpp"

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

    void LveWindow::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        _window = glfwCreateWindow(_width, _height, _windowName.c_str(), nullptr, nullptr);
    }
}
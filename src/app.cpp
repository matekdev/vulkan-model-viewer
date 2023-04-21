#include "app.hpp"

namespace lve
{
    void App::Run()
    {
        while (!_lveWindow.ShouldClose())
        {
            glfwPollEvents();
        }
    }
}
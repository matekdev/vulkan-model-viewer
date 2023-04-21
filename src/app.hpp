#pragma once

#include "lve_window.hpp"

namespace lve
{
    class App
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void Run();

    private:
        LveWindow _lveWindow{WIDTH, HEIGHT, "Vulkan Model Viewer"};
    };
}
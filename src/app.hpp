#pragma once

#include "lve_window.hpp"
#include "lve_pipeline.hpp"

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
        LvePipeline _lvePipeline{"../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv"};
    };
}
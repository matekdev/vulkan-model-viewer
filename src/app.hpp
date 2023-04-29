#pragma once

#include "lve_device.hpp"
#include "lve_object.hpp"
#include "lve_renderer.hpp"
#include "lve_window.hpp"

#include <memory>
#include <vector>

namespace lve
{
    class App
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        App();
        ~App();

        App(const App &) = delete;
        App &operator=(const App &) = delete;

        void Run();

    private:
        void loadObjects();

        LveWindow _lveWindow{WIDTH, HEIGHT, "Vulkan Model Viewer"};
        LveDevice _lveDevice{_lveWindow};
        LveRenderer _lveRenderer{_lveWindow, _lveDevice};

        std::vector<LveObject> _objects;
    };
}
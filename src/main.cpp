#include "app.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main()
{
    auto app = lve::App{};

    try
    {
        app.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
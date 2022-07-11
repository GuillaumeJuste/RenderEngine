#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "Engine/Application/GraphicsApplication.hpp"

using namespace RenderEngine;

int main() {
    GraphicsApplication app;

    try {
        app.Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
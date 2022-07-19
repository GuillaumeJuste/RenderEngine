#include "Engine/Application/GraphicsApplication.hpp"
#include "Engine/Device/DeviceContext.hpp"
#include "Engine/Window/Window.hpp"
#include <iostream>

using namespace RenderEngine;

void GraphicsApplication::Run()
{
    InitWindow();
    InitVulkan();
    MainLoop();
    Cleanup();
}

void GraphicsApplication::InitWindow()
{
    std::cout << "[Initialize] Window" << std::endl;
    window = new Window(1024, 720, "Render Engine");
}

void GraphicsApplication::InitVulkan()
{
    std::cout << "[Initialize] Vulkan" << std::endl;
    deviceContext = new DeviceContext(window);
}

void GraphicsApplication::MainLoop()
{
    while (!glfwWindowShouldClose(window->GetGLFWWindow())) {
        glfwPollEvents();
        deviceContext->GetDevice()->DrawFrame();
    }
    vkDeviceWaitIdle(deviceContext->GetDevice()->GetLogicalDevice());
}

void GraphicsApplication::Cleanup()
{
    std::cout << "[Cleaning] Vulkan context" << std::endl;
    deviceContext->Cleanup();
    delete deviceContext;
    std::cout << "[Cleaned] Vulkan context" << std::endl;

    std::cout << "[Cleaning] Window" << std::endl;
    window->Cleanup();
    delete window;
    std::cout << "[Cleaned] Window" << std::endl;
}
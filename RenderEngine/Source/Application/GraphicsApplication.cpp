#include "Application/GraphicsApplication.hpp"
#include "Core/Object/GameObject/GameObject.hpp"

#include <iostream>


using namespace RenderEngine;
using namespace RenderEngine::Vulkan;
using namespace RenderEngine::Core;

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
    vulkanContext = new VulkanContext(window);

    deviceContext = vulkanContext->CreateDeviceContext();
}

void GraphicsApplication::MainLoop()
{
    SceneCreateInfo sceneInfo;
    sceneInfo.name = "test_scene_1";

    Scene::InitializeScene(sceneInfo, &scene);

    const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0
    };
    
    Mesh mesh;
    Mesh::InitializeMesh(vertices, indices, &mesh);

    Mathlib::Transform transform;

    GameObjectCreateInfo createinfo;
    createinfo.mesh = &mesh;
    createinfo.transform = transform;
    createinfo.parent = scene.GetSceneRoot();
    createinfo.name = "first_object";

    GameObject* obj = scene.AddGameObject(createinfo);


    while (!glfwWindowShouldClose(window->GetGLFWWindow())) {
        glfwPollEvents();
        deviceContext->GetRenderContext()->DrawFrame();
    }
    vkDeviceWaitIdle(deviceContext->GetLogicalDevice());
}

void GraphicsApplication::Cleanup()
{
    vulkanContext->Cleanup();
    delete vulkanContext;

    window->Cleanup();
    delete window;
}
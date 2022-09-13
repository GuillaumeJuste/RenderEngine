#include "Application/GraphicsApplication.hpp"
#include "Core/Object/GameObject/GameObject.hpp"

#include <iostream>


using namespace RenderEngine;
using namespace RenderEngine::Engine::Vulkan;
using namespace RenderEngine::Core;
using namespace RenderEngine::Window;

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
    window = new GLFW::Window(1024, 720, "Render Engine");
}

void GraphicsApplication::InitVulkan()
{
    std::cout << "[Initialize] Vulkan" << std::endl;
    IEngineInstanceCreateInfo instanceCreateInfo;

    vulkanContext.InitializeInstance(instanceCreateInfo);

    IDeviceContextCreateInfo deviceCreateInfo;
    deviceCreateInfo.window = window;

    deviceContext = vulkanContext.CreateDeviceContext(deviceCreateInfo);

    IRenderContextCreateInfo renderContextCreateInfo;
    renderContext = deviceContext->CreateRenderContext(renderContextCreateInfo);
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
    scene.RemoveGameObject(obj);

    if (obj == obj)

    while (!glfwWindowShouldClose(window->GetGLFWWindow())) {
        glfwPollEvents();
        renderContext->DrawFrame();
    }
    deviceContext->WaitDeviceIdle();
}

void GraphicsApplication::Cleanup()
{
    vulkanContext.Cleanup();

    window->Cleanup();
    delete window;
}
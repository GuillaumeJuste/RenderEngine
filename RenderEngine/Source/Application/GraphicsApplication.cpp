#include "Application/GraphicsApplication.hpp"
#include "Core/Object/GameObject/GameObject.hpp"
#include "Core/Scene/Scene.hpp"

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
    deviceContext = new DeviceContext(window);
}

void GraphicsApplication::MainLoop()
{
    SceneCreateInfo sceneInfo;
    sceneInfo.name = "test_scene_1";

    Scene scene;
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
        deviceContext->GetDevice()->DrawFrame();
    }
    vkDeviceWaitIdle(deviceContext->GetDevice()->GetLogicalDevice());
}

void GraphicsApplication::Cleanup()
{
    deviceContext->Cleanup();
    delete deviceContext;

    window->Cleanup();
    delete window;
}
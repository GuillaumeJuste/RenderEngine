#include "Application/GraphicsApplication.hpp"
#include "Core/Object/GameObject/GameObject.hpp"
#include "Core/Components/MeshRenderer/MeshRenderer.hpp"
#include "Core/Components/Light/Light.hpp"
#include "Application/CustomComponents/RotatorComponent.hpp"
#include <iostream>


using namespace RenderEngine;
using namespace RenderEngine::Engine::Vulkan;
using namespace RenderEngine::Core;
using namespace RenderEngine::Window;

void GraphicsApplication::Run()
{
    InitWindow();
    ressourceManager = RessourceManager::GetInstance();
    InitEngine();
    MainLoop();
    Cleanup();
}

void GraphicsApplication::InitWindow()
{
    std::cout << "[Initialize] Window" << std::endl;
    window = new GLFW::Window(1024, 720, "Render Engine");
}

void GraphicsApplication::InitEngine()
{
    std::cout << "[Initialize] Vulkan" << std::endl;
    IEngineInstanceCreateInfo instanceCreateInfo {};
    instanceCreateInfo.applicationName = "RenderEngine";
    instanceCreateInfo.applicationVersion = Mathlib::Vec3(1.f, 0.f, 0.f);
    instanceCreateInfo.engineName = "VulkanRenderer";
    instanceCreateInfo.engineVersion = Mathlib::Vec3(1.f , 0.f, 0.f);
    vulkanContext.InitializeInstance(instanceCreateInfo);

    IDeviceContextCreateInfo deviceCreateInfo {};
    deviceCreateInfo.window = window;
    deviceContext = vulkanContext.CreateDeviceContext(deviceCreateInfo);
    std::vector<std::string> deviceNames = deviceContext->QueryAvailblePhysicalDevices();
    deviceContext->InitializeDeviceContext(UserSelectPhysicalDevice(deviceNames));

    SwapChainCommandBufferCreateInfo CBCreateInfo{};
    CBCreateInfo.customViewport = false;
    CBCreateInfo.customScissor = false;

    IRenderContextCreateInfo renderContextCreateInfo {};
    renderContextCreateInfo.swapChainCommandBufferCreateInfo = &CBCreateInfo;
    renderContext = deviceContext->CreateRenderContext(renderContextCreateInfo);
}

std::string GraphicsApplication::UserSelectPhysicalDevice(std::vector<std::string> _physicalDevicesNames)
{
    size_t size = _physicalDevicesNames.size();
    

    if(size == 0)
        throw std::runtime_error("No GPU listed");

    else if(size == 1)
        return _physicalDevicesNames[0];
    
    std::cout << "Pick GPU for rendering :" << std::endl;

    for (int i = 0; i < size; i++)
    {
        std::cout << "[" << i << "] : " << _physicalDevicesNames[i] << std::endl;
    }
    int gpuIndex = -1;
    while (gpuIndex < 0 || gpuIndex >= size)
    {
        std::cin >> gpuIndex;
    }

    return _physicalDevicesNames[gpuIndex];
}

Scene* GraphicsApplication::SetupTestScene()
{
    Scene* scene = sceneManager.AddScene();
    scene->name = "test_scene_1";

    Camera* camera = scene->GetCamera();
    Mathlib::Transform cameraTransform;
    cameraTransform.position = Mathlib::Vec3(0.0f, 0.0f, -10.0f);
    camera->SetLocalTransform(cameraTransform);

    Mesh* mesh = RessourceManager::GetInstance()->LoadMesh("Resources/Models/viking_room.obj");

    Texture* texture = RessourceManager::GetInstance()->LoadTexture("Resources/Textures/viking_room.png");
    Texture* texture2 = RessourceManager::GetInstance()->LoadTexture("Resources/Textures/texture.jpg");

    Mathlib::Transform transform;
    transform.position = Mathlib::Vec3(0.f, .0f, 0.f);
    transform.scale = Mathlib::Vec3(1.f, 1.f, 1.f);
    transform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 90.f, 90.f));

    GameObjectCreateInfo createinfo;
    createinfo.transform = transform;
    createinfo.parent = nullptr;
    createinfo.name = "first_object";

    GameObject* obj = scene->AddGameObject(createinfo);

    MeshRenderer* meshRenderer = obj->GetComponent<MeshRenderer>();
    meshRenderer->SetMesh(mesh);
    meshRenderer->SetTexture(texture);
    meshRenderer->fragmentShaderFilePath = "Resources/Shaders/TextureFragmentShader.spv";

    Mathlib::Transform transform2;
    transform2.position = Mathlib::Vec3(2.f, 3.0f, 2.f);
    transform2.scale = Mathlib::Vec3(0.5f, 0.5f, 0.5f);

    GameObjectCreateInfo createinfo2;
    createinfo2.transform = transform2;
    createinfo2.parent = nullptr;
    createinfo2.name = "second_object";

    GameObject* obj2 = scene->AddGameObject(createinfo2);
    MeshRenderer* meshRenderer2 = obj2->GetComponent<MeshRenderer>();
    meshRenderer2->SetTexture(texture2);

    RotatorComponent* rotator2 = obj2->AddComponent <RotatorComponent>();
    rotator2->rotationAxis = ROTATION_AXIS::Z;


    /* Mathlib::Transform transform4;
    transform4.position = Mathlib::Vec3(1.5f, 0.0f, 0.f);
    transform4.scale = Mathlib::Vec3(0.5f, 0.5f, 0.5f);

    GameObjectCreateInfo createinfo4;
    createinfo4.transform = transform4;
    createinfo4.parent = obj2;
    createinfo4.name = "fourth_object";

    GameObject* obj4 = scene->AddGameObject(createinfo4);*/


    return scene;
}

Scene* GraphicsApplication::SetupIlluminationScene()
{
    Scene* scene = sceneManager.AddScene();
    scene->name = "test_scene_illumination";

    Camera* camera = scene->GetCamera();
    Mathlib::Transform cameraTransform;
    cameraTransform.position = Mathlib::Vec3(0.0f, 0.0f, -3.0f);
    cameraTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, 0.f));
    camera->SetLocalTransform(cameraTransform);

    Texture* texture = RessourceManager::GetInstance()->LoadTexture("Resources/Textures/White.jpg");

    Mathlib::Transform transform;
    transform.position = Mathlib::Vec3(0.f, 0.0f, 0.f);
    transform.scale = Mathlib::Vec3(0.5f, 0.5f, 0.5f);
    transform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 45.f, 0.f));

    GameObjectCreateInfo createinfo;
    createinfo.transform = transform;
    createinfo.parent = nullptr;
    createinfo.name = "first_object";

    GameObject* obj = scene->AddGameObject(createinfo);

    MeshRenderer* meshRenderer = obj->GetComponent<MeshRenderer>();
    meshRenderer->SetTexture(texture);
    meshRenderer->ambient = Mathlib::Vec3(1.0f, 0.5f, 0.31f);
    meshRenderer->diffuse = Mathlib::Vec3(1.0f, 0.5f, 0.31f);
    meshRenderer->specular = Mathlib::Vec3(0.5f, 0.5f, 0.5f);
    meshRenderer->shininess = 32.0f;

    RotatorComponent* rotator = obj->AddComponent<RotatorComponent>();
    rotator->rotationAxis = ROTATION_AXIS::Y;

    Mathlib::Transform transform2;
    transform2.position = Mathlib::Vec3(0.f, 0.0f, -3.f);
    transform2.scale = Mathlib::Vec3(0.1f, 0.1f, 0.1f);

    GameObjectCreateInfo createinfo2;
    createinfo2.transform = transform2;
    createinfo2.parent = nullptr;
    createinfo2.name = "light_1";

    GameObject* obj2 = scene->AddGameObject(createinfo2);

    Light* light = obj2->AddComponent<Light>();
    light->color = Mathlib::Vec3(1.0f, 0.42f, 0.0f);
    light->ambient = Mathlib::Vec3(0.2f, 0.2f, 0.2f);
    light->diffuse = Mathlib::Vec3(0.5f, 0.5f, 0.5f);
    light->specular = Mathlib::Vec3(1.0f, 1.0f, 1.0f);

    return scene;
}

void GraphicsApplication::MainLoop()
{
    Scene* scene = SetupIlluminationScene();

    scene->Initialize();
    scene->Start();

    while (!glfwWindowShouldClose(window->GetGLFWWindow())) {
        glfwPollEvents();
        renderContext->DrawScene(scene);
        scene->Update();
    }
    deviceContext->WaitDeviceIdle();
}

void GraphicsApplication::Cleanup()
{
    vulkanContext.Cleanup();

    window->Cleanup();
    delete window;
}
#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "Rendering/Vulkan/Context/VulkanContext/VulkanContext.hpp"
#include "Window/Glfw/Window.hpp"
#include "SceneGraph/Scene/SceneManager.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "SceneGraph/Object/GameObject/GameObject.hpp"
#include "SceneGraph/Components/MeshRenderer/MeshRenderer.hpp"
#include "SceneGraph/Components/Light/PointLight.hpp"
#include "SceneGraph/Components/Light/DirectionalLight.hpp"
#include "SceneGraph/Components/Light/SpotLight.hpp"
#include "Component/RotatorComponent.hpp"
#include "Component/CameraController.hpp"
#include <iostream>

#include<Mathlib/Mathlib/Include/Misc/Math.hpp>

using namespace RenderEngine::Rendering;
using namespace RenderEngine;

/**
* @brief Window for engine rendering
*/
GLFW::Window* window;

/**
 * @brief vulkan context
*/
RenderEngine::Rendering::VulkanContext vulkanContext;

/**
 * @brief device used to render scene
*/
IDeviceContext* deviceContext;

/**
 * @brief render context to render scene
*/
IRenderContext* renderContext;

SceneManager* sceneManager;

ResourceManager* resourceManager;

using namespace RenderEngine;
using namespace RenderEngine::Rendering;
using namespace RenderEngine::Window;

void InitWindow()
{
    std::cout << "[Initialize] Window" << std::endl;
    window = new GLFW::Window(1024, 720, "Render Engine");
}

std::string UserSelectPhysicalDevice(std::vector<std::string> _physicalDevicesNames)
{
    size_t size = _physicalDevicesNames.size();


    if (size == 0)
        throw std::runtime_error("No GPU listed");

    else if (size == 1)
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

void InitEngine()
{
    std::cout << "[Initialize] Vulkan" << std::endl;
    IEngineInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.applicationName = "RenderEngine";
    instanceCreateInfo.applicationVersion = Mathlib::Vec3(1.f, 0.f, 0.f);
    instanceCreateInfo.engineName = "VulkanRenderer";
    instanceCreateInfo.engineVersion = Mathlib::Vec3(1.f, 0.f, 0.f);
    vulkanContext.InitializeInstance(instanceCreateInfo);

    IDeviceContextCreateInfo deviceCreateInfo{};
    deviceCreateInfo.window = window;
    deviceContext = vulkanContext.CreateDeviceContext(deviceCreateInfo);
    std::vector<std::string> deviceNames = deviceContext->QueryAvailblePhysicalDevices();
    deviceContext->InitializeDeviceContext(UserSelectPhysicalDevice(deviceNames));

    SwapChainCommandBufferCreateInfo CBCreateInfo{};
    CBCreateInfo.customViewport = false;
    CBCreateInfo.customScissor = false;

    IRenderContextCreateInfo renderContextCreateInfo{};
    renderContextCreateInfo.swapChainCommandBufferCreateInfo = &CBCreateInfo;
    renderContext = deviceContext->CreateRenderContext(renderContextCreateInfo);
}

Scene* SetupSimpleCubeScene()
{
    Scene* scene = sceneManager->AddScene();
    scene->name = "test_scene_simple_plane";

    Camera* camera = scene->GetCamera();
    Mathlib::Transform cameraTransform;
    cameraTransform.position = Mathlib::Vec3(0.0f, 0.0f, -1.0f);
    cameraTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, 0.f));
    camera->SetLocalTransform(cameraTransform);
    camera->fov = 90.f;

    CameraController* cameraController = camera->AddComponent<CameraController>();
    cameraController->window = window;

    Mathlib::Transform objTransform;
    objTransform.position = Mathlib::Vec3(0.f, 0.f, 2.0f);
    objTransform.scale = Mathlib::Vec3(2.f, 1.0f, 1.f);
    objTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, 0.f));

    GameObjectCreateInfo objCreateinfo;
    objCreateinfo.transform = objTransform;
    objCreateinfo.parent = nullptr;
    objCreateinfo.name = "cube";

    GameObject* obj = scene->AddGameObject(objCreateinfo);

    Shader* fragShader = resourceManager->LoadShader("Resources/Engine/Shaders/BlinnPhongFragmentShader.frag.spv", FRAGMENT);
    Texture* wallTexture = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Brick/bricks.jpg");
    Texture* wallMetalnessMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Wall/metallic.png");
    Texture* wallRoughnessMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Wall/roughness.png");
    Texture* wallNormalMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Brick/bricks_normal.jpg");
    Texture* wallAoMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Wall/ao.png");

    MeshRenderer* objMeshRenderer = obj->GetComponent<MeshRenderer>();
    objMeshRenderer->fragmentShader = fragShader;
    objMeshRenderer->material.texture = wallTexture;
    objMeshRenderer->material.metalnessMap = wallMetalnessMap;
    objMeshRenderer->material.roughnessMap = wallRoughnessMap;
    objMeshRenderer->material.normalMap = wallNormalMap;
    objMeshRenderer->material.ambientOcclusionMap = wallAoMap;
    objMeshRenderer->material.ambient = Mathlib::Vec4(0.1f, 0.1f, 0.1f, 1.f);
    objMeshRenderer->material.diffuse = Mathlib::Vec4(0.4f, 0.4f, 0.4f, 1.f);
    objMeshRenderer->material.specular = Mathlib::Vec4(0.8f, 0.8f, 0.8f, 1.f);
    objMeshRenderer->material.shininess = 16.0f;

    RotatorComponent* rotator = obj->AddComponent<RotatorComponent>();
    rotator->rotationAxis = ROTATION_AXIS::Y;

    Mathlib::Transform lightTransform;
    lightTransform.position = Mathlib::Vec3(0.f, 0.0f, 0.f);
    lightTransform.scale = Mathlib::Vec3(0.1f, 0.1f, 0.1f);
    lightTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, 0.f));

    GameObjectCreateInfo lightCreateInfo;
    lightCreateInfo.transform = lightTransform;
    lightCreateInfo.parent = nullptr;
    lightCreateInfo.name = "light_1";

    GameObject* light1 = scene->AddGameObject(lightCreateInfo);

    MeshRenderer* lightMeshRenderer = light1->GetComponent<MeshRenderer>();
    lightMeshRenderer->fragmentShader = resourceManager->LoadShader("Resources/Engine/Shaders/TextureFragmentShader.frag.spv", FRAGMENT);

    PointLight* lightComponent = light1->AddComponent<PointLight>();
    lightComponent->color = Mathlib::Vec3(1.f, 1.0f, 1.0f);
    lightComponent->intensity = 5.f;
    lightComponent->range = 10.f;

    return scene;
}

void MainLoop()
{
    Scene* scene = SetupSimpleCubeScene();

    scene->Initialize();
    scene->Start();

    while (!window->WindowShouldClose()) 
    {
        window->Update();
        renderContext->DrawScene(scene);
        scene->Update();
    }
    deviceContext->WaitDeviceIdle();
}

void Cleanup()
{
    delete sceneManager;
    resourceManager->Clean();
    delete resourceManager;
    vulkanContext.Cleanup();
    window->Cleanup();
    delete window;
}

void Run()
{
    InitWindow();
    InitEngine();
    resourceManager = new ResourceManager(renderContext);
    sceneManager = new SceneManager(resourceManager);
    MainLoop();
    Cleanup();
}


int main() 
{
    try {
        Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
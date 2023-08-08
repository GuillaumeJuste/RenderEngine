#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "Rendering/Vulkan/Context/VulkanContext/VulkanContext.hpp"
#include "Window/Glfw/Window.hpp"
#include "SceneGraph/Scene/SceneManager.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "SceneGraph/Object/GameObject/GameObject.hpp"
#include "Components/MeshRenderer/MeshRenderer.hpp"
#include "Components/Light/PointLight.hpp"
#include "Components/Light/DirectionalLight.hpp"
#include "Components/Light/SpotLight.hpp"
#include "Components/CameraController/CameraController.hpp"
#include "Component/RotatorComponent.hpp"
#include <iostream>

#include<Mathlib/Mathlib/Include/Misc/Math.hpp>

using namespace RenderEngine;
using namespace RenderEngine::Rendering;
using namespace RenderEngine::SceneGraph;
using namespace Loader;

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

/// initialize Window
void InitWindow()
{
    std::cout << "[Initialize] Window" << std::endl;
    window = new GLFW::Window(1024, 720, "Render Engine");
}

/**
 * @brief Select GPU used for rendering if multiple are available
 * @param _physicalDevicesNames list of the GPUs names
 * @return selected GPU name
*/
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

/// intialize vulkan rendering context
void InitEngine()
{
    // Initialize Vulkan instance
    IEngineInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.applicationName = "RenderEngine";
    instanceCreateInfo.applicationVersion = Mathlib::Vec3(1.f, 0.f, 0.f);
    instanceCreateInfo.engineName = "VulkanRenderer";
    instanceCreateInfo.engineVersion = Mathlib::Vec3(1.f, 0.f, 0.f);
    vulkanContext.InitializeInstance(instanceCreateInfo);

    // Initialize device instance
    IDeviceContextCreateInfo deviceCreateInfo{};
    deviceCreateInfo.window = window;
    deviceContext = vulkanContext.CreateDeviceContext(deviceCreateInfo);

    // select GPU used by the device instance
    std::vector<std::string> deviceNames = deviceContext->QueryAvailblePhysicalDevices();
    deviceContext->InitializeDeviceContext(UserSelectPhysicalDevice(deviceNames));

    // Initialize rendering context
    SwapChainCommandBufferCreateInfo CBCreateInfo{};
    CBCreateInfo.customViewport = false;
    CBCreateInfo.customScissor = false;

    IRenderContextCreateInfo renderContextCreateInfo{};
    renderContextCreateInfo.swapChainCommandBufferCreateInfo = &CBCreateInfo;
    renderContext = deviceContext->CreateRenderContext(renderContextCreateInfo);
}

/// setup scene
Scene* SetupSimpleCubeScene()
{
    // create the scene
    Scene* scene = sceneManager->AddScene();
    scene->name = "test_scene_simple_plane";

    // setup skybox
    scene->skybox.mesh = resourceManager->LoadMesh("Resources/Engine/Models/cube.obj");
    scene->skybox.BRDFlut = resourceManager->LoadTexture("Resources/Engine/Textures/default_brdf_lut.png");
    scene->skybox.vertexShader = resourceManager->LoadShader("Resources/Engine/Shaders/Skybox.vert.spv", VERTEX);
    scene->skybox.vertexShaderDescriptorSet = ShaderDescriptorSet::GenerateSkyboxVertexShaderDescriptor();
    scene->skybox.fragmentShader = resourceManager->LoadShader("Resources/Engine/Shaders/Skybox.frag.spv", FRAGMENT);
    scene->skybox.fragmentShaderDescriptorSet = ShaderDescriptorSet::GenerateSkyboxFragmentShaderDescriptor();
    scene->skybox.cubemap = resourceManager->LoadAsset("Resources/Engine/Textures/HDR/newport_loftCubemap.asset");
    scene->skybox.irradianceMap = resourceManager->LoadAsset("Resources/Engine/Textures/HDR/newport_loftIrradiance.asset");
    scene->skybox.prefilterMap = resourceManager->LoadAsset("Resources/Engine/Textures/HDR/newport_loftPrefiltered.asset");

    // set camera initial position and rotation
    Camera* camera = scene->GetCamera();
    Mathlib::Transform cameraTransform;
    cameraTransform.position = Mathlib::Vec3(0.0f, 0.0f, -1.0f);
    cameraTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, 0.f));
    camera->SetLocalTransform(cameraTransform);
    camera->fov = 90.f;

    // add camera controller to the camera
    // input are rebindable
    RenderEngine::Component::CameraController* cameraController = camera->AddComponent<RenderEngine::Component::CameraController>();
    cameraController->window = window;

    // Add a GameObject to the scene
    // create transform
    Mathlib::Transform objTransform;
    objTransform.position = Mathlib::Vec3(0.f, 0.f, 2.0f);
    objTransform.scale = Mathlib::Vec3(2.f, 1.0f, 1.f);
    objTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, 0.f));

    // set creation parameters
    GameObjectCreateInfo objCreateinfo;
    objCreateinfo.transform = objTransform;
    objCreateinfo.parent = nullptr;
    objCreateinfo.name = "cube";

    // Add object to the scene
    GameObject* obj = scene->AddGameObject(objCreateinfo);

    // load assets used for rendering
    Shader* vertexShader = resourceManager->LoadShader("Resources/Engine/Shaders/VertexShader.vert.spv", VERTEX);
    Shader* fragShader = resourceManager->LoadShader("Resources/Engine/Shaders/BlinnPhongFragmentShader.frag.spv", FRAGMENT);
    Mesh* mesh = resourceManager->LoadMesh("Resources/Sample/SceneCreation/Models/cube.obj");
    Texture* wallTexture = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Brick/bricks.jpg");
    Texture* wallMetalnessMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Wall/metallic.png");
    Texture* wallRoughnessMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Wall/roughness.png");
    Texture* wallNormalMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Brick/bricks_normal.jpg");
    Texture* wallAoMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Wall/ao.png");

    // Add & set a MeshRenderer to the GameObject
    RenderEngine::Component::MeshRenderer* objMeshRenderer = obj->AddComponent<RenderEngine::Component::MeshRenderer>();
    objMeshRenderer->mesh = mesh;
    objMeshRenderer->material.vertexShader = vertexShader;
    objMeshRenderer->material.vertexShaderDescriptorSet = ShaderDescriptorSet::GenerateDefaultVertexShaderDescriptor();
    objMeshRenderer->material.fragmentShader = fragShader;
    objMeshRenderer->material.fragmentShaderDescriptorSet = ShaderDescriptorSet::GenerateBlinnPhongFragmentShaderDescriptor();
    objMeshRenderer->material.albedo = wallTexture;
    objMeshRenderer->material.metalnessMap = wallMetalnessMap;
    objMeshRenderer->material.roughnessMap = wallRoughnessMap;
    objMeshRenderer->material.normalMap = wallNormalMap;
    objMeshRenderer->material.ambientOcclusionMap = wallAoMap;
    objMeshRenderer->material.ambient = Mathlib::Vec4(0.1f, 0.1f, 0.1f, 1.f);
    objMeshRenderer->material.diffuse = Mathlib::Vec4(0.4f, 0.4f, 0.4f, 1.f);
    objMeshRenderer->material.specular = Mathlib::Vec4(0.8f, 0.8f, 0.8f, 1.f);
    objMeshRenderer->material.shininess = 16.0f;

    // add a rotator component 
    RotatorComponent* rotator = obj->AddComponent<RotatorComponent>();
    rotator->rotationAxis = ROTATION_AXIS::Y;

    // Add a light (A GameObject with a light component)
    // lights type available: point light, directional light and spot lights
    Mathlib::Transform lightTransform;
    lightTransform.position = Mathlib::Vec3(0.f, 0.0f, 0.f);
    lightTransform.scale = Mathlib::Vec3(0.1f, 0.1f, 0.1f);
    lightTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, 0.f));

    GameObjectCreateInfo lightCreateInfo;
    lightCreateInfo.transform = lightTransform;
    lightCreateInfo.parent = nullptr;
    lightCreateInfo.name = "light_1";

    GameObject* light1 = scene->AddGameObject(lightCreateInfo);

    RenderEngine::Component::MeshRenderer* lightMeshRenderer = light1->AddComponent<RenderEngine::Component::MeshRenderer>();
    lightMeshRenderer->mesh = mesh;
    lightMeshRenderer->material.vertexShader = vertexShader;
    lightMeshRenderer->material.vertexShaderDescriptorSet = ShaderDescriptorSet::GenerateDefaultVertexShaderDescriptor();
    lightMeshRenderer->material.fragmentShader = resourceManager->LoadShader("Resources/Engine/Shaders/TextureFragmentShader.frag.spv", FRAGMENT);
    lightMeshRenderer->material.fragmentShaderDescriptorSet = ShaderDescriptorSet::GenerateDefaultFragmentShaderDescriptor();
    lightMeshRenderer->material.albedo = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/white.jpg");
    lightMeshRenderer->material.metalnessMap = lightMeshRenderer->material.albedo;
    lightMeshRenderer->material.roughnessMap = lightMeshRenderer->material.albedo;
    lightMeshRenderer->material.normalMap = lightMeshRenderer->material.albedo;
    lightMeshRenderer->material.ambientOcclusionMap = lightMeshRenderer->material.albedo;

    RenderEngine::Component::PointLight* lightComponent = light1->AddComponent<RenderEngine::Component::PointLight>();
    lightComponent->color = Mathlib::Vec3(1.f, 1.0f, 1.0f);
    lightComponent->intensity = 5.f;
    lightComponent->range = 10.f;

    return scene;
}

// Engine main loop
void MainLoop()
{
    // create and intilize the scene
    Scene* scene = SetupSimpleCubeScene();

    scene->Initialize();
    scene->Start();

    // loop
    while (!window->WindowShouldClose() && !window->CheckKeyStatus(RenderEngine::Utils::Input::KEY_ESCAPE, RenderEngine::Utils::InputStatus::PRESS))
    {
        window->Update();
        renderContext->DrawScene(scene);
        scene->Update();
    }
    deviceContext->WaitDeviceIdle();
}

// clean used resources
void Cleanup()
{
    sceneManager->Cleanup();
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
    sceneManager = new SceneManager();
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
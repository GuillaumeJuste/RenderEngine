#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "Rendering/Vulkan/Context/VulkanContext/VulkanContext.hpp"
#include "Window/Glfw/Window.hpp"
#include "SceneGraph/Scene/SceneManager.hpp"
#include "Rendering/Base/ResourceManager/ResourceManager.hpp"
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

Scene* SetupPBRScene()
{
    Scene* scene = sceneManager->AddScene();
    scene->name = "test_scene_simple_cube";

    scene->skybox.mesh = resourceManager->LoadMesh("Resources/Engine/Models/cube.obj");
    scene->skybox.BRDFlut = resourceManager->LoadTexture("Resources/Engine/Textures/default_brdf_lut.png", TextureFormat::RGBA);
    scene->skybox.vertexShader = resourceManager->LoadShader("Resources/Engine/Shaders/GLSL/Skybox.vert.spv", VERTEX);
    scene->skybox.vertexShaderDescriptorSet = ShaderDescriptorSet::GenerateSkyboxVertexShaderDescriptor();
    scene->skybox.fragmentShader = resourceManager->LoadShader("Resources/Engine/Shaders/GLSL/Skybox.frag.spv", FRAGMENT);
    scene->skybox.fragmentShaderDescriptorSet = ShaderDescriptorSet::GenerateSkyboxFragmentShaderDescriptor();
    scene->skybox.cubemap = resourceManager->LoadAsset("Resources/Engine/Textures/HDR/newport_loftCubemap.asset");
    scene->skybox.irradianceMap = resourceManager->LoadAsset("Resources/Engine/Textures/HDR/newport_loftIrradiance.asset");
    scene->skybox.prefilterMap = resourceManager->LoadAsset("Resources/Engine/Textures/HDR/newport_loftPrefiltered.asset");

    /*Texture* skyboxTexture = resourceManager->LoadTexture("Resources/Engine/Textures/HDR/newport_loft.hdr", TextureFormat::HDR_RGBA, false);
    resourceManager->CreateSkyboxFromTexture(skyboxTexture, Mathlib::Vec2(512.f, 512.f), &scene->skybox);

    resourceManager->SaveAsset(scene->skybox.cubemap);
    resourceManager->SaveAsset(scene->skybox.irradianceMap);
    resourceManager->SaveAsset(scene->skybox.prefilterMap);*/
    
    Camera* camera = scene->GetCamera();
    Mathlib::Transform cameraTransform;
    cameraTransform.position = Mathlib::Vec3(0.0f, 0.0f, -2.f);
    cameraTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, 0.f));
    camera->SetLocalTransform(cameraTransform);
    camera->fov = 90.f;

    /*RenderEngine::Component::CameraController* cameraController = camera->AddComponent<RenderEngine::Component::CameraController>();
    cameraController->window = window;*/

    Mesh* sphere = resourceManager->LoadMesh("Resources/Sample/ScenePBR/Models/Sphere.obj");
    Shader* vertexShader = resourceManager->LoadShader("Resources/Engine/Shaders/HLSL/VertexShader.hlsl", VERTEX);
    Shader* fragShader = resourceManager->LoadShader("Resources/Engine/Shaders/HLSL/PBRFragmentShader.hlsl", FRAGMENT);

    /*Shader* vertexShader = resourceManager->LoadShader("Resources/Engine/Shaders/GLSL/VertexShader.vert.spv", VERTEX);
    Shader* fragShader = resourceManager->LoadShader("Resources/Engine/Shaders/GLSL/PBRFragmentShader.frag.spv", FRAGMENT);*/

    Texture* wallTexture = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/Wall/albedo.png", TextureFormat::RGBA);
    Texture* wallMetalnessMap = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/Wall/metallic.png", TextureFormat::RGBA);
    Texture* wallRoughnessMap = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/Wall/roughness.png", TextureFormat::RGBA);
    Texture* wallNormalMap = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/Wall/normal.png", TextureFormat::RGBA);
    Texture* wallAoMap = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/Wall/ao.png", TextureFormat::RGBA);

    /*Sphere 1*/
    Mathlib::Transform transform;
    transform.position = Mathlib::Vec3(-1.f, 0.f, 0.0f);
    transform.scale = Mathlib::Vec3(0.015f, 0.015f, 0.015f);

    GameObjectCreateInfo createinfo;
    createinfo.transform = transform;
    createinfo.parent = nullptr;
    createinfo.name = "sphere_1";

    GameObject* obj = scene->AddGameObject(createinfo);

    RenderEngine::Component::MeshRenderer* meshRenderer = obj->AddComponent<RenderEngine::Component::MeshRenderer>();
    meshRenderer->mesh = sphere;
    meshRenderer->material.vertexShader = vertexShader;
    meshRenderer->material.vertexShaderDescriptorSet = ShaderDescriptorSet::GenerateDefaultVertexShaderDescriptor();
    meshRenderer->material.fragmentShader = fragShader;
    meshRenderer->material.fragmentShaderDescriptorSet = ShaderDescriptorSet::GeneratePBRFragmentShaderDescriptor();
    meshRenderer->material.albedo = wallTexture;
    meshRenderer->material.metalnessMap = wallMetalnessMap;
    meshRenderer->material.roughnessMap = wallRoughnessMap;
    meshRenderer->material.normalMap = wallNormalMap;
    meshRenderer->material.ambientOcclusionMap = wallAoMap;
    meshRenderer->material.shininess = 32.0f;
    meshRenderer->material.ambient = Mathlib::Vec4(0.1f, 0.1f, 0.1f, 1.f);
    meshRenderer->material.diffuse = Mathlib::Vec4(0.4f, 0.4f, 0.4f, 1.f);
    meshRenderer->material.specular = Mathlib::Vec4(0.8f, 0.8f, 0.8f, 1.f);

    RotatorComponent* rotator = obj->AddComponent<RotatorComponent>();
    rotator->rotationAxis = ROTATION_AXIS::Y;

    /*Sphere 2*/

    Mathlib::Transform transform2;
    transform2.position = Mathlib::Vec3(0.f, 0.f, 0.0f);
    transform2.scale = Mathlib::Vec3(0.015f, 0.015f, 0.015f);

    GameObjectCreateInfo createinfo2;
    createinfo2.transform = transform2;
    createinfo2.parent = nullptr;
    createinfo2.name = "sphere_2";

    GameObject* obj2 = scene->AddGameObject(createinfo2);

    Texture* ironTexture = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/Rusted_iron/albedo.png", TextureFormat::RGBA);
    Texture* ironMetalnessMap = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/Rusted_iron/metallic.png", TextureFormat::RGBA);
    Texture* ironRoughnessMap = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/Rusted_iron/roughness.png", TextureFormat::RGBA);
    Texture* ironNormalMap = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/Rusted_iron/normal.png", TextureFormat::RGBA);
    Texture* ironAoMap = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/Rusted_iron/ao.png", TextureFormat::RGBA);

    RenderEngine::Component::MeshRenderer* meshRenderer2 = obj2->AddComponent<RenderEngine::Component::MeshRenderer>();
    meshRenderer2->mesh = sphere;
    meshRenderer2->material.vertexShader = vertexShader;
    meshRenderer2->material.vertexShaderDescriptorSet = ShaderDescriptorSet::GenerateDefaultVertexShaderDescriptor();
    meshRenderer2->material.fragmentShader = fragShader;
    meshRenderer2->material.fragmentShaderDescriptorSet = ShaderDescriptorSet::GeneratePBRFragmentShaderDescriptor();
    meshRenderer2->material.albedo = ironTexture;
    meshRenderer2->material.metalnessMap = ironMetalnessMap;
    meshRenderer2->material.roughnessMap = ironRoughnessMap;
    meshRenderer2->material.normalMap = ironNormalMap;
    meshRenderer2->material.ambientOcclusionMap = ironAoMap;
    meshRenderer2->material.shininess = 32.0f;
    meshRenderer2->material.ambient = Mathlib::Vec3(0.1f, 0.1f, 0.1f);
    meshRenderer2->material.diffuse = Mathlib::Vec3(0.5f, 0.5f, 0.5f);
    meshRenderer2->material.specular = Mathlib::Vec3(1.0f, 1.0f, 1.0f);

    RotatorComponent* rotator2 = obj2->AddComponent<RotatorComponent>();
    rotator2->rotationAxis = ROTATION_AXIS::Y;

    /*Sphere 3*/

    Mathlib::Transform transform3;
    transform3.position = Mathlib::Vec3(1.f, 0.f, 0.0f);
    transform3.scale = Mathlib::Vec3(0.015f, 0.015f, 0.015f);

    GameObjectCreateInfo createinfo3;
    createinfo3.transform = transform3;
    createinfo3.parent = nullptr;
    createinfo3.name = "sphere_3";

    GameObject* obj3 = scene->AddGameObject(createinfo3);

    Texture* goldTexture = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/Gold/albedo.png", TextureFormat::RGBA);
    Texture* goldMetalnessMap = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/Gold/metallic.png", TextureFormat::RGBA);
    Texture* goldRoughnessMap = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/Gold/roughness.png", TextureFormat::RGBA);
    Texture* goldNormalMap = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/Gold/normal.png", TextureFormat::RGBA);
    Texture* goldAoMap = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/Gold/ao.png", TextureFormat::RGBA);

    RenderEngine::Component::MeshRenderer* meshRenderer3 = obj3->AddComponent<RenderEngine::Component::MeshRenderer>();
    meshRenderer3->mesh = sphere;
    meshRenderer3->material.vertexShader = vertexShader;
    meshRenderer3->material.vertexShaderDescriptorSet = ShaderDescriptorSet::GenerateDefaultVertexShaderDescriptor();
    meshRenderer3->material.fragmentShader = fragShader;
    meshRenderer3->material.fragmentShaderDescriptorSet = ShaderDescriptorSet::GeneratePBRFragmentShaderDescriptor();
    meshRenderer3->material.albedo = goldTexture;
    meshRenderer3->material.metalnessMap = goldMetalnessMap;
    meshRenderer3->material.roughnessMap = goldRoughnessMap;
    meshRenderer3->material.normalMap = goldNormalMap;
    meshRenderer3->material.ambientOcclusionMap = goldAoMap;
    
    meshRenderer3->material.shininess = 32.0f;
    meshRenderer3->material.ambient = Mathlib::Vec3(0.1f, 0.1f, 0.1f);
    meshRenderer3->material.diffuse = Mathlib::Vec3(0.5f, 0.5f, 0.5f);
    meshRenderer3->material.specular = Mathlib::Vec3(1.0f, 1.0f, 1.0f);

    RotatorComponent* rotator3 = obj3->AddComponent<RotatorComponent>();
    rotator3->rotationAxis = ROTATION_AXIS::Y;

    /*light 1*/
    Mathlib::Transform transform4;
    transform4.position = Mathlib::Vec3(0.f, 0.0f, -1.f);
    transform4.scale = Mathlib::Vec3(0.1f, 0.1f, 0.1f);

    GameObjectCreateInfo createinfo4;
    createinfo4.transform = transform4;
    createinfo4.parent = nullptr;
    createinfo4.name = "light_4";

    GameObject* light4 = scene->AddGameObject(createinfo4);

    RenderEngine::Component::PointLight* lightComponent4 = light4->AddComponent<RenderEngine::Component::PointLight>();
    lightComponent4->color = Mathlib::Vec3(1.0f, 1.0f, 1.f);
    lightComponent4->range = 30.f;
    lightComponent4->intensity = 1.f;

    RenderEngine::Component::MeshRenderer* meshRenderer4 = light4->AddComponent<RenderEngine::Component::MeshRenderer>();
    meshRenderer4->mesh = resourceManager->LoadMesh("Resources/Sample/ScenePBR/Models/cube.obj");
    meshRenderer4->material.vertexShader = vertexShader;
    meshRenderer4->material.vertexShaderDescriptorSet = ShaderDescriptorSet::GenerateDefaultVertexShaderDescriptor();
    meshRenderer4->material.fragmentShader = resourceManager->LoadShader("Resources/Engine/Shaders/GLSL/TextureFragmentShader.frag.spv", FRAGMENT);
    meshRenderer4->material.fragmentShaderDescriptorSet = ShaderDescriptorSet::GenerateDefaultFragmentShaderDescriptor();
    meshRenderer4->material.albedo = resourceManager->LoadTexture("Resources/Sample/ScenePBR/Textures/white.jpg", TextureFormat::RGBA);
    meshRenderer4->material.metalnessMap = meshRenderer4->material.albedo;
    meshRenderer4->material.roughnessMap = meshRenderer4->material.albedo;
    meshRenderer4->material.normalMap = meshRenderer4->material.albedo;
    meshRenderer4->material.ambientOcclusionMap = meshRenderer4->material.albedo;

    meshRenderer4->material.frontFace = FrontFace::COUNTER_CLOCKWISE;
    meshRenderer4->enable = false;

    return scene;
}

void MainLoop()
{
    Scene* scene = SetupPBRScene();

    scene->Initialize();
    scene->Start();

    while (!window->WindowShouldClose() && !window->CheckKeyStatus(RenderEngine::Utils::Input::KEY_ESCAPE, RenderEngine::Utils::InputStatus::PRESS))
    {
        window->Update();
        renderContext->DrawScene(scene);
        scene->Update();
    }
    deviceContext->WaitDeviceIdle();
}

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
    resourceManager = renderContext->CreateResourceManager();
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
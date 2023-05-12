#pragma once

#ifndef RENDERENGINE_SCENEMANAGER
#define RENDERENGINE_SCENEMANAGER

#include "SceneGraph/Scene/Scene.hpp"

#include <forward_list>

namespace RenderEngine
{
    class ResourceManager;
}

namespace RenderEngine::SceneGraph
{
    /**
     * @brief SceneManager class
    */
    class SceneManager
    {
    private:
        RenderEngine::ResourceManager* resourceManager;

        std::forward_list<Scene> scenes;

    public:
        SceneManager(RenderEngine::ResourceManager* _resourceManager);
        ~SceneManager() = default;

        Scene* AddScene();

        Scene* GetScene(std::string _name);

        bool DeleteScene(Scene* _scene);

        void Cleanup();
    };
}

#endif
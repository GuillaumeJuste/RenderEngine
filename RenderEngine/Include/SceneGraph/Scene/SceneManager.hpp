#pragma once

#ifndef RENDERENGINE_SCENEMANAGER
#define RENDERENGINE_SCENEMANAGER

#include "SceneGraph/Scene/Scene.hpp"

#include <forward_list>

namespace RenderEngine::SceneGraph
{
    /**
     * @brief SceneManager class
    */
    class SceneManager
    {
    private:

        std::forward_list<Scene> scenes;

    public:
        SceneManager() = default;
        ~SceneManager() = default;

        Scene* AddScene();

        Scene* GetScene(std::string _name);

        bool DeleteScene(Scene* _scene);

        void Cleanup();
    };
}

#endif
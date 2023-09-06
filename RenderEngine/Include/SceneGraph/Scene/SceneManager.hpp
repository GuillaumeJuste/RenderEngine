#pragma once

#ifndef RENDERENGINE_SCENEMANAGER
#define RENDERENGINE_SCENEMANAGER

#include "SceneGraph/Scene/Scene.hpp"

#include <forward_list>

namespace RenderEngine::SceneGraph
{
    /**
     * @brief SceneManager class implementation
    */
    class SceneManager
    {
    private:
        /**
         * @brief list of created scenes
        */
        std::forward_list<Scene> scenes;

    public:
        /// default constructor
        SceneManager() = default;

        /// default destructor
        ~SceneManager() = default;

        /**
         * @brief Add a new scene to the scene manager
         * @return scene newly created
        */
        Scene* AddScene();

        /**
         * @brief Get a scene from it's name
         * @param _name name of the scene to fine
         * @return scene matching the name
        */
        Scene* GetScene(std::string _name);

        /**
         * @brief delete a scene from the manager
         * @param _scene scene to delete
         * @return true if the scene was deleted
        */
        bool DeleteScene(Scene* _scene);

        /**
         * @brief Delete all the scenes from the manager
        */
        void Cleanup();
    };
}

#endif
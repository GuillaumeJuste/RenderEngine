#pragma once

#ifndef RENDERENGINE_SCENE
#define RENDERENGINE_SCENE

#include "SceneGraph/Object/GameObject/GameObject.hpp"
#include "SceneGraph/Object/GameObject/GameObjectCreateInfo.hpp"
#include "SceneGraph/Object/GameObject/Camera/Camera.hpp"
#include "SceneGraph/Scene/Skybox.hpp"

#include "Utils/Time/Time.hpp"
#include "Utils/Event/Event.hpp"

#include <string>
#include <list>

namespace RenderEngine::SceneGraph
{
    /**
     * @brief Engine scene class
    */
    class Scene
    {
    private:
        /**
         * @brief list of objects present in the scene
        */
        std::list<GameObject> gameObjects;

        /**
         * @brief root object in the scene
        */
        GameObject rootObject;

        /**
         * @brief scene main camera
        */
        Camera mainCamera;

        /**
         * @brief scene internal timer used for delta time computation
        */
        RenderEngine::Utils::Time timer;

        /**
         * @brief scene fixed update
         * called by the timer on fixed update time and call all game objects fixed update
         * @param _deltaTime fixed delta time
        */
        void FixedUpdate(double _deltaTime);

    public:
        /**
        * @brief scene name
        */
        std::string name;

        /**
         * @brief scene skybox;
        */
        Skybox skybox;

        /**
         * @brief scene fixed update time step
        */
        float fixedTimestep = 0.02f;

        /**
         * @brief event called when a gameObject is added to the scene
        */
        RenderEngine::Utils::Event<GameObject*> GameObjectAdded;

        /**
         * @brief event called when a gameObject is removed from the scene
        */
        RenderEngine::Utils::Event<GameObject*> GameObjectRemoved;

        /// constructor
        Scene();

        /// default copy constructor
        Scene(const Scene&) = default;

        /// destructor
        ~Scene();

        /**
         * @brief initialize scene game objects
        */
        void Initialize();

        /**
         * @brief Start game objects and timer
        */
        void Start();

        /**
         * @brief Update the scene
        */
        void Update();

        /**
         * @brief Add gameobject  to the scene
         * @param _createInfo creation information of the object to add to the scene
         * @return pointer on created Game Object
        */
        GameObject* AddGameObject(GameObjectCreateInfo _createInfo);

        /**
         * @brief Remove Game Object from the scene
         * @param _gao pointer to the Game Object to remove
         * @return true if Game Object was removed
        */
        bool RemoveGameObject(GameObject* _gao);

        /**
         * @brief Get Game Object present in the scene by name 
         * @param _name name of the Game Object to get
         * @return pointer GameObject found or nullptr if no match
        */
        const GameObject* GetGameObjectByName(std::string _name);

        /**
         * @brief Get Game Object present in the scene by ID 
         * @param _id id of the Game Object to get
         * @return pointer GameObject found or nullptr if no match
        */
        const GameObject* GetGameObjectByID(unsigned int _id);

        /**
         * @brief Get scene root Game Object
         * @return root GameObject
        */
        const GameObject& GetSceneRoot();

        /**
         * @brief Get a pointer on the scene main camera
         * @return pointer to the camera
        */
        Camera* GetCamera();

        /**
         * @brief Clear the scene
        */
        void Cleanup();

        /**
         * @brief compare two scene by name
         * @param _scene scene to compare with
        */
        bool operator==(const Scene& _scene) const;
    };
}

#endif
#pragma once

#ifndef RENDERENGINE_SCENE
#define RENDERENGINE_SCENE

#include "Core/Object/GameObject/GameObject.hpp"
#include "Core/Object/GameObject/GameObjectCreateInfo.hpp"
#include "Core/Scene/SceneCreateInfo.hpp"

#include <string>
#include <forward_list>


namespace RenderEngine::Core
{
    /**
     * @brief Engine scene class
    */
    class Scene
    {
    private:

        /**
         * @brief scene name
        */
        std::string name;
        
        /**
         * @brief list of objects present in the scene
        */
        std::forward_list<GameObject> gameObjects;

        /**
         * @brief root object in the scene
        */
        GameObject rootObject;

    public:
        Scene() = default;
        ~Scene() = default;

        /**
         * @brief Initialize scene from create info
         * @param _createinfo Informations to inititalize scene with
         * @param _output Scene to initalize
        */
        static void InitializeScene(const SceneCreateInfo& _createinfo, Scene* _output);

        void Initilize();
        void Start();
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
        GameObject* GetSceneRoot();

        std::forward_list<GameObject> GetGameObjects() const;

        void Cleanup();
    };
}

#endif
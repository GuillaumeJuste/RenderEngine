#include "SceneGraph/Scene/SceneManager.hpp"
#include "ResourceManager/ResourceManager.hpp"

using namespace RenderEngine::SceneGraph;

SceneManager::SceneManager(RenderEngine::ResourceManager* _resourceManager) :
    resourceManager{ _resourceManager }
{

}

Scene* SceneManager::AddScene()
{
    return &scenes.emplace_front(Scene(resourceManager));
}

Scene* SceneManager::GetScene(std::string _name)
{
    for (std::forward_list<Scene>::iterator it = scenes.begin(); it != scenes.end(); ++it)
    {
        if (it->name == _name)
            return &(*it);
    }
    return nullptr;
}

bool SceneManager::DeleteScene(Scene* _scene) 
{
    for (std::forward_list<Scene>::iterator it = scenes.begin(); it != scenes.end(); ++it)
    {
        if (&(*it) == _scene)
        {
            it->Cleanup();
            scenes.remove((*it));
            return true;
        }
    }
    return false;
}

void SceneManager::Cleanup()
{
    for (std::forward_list<Scene>::iterator it = scenes.begin(); it != scenes.end(); ++it)
    {
        (*it).Cleanup();
    }
}
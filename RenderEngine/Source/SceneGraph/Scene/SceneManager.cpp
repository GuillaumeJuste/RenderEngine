#include "SceneGraph/Scene/SceneManager.hpp"

using namespace RenderEngine::SceneGraph;

Scene* SceneManager::AddScene()
{
    return &scenes.emplace_front();
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
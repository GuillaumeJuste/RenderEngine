#pragma once

#ifndef RENDERENGINE_GAMEOBJECTCREATEINFO
#define RENDERENGINE_GAMEOBJECTCREATEINFO

#include "Core/Object/Mesh/Mesh.hpp"
#include "Transform/Transform.hpp"


namespace RenderEngine::Core
{
	class GameObject;

	struct GameObjectCreateInfo
	{
		std::string name;

		Mesh* mesh = nullptr;

		GameObject* parent = nullptr;

		Mathlib::Transform transform;

		GameObjectCreateInfo() = default;
	};
}

#endif
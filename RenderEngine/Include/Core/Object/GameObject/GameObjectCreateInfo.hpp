#pragma once

#ifndef RENDERENGINE_GAMEOBJECTCREATEINFO
#define RENDERENGINE_GAMEOBJECTCREATEINFO

#include "Core/Object/Object.hpp"
#include "Core/Object/Mesh/Mesh.hpp"
#include "Transform/Transform.hpp"


namespace RenderEngine::Core
{
	struct GameObjectCreateInfo
	{
		Mesh* mesh = nullptr;

		Object* parent = nullptr;

		Mathlib::Transform transform;

		GameObjectCreateInfo() = default;
	};
}

#endif
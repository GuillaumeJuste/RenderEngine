#pragma once

#ifndef RENDERENGINE_SCENECREATEINFO
#define RENDERENGINE_SCENECREATEINFO

#include "Core/Object/GameObject/GameObject.hpp"

namespace RenderEngine::Core
{
	struct SceneCreateInfo
	{
		std::string name;

		SceneCreateInfo() = default;
	};
}

#endif
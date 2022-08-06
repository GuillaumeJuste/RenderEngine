#pragma once

#ifndef RENDERENGINE_SCENECREATEINFO
#define RENDERENGINE_SCENECREATEINFO

#include "Core/Object/GameObject/GameObject.hpp"

namespace RenderEngine::Core
{
	/**
	 * @brief Struct holding informations to create a scene
	*/
	struct SceneCreateInfo
	{
		/**
		 * @brief scene name
		*/
		std::string name;

		SceneCreateInfo() = default;
	};
}

#endif
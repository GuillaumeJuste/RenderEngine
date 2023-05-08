#pragma once

#ifndef RENDERENGINE_GAMEOBJECTCREATEINFO
#define RENDERENGINE_GAMEOBJECTCREATEINFO

#include "Transform/Transform.hpp"


namespace RenderEngine::SceneGraph
{
	class GameObject;

	/**
	 * @brief struct storing GameObject creation informations
	*/
	struct GameObjectCreateInfo
	{
		/**
		 * @brief GameObject name
		*/
		std::string name;

		/**
		 * @brief GameObject parent
		*/
		GameObject* parent = nullptr;

		/**
		 * @brief GameObject transform
		*/
		Mathlib::Transform transform;

		GameObjectCreateInfo() = default;
	};
}

#endif
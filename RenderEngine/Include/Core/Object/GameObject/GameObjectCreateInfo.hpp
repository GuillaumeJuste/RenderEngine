#pragma once

#ifndef RENDERENGINE_GAMEOBJECTCREATEINFO
#define RENDERENGINE_GAMEOBJECTCREATEINFO

#include "Core/Object/Mesh/Mesh.hpp"
#include "Transform/Transform.hpp"


namespace RenderEngine::Core
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
		 * @brief GameObject mesh
		*/
		Mesh* mesh = nullptr;

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
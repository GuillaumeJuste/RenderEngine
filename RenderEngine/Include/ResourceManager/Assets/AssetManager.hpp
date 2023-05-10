#pragma once

#ifndef RENDERENGINE_ASSETMANAGER
#define RENDERENGINE_ASSETMANAGER

#include <unordered_map>
#include <string>

namespace RenderEngine::Assets
{
	template <typename T>
	class AssetManager
	{
	private:
		std::unordered_map<std::string, T*> assetsMap;
	public:
		// constructor taking load function on construction
		// copy constructor cleaning unordered map before updating itself

		T* AddAsset(std::string filePath, const T& _asset);
		T* GetAsset(const std::string _filePath) const;
		bool RemoveAsset(const std::string _filePath);
	};
}

#include "ResourceManager/Assets/AssetManager.inl"

#endif
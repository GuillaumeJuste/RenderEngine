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
		AssetManager() = default;
		AssetManager(const AssetManager& _copy) = delete;

		void Add(std::string filePath, T* _asset);
		T* Get(const std::string _filePath) const;
		bool Unload(const std::string _filePath);
		void Clean();
	};
}

#include "ResourceManager/Assets/AssetManager.inl"

#endif
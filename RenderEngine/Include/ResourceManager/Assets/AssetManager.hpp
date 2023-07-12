#pragma once

#ifndef RENDERENGINE_ASSETMANAGER
#define RENDERENGINE_ASSETMANAGER

#include <unordered_map>
#include <string>

namespace RenderEngine::Assets
{
	/**
	 * @brief Asset manager class implementation
	 * @tparam T Type of the asset to manage
	*/
	template <typename T>
	class AssetManager
	{
	private:
		/// held assets map
		std::unordered_map<std::string, T*> assetsMap;
	public:
		/// default constructor
		AssetManager() = default;

		/// default copy constructor
		AssetManager(const AssetManager& _copy) = delete;

		/**
		 * @brief Add asset to the map
		 * @param filePath filepath of the asset
		 * @param _asset asset data
		*/
		void Add(std::string filePath, T* _asset);

		/**
		 * @brief Get asset from its file path
		 * @param _filePath asset filepath
		 * @return asset pointer if found
		*/
		T* Get(const std::string _filePath) const;

		/**
		 * @brief unload asset from the map
		 * @param _filePath file path of the asset  to remove
		 * @return true if asset was removed
		*/
		bool Unload(const std::string _filePath);

		/**
		 * @brief clear all asset contained in the map
		*/
		void Clean();
	};
}

#include "ResourceManager/Assets/AssetManager.inl"

#endif
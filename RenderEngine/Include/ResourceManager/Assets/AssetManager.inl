namespace RenderEngine::Assets
{
	template <typename T>
	T* AssetManager<T>::AddAsset(std::string filePath, const T& _asset)
	{
		T* assetPointer = new T(_asset);
		assetsMap[filePath] = new T(_asset);
		return assetPointer;
	}

	template <typename T>
	T* AssetManager<T>::GetAsset(const std::string _filePath) const
	{
		try {
			return assetsMap.at(_filePath);
		}
		catch (const std::out_of_range& oor) {
			return nullptr;
		}
	}

	template <typename T>
	bool AssetManager<T>::RemoveAsset(const std::string _filePath)
	{
		return assetsMap.erase(_filePath);
	}
}
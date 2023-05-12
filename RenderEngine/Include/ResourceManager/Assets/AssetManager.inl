namespace RenderEngine::Assets
{
	template <typename T>
	void AssetManager<T>::Add(std::string filePath, T* _asset)
	{
		assetsMap[filePath] = _asset;
	}

	template <typename T>
	T* AssetManager<T>::Get(const std::string _filePath) const
	{
		auto it = assetsMap.find(_filePath);
		if (it != assetsMap.end())
		{
			return it->second;
		}
		return nullptr;
	}

	template <typename T>
	bool AssetManager<T>::Unload(const std::string _filePath)
	{
		auto it = assetsMap.find(_filePath);
		if (it != assetsMap.end())
		{	
			delete it->second;
			assetsMap.erase(it);
			return true;
		}
		return false;
	}

	template <typename T>
	void AssetManager<T>::Clean()
	{
		for (auto it = assetsMap.begin(); it != assetsMap.end(); it++)
		{
			delete it->second;
		}
		assetsMap.clear();
	}

}
namespace RenderEngine::Core
{
	template<typename T>
	T* GameObject::AddComponent()
	{
		T* newComponent = new T();
		Component* castedComponent = dynamic_cast<Component*>(newComponent);
		if (castedComponent != nullptr)
		{
			components.push_back(castedComponent);

			return newComponent;
		}
		delete newComponent;
		return nullptr;
	}

	template<typename T>
	T* GameObject::GetComponent()
	{
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		{
			if (dynamic_cast<T*>(*it) != nullptr)
				return dynamic_cast<T*>(*it);
		}
		return nullptr;
	}
}
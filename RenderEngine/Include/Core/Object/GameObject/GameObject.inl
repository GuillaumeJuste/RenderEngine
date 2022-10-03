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

	template<typename T>
	std::vector<T*> GameObject::GetComponents()
	{
		std::vector<T*> matchingComponents;

		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		{
			T* tmp = dynamic_cast<T*>(*it);
			if (tmp != nullptr)
				matchingComponents.push_back(tmp);
		}
		return matchingComponents;
	}

	template<typename T>
	bool GameObject::RemoveComponent(T* _component)
	{
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		{
			if (_component == (*it))
			{
				components.erase(it);
				return true;
			}
		}
		return false;
	}
}
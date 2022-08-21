#include <iostream>

namespace Utils
{
	template <typename... Args>
	Event<Args...>::~Event()
	{
		Clear();
	}

	template <typename... Args>
	void Event<Args...>::Add(void(*_func)(Args...))
	{
		functionHandles.push_back(FunctionHandle<Args...>(_func));
	}

	template <typename... Args>
	template <typename C>
	void Event<Args...>::Add(C* _caller, void(C::* _func)(Args...))
	{
		methodHandles.push_back(MethodHandle<Args...>
		{
			new MethodHandleData<C, Args...>{ _caller, _func },
				MethodHandleData<C, Args...>::InterfaceCall
		});
	}

	template <typename... Args>
	bool Event<Args...>::Remove(void (*_func)(Args...))
	{
		for (auto it = functionHandles.begin(); it != functionHandles.end(); ++it)
		{
			if (it->func == _func)
			{
				functionHandles.erase(it);
				return true;
			}
		}

		return false;
	}

	template <typename... Args>
	template <typename C>
	bool Event<Args...>::Remove(const C* _caller, void(C::* _func)(Args...))
	{
		for (auto it = methodHandles.begin(); it != methodHandles.end(); ++it)
		{
			MethodHandleData<C, Args...>* data = reinterpret_cast<MethodHandleData<C, Args...>*>(it->data);

			if (data->caller == _caller && data->func == _func)
			{
				delete data;

				methodHandles.erase(it);
				return true;
			}
		}

		return false;
	}

	template <typename... Args>
	void Event<Args...>::Execute(const Args&... _args)
	{
		for (auto it = functionHandles.begin(); it != functionHandles.end(); ++it)
		{
			it->func(_args...);
		}

		for (auto it = methodHandles.begin(); it != methodHandles.end(); ++it)
		{
			it->func(it->data, _args...);
		}
	}

	template <typename... Args>
	void Event<Args...>::Clear()
	{
		functionHandles.clear();

		for (auto it = methodHandles.begin(); it != methodHandles.end(); ++it)
		{
			delete it->data;
		}

		methodHandles.clear();
	}

	template <typename... Args>
	void Event<Args...>::operator+=(void(*_func)(Args...))
	{
		Add(_func);
	}

	template <typename... Args>
	bool Event<Args...>::operator-=(void(*_func)(Args...))
	{
		return Remove(_func);
	}

	template <typename... Args>
	void Event<Args...>::operator()(const Args&... _args)
	{
		Execute(_args...);
	}
}
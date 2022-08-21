#pragma once

#ifndef UTILS_EVENT
#define UTILS_EVENT

#include "Utils/Event/MethodHandleData.hpp"

#include <vector>

namespace Utils
{
	/**
	 * @brief Handle struct for a method
	 * @tparam ...Args method's arguments types
	*/
	template <typename... Args>
	struct MethodHandle
	{
		
		/// Abstract method data (used by function interface).
		MethodHandleDataBase* data = nullptr;

		/// Interface function ptr handle.
		void(*func)(void*, Args...) = nullptr;
	};

	/**
	 * @brief Handle struct for a function
	 * @tparam ...Args function's arguments types
	*/
	template <typename... Args>
	struct FunctionHandle
	{
		/// Function ptr handle.
		void(*func)(Args...) = nullptr;
	};

	/**
	 * @brief Event class
	 * @tparam ...Args arguments type
	*/
	template <typename... Args>
	class Event
	{
	private:
		/// vector of functions to call when event is raised
		std::vector<FunctionHandle<Args...>> functionHandles;

		/// vector of methods to call when event is raised
		std::vector<MethodHandle<Args...>> methodHandles;
	public:
		/// Default constructor.
		Event() = default;

		/// Default copy constructor.
		Event(const Event&) = default;
		
		/// Default Move constructor.
		Event(Event&& _other) = default;

		/// Destructor (auto clean).
		~Event();

		/**
		 * @brief Add function to be call on event raised
		 * @param _func function to add
		*/
		void Add(void(*_func)(Args...));

		/**
		 * @brief Add method to be called on event raised
		 * @tparam C caller class type
		 * @param _caller caller instance
		 * @param _func method to add
		*/
		template <typename C>
		void Add(C* _caller, void(C::* _func)(Args...));
		
		/**
		 * @brief Remove function from the list of function to be called on event raised
		 * @param _func function to remove
		 * @return true if function was removed
		*/
		bool Remove(void (*_func)(Args...));

		/**
		 * @brief Remove method from the list of function to be called on event raised
		 * @tparam C caller class type
		 * @param _caller caller instance
		 * @param _func function to remove
		 * @return if method was removed
		*/
		template <typename C>
		bool Remove(const C* _caller, void(C::* _func)(Args...));

		/**
		 * @brief Raise the event
		 * @param ..._args Argument to forward to functions.
		*/
		void Execute(const Args&... _args);

		/**
		 * @brief clear function and methods vectors
		*/
		void Clear();

		/**
		 * @brief Add function to be call on event raised
		 * @param _func function to add
		*/
		void operator+=(void(*_func)(Args...));
		
		/**
		 * @brief Remove function from the list of function to be called on event raised
		 * @param _func function to remove
		 * @return true if function was removed
		*/
		bool operator-=(void(*_func)(Args...));

		/**
		 * @brief Raise the event
		 * @param ..._args Argument to forward to functions.
		*/
		void operator()(const Args&... _args);
	};
}

#include "Utils/Event/Event.inl"

#endif;
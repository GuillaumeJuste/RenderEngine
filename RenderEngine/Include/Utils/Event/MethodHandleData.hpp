#pragma once

#ifndef UTILS_FUNCHANDLE
#define UTILS_FUNCHANDLE

#include <utility>

namespace Utils
{
	/**
	 * @brief Abstract struct used to store 
	 * methods handle data regardless of template type
	*/
	struct MethodHandleDataBase
	{
		/**
		 * @brief Compare this handle data with other
		 * @param _other Handle data to compare with
		 * @return true if both handle data are the same
		*/
		virtual bool Compare(const MethodHandleDataBase* _other) const = 0;
	};

	/**
	 * @brief Template used to store method handle data
	 * @tparam C method caller instance
	 * @tparam ...Args method arguments
	*/
	template <typename C, typename... Args>
	struct MethodHandleData : MethodHandleDataBase
	{
		/// Caller ptr handle.
		C* caller = nullptr;

		/// Function ptr handle.
		void(C::* func)(Args...) = nullptr;

		/// Default constructor
		MethodHandleData() = default;

		/**
		 * @brief default value constructor
		 * @param _caller method caller instance
		 * @param _func method to call
		*/
		MethodHandleData(C* _caller, void(C::* _func)(Args...));

		/**
		 * @brief Compare this handle data with other
		 * @param _other Handle data to compare with
		 * @return true if both handle data are the same
		*/
		bool Compare(const MethodHandleDataBase* _other) const override final;

		/**
		 * @brief Interface methods requiered for function handle.
		 *		  Ensure correct child type call.
		 * @param _data Stored typed FuncMemberData.
		 * @param ..._args method arguments.
		*/
		static void InterfaceCall(void* _data, Args... _args);
	};
}

#include "Utils/Event/MethodHandleData.inl"

#endif;
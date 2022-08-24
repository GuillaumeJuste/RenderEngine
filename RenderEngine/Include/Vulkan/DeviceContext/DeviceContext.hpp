#pragma once

#ifndef RENDERENGINE_DEVICECONTEXT
#define RENDERENGINE_DEVICECONTEXT

#include "Vulkan/DeviceContext/DeviceContextCreateInfo.hpp"
#include "Vulkan/DeviceContext/QueueFamilyIndices.hpp"
#include "Vulkan/SwapChain/SwapChainSupportDetails.hpp"
#include "Vulkan/Surface/Surface.hpp"
#include "Glfw/Window/Window.hpp"
#include "Vulkan/RenderContext/RenderContext.hpp"
#include "Vulkan/DeviceContext/PhysicalDeviceProperties.hpp"

#include <forward_list>

namespace RenderEngine::Vulkan
{
	class DeviceContext
	{
	private:
		/**
		 * @brief instance used to creat the device
		*/
		VkInstance instance;

		/**
		 * @brief window used to create the properties from to create the device
		*/
		WindowProperties* windowProperties;

		/**
		 * @brief physical device (GPU)
		*/
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		/**
		 * @brief properties of the physical device
		*/
		PhysicalDeviceProperties physicalDeviceProperties;

		/**
		 * @brief logical device
		*/
		VkDevice logicalDevice = VK_NULL_HANDLE;

		/**
		 * @brief device graphics queue
		*/
		VkQueue graphicsQueue = VK_NULL_HANDLE;

		/**
		 * @brief device present queue
		*/
		VkQueue presentQueue = VK_NULL_HANDLE;

		/**
		 * @brief render context owned by the device
		*/
		std::forward_list<RenderContext> renderContexts;

		/**
		 * @brief check if the device properties match the properties necessary for rendering with vulkan
		 * @param _properties properties of the physical device
		 * @return true if the properties match
		*/
		bool IsDeviceSuitable(PhysicalDeviceProperties* _properties);

		/**
		 * @brief Find the index for the present and graphics queues
		 * @param _device divice to serch index from
		 * @param _output struct containing the index for the queues 
		 * @return true if queues index were found
		*/
		bool FindQueueFamilies(VkPhysicalDevice _device, QueueFamilyIndices* _output);
		
		/**
		 * @brief check if the physical device have all the requiered extentions
		 * @param _device physical device to check
		 * @return true if all requiered extentions are found
		*/
		bool checkDeviceExtensionSupport(const VkPhysicalDevice& _device);
		
		/**
		 * @brief Selection a physical device
		*/
		void PickPhysicalDevice();

		/**
		 * @brief Console print for user selection of the physical device if multiple are eligible
		 * @param _physicalDevicesProperties List of the physical devices eligible
		 * @return selected physical device
		*/
		PhysicalDeviceProperties UserSelectPhysicalDevice(std::vector<PhysicalDeviceProperties> _physicalDevicesProperties);

		/**
		 * @brief create logical device
		*/
		void CreateLogicalDevice();

	public:
		/// default constructor
		DeviceContext() = default;

		/// default destructor
		~DeviceContext() = default;

		/**
		 * @brief Initalize a device
		 * @param _createInfo Information used to intitalize the device
		 * @param _output 
		*/
		static void InitalizeDeviceContext(const DeviceContextCreateInfo& _createInfo, DeviceContext* _output);

		/**
		 * @brief Create a Render context and add it to the list of owned render context
		 * @return Render context created
		*/
		RenderContext* AddRenderContext();

		/**
		 * @brief get a reference on the physical device
		*/
		const VkPhysicalDevice& GetPhysicalDevice() const;
		
		/**
		 * @brief get a reference on the logical device
		*/
		const VkDevice& GetLogicalDevice() const;
		
		/**
		 * @brief get a reference on the graphics queue
		*/
		const VkQueue& GetGraphicsQueue() const;

		/**
		 * @brief Clean used vulkan class and list of owned render contexts
		*/
		void Cleanup();
	};
}

#endif
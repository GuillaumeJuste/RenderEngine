#pragma once

#ifndef RENDERENGINE_DESCRIPTORSETDATALIST
#define RENDERENGINE_DESCRIPTORSETDATALIST

#include "Rendering/Vulkan/Descriptor/Set/DescriptorData.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorDataListCreateInfo.hpp"

namespace RenderEngine::Rendering
{
	class DescriptorDataList
	{
	private :
		std::vector<DescriptorData> descriptorDatas;

		static VkShaderStageFlagBits EnumToVkFlag(RenderEngine::Assets::ShaderStage _stage);
	public :
		DescriptorDataList() = default;

		size_t Size() const;
		void Add(const DescriptorData& _data);
		
		const DescriptorData& operator[](size_t _index) const;

		static std::vector<DescriptorDataList> GenerateDescriptorDataLists(DescriptorDataListCreateInfo& _createInfo);
	};
}

#endif
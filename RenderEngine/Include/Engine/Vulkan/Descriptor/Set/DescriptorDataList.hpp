#pragma once

#ifndef RENDERENGINE_DESCRIPTORSETDATALIST
#define RENDERENGINE_DESCRIPTORSETDATALIST

#include "Engine/Vulkan/Descriptor/Set/DescriptorData.hpp"

namespace RenderEngine::Engine::Vulkan
{
	class DescriptorDataList
	{
	private :
		std::vector<DescriptorData> descriptorDatas;

	public :
		DescriptorDataList() = default;

		size_t Size() const;
		void Add(const DescriptorData& _data);
		
		void Remove(const DescriptorData& _data);

		const DescriptorData& operator[](size_t _index) const;
	};
}

#endif
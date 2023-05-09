#include "Rendering/Vulkan/Descriptor/Set/DescriptorDataList.hpp"

using namespace RenderEngine::Rendering;

size_t DescriptorDataList::Size() const
{
	return descriptorDatas.size();
}

void DescriptorDataList::Add(const DescriptorData& _data)
{
	descriptorDatas.push_back(_data);
}

const DescriptorData& DescriptorDataList::operator[](size_t _index) const
{
	return descriptorDatas[_index];
}
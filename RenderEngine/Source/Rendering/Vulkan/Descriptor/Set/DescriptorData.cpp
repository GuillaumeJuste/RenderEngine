#include "Rendering/Vulkan/Descriptor/Set/DescriptorData.hpp"

using namespace RenderEngine::Rendering;

bool DescriptorData::operator==(const DescriptorData& _rhs)
{
	return binding == _rhs.binding && descriptorType == _rhs.descriptorType && stageFlags == _rhs.stageFlags;
}
#include "Engine/Vulkan/Descriptor/Set/DescriptorData.hpp"

using namespace RenderEngine::Engine::Vulkan;

bool DescriptorData::operator==(const DescriptorData& _rhs)
{
	return binding == _rhs.binding && descriptorType == _rhs.descriptorType && stageFlags == _rhs.stageFlags;
}
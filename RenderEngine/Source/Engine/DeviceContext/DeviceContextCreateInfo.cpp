#include "Engine/DeviceContext/DeviceContextCreateInfo.hpp"

using namespace RenderEngine::Vulkan;

DeviceContextCreateInfo::DeviceContextCreateInfo(const VkInstance& _instance, Surface* _surface, Window* _window) :
	instance { _instance }, surface { _surface }, window { _window }
{
}
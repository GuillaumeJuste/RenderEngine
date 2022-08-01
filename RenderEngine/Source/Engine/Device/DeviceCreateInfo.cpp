#include "Engine/Device/DeviceCreateInfo.hpp"
#include "Engine/Window/Window.hpp"

using namespace RenderEngine::Vulkan;

DeviceCreateInfo::DeviceCreateInfo(const VkInstance& _instance, Surface* _surface, Window* _window) :
	instance { _instance }, surface { _surface }, window { _window }
{
}
#pragma once

#ifndef RENDERENGINE_IENGINEINSTANCECREATEINFO
#define RENDERENGINE_IENGINEINSTANCECREATEINFO

#include "Space/Vec3.hpp"

namespace RenderEngine::Engine::Base
{
	struct IEngineInstanceCreateInfo
	{
		/// application name
		std::string applicationName;

		/// application version (Major, Minor, Patch)
		Mathlib::Vec3 applicationVersion = Mathlib::Vec3::Zero;
		
		/// engine name
		std::string engineName;
		
		/// engine version (Major, Minor, Patch)
		Mathlib::Vec3 engineVersion = Mathlib::Vec3::Zero;
	};
}

#endif
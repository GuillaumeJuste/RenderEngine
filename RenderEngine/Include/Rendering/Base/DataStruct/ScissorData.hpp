#pragma once

#ifndef RENDERENGINE_SCISSORDATA
#define RENDERENGINE_SCISSORDATA

#include "Collections/Space.hpp"

namespace RenderEngine::Rendering
{
	struct ScissorData
	{
		Mathlib::Vec2 offset = Mathlib::Vec2::Zero;
		Mathlib::Vec2 extent = Mathlib::Vec2::Zero;
	};
}

#endif
#pragma once

#ifndef RENDERENGINE_VIEWPORTDATA
#define RENDERENGINE_VIEWPORTDATA

namespace RenderEngine::Engine::Base
{
	struct ViewportData
	{
		float X = 0.f;
		float Y = 0.f;
		float width = 0.f;
		float height = 0.f;
		float minDepth = 0.f;
		float maxDepth = 1.f;
	};
}

#endif
#pragma once

#ifndef RENDERENGINE_IASSET
#define RENDERENGINE_IASSET

#include <string>

namespace RenderEngine::Assets
{
	struct IAsset
	{
	public:
		std::string filePath;

		bool operator==(const IAsset& _rhs) const;

		virtual void Clean() = 0;
	};
}

#endif
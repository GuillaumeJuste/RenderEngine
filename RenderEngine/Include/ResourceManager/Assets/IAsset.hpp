#pragma once

#ifndef RENDERENGINE_IASSET
#define RENDERENGINE_IASSET

#include <string>

namespace RenderEngine::Assets
{
	/**
	 * @brief Assets base struct
	*/
	struct IAsset
	{
	public:
		/// filepath to the asset
		std::string filePath;

		/**
		 * @brief asset comparison from name
		 * @param _rhs asset to compare this with
		 * @return true if assests name match
		*/
		bool operator==(const IAsset& _rhs) const;
	};
}

#endif
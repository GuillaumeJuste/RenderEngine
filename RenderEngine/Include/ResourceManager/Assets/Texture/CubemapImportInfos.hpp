#pragma once

#ifndef RENDERENGINE_CUBEMAPIMPORTINFOS
#define RENDERENGINE_CUBEMAPIMPORTINFOS

#include "ResourceManager/Assets/IAsset.hpp"
#include "Rendering/Base/Interface/Primitive/ITexture.hpp"

namespace RenderEngine::Assets
{
    /**
     * @brief CubemapImportInfos data struct 
	 */
    struct CubemapImportInfos
    {
		/// file path to cubemap faces texture
		union
		{
			std::string pathes[6]{};
			struct
			{
				std::string right;
				std::string left;
				std::string top;
				std::string bottom;
				std::string front;
				std::string back;
			};
		};

		/// constructor from file paths
        CubemapImportInfos(const std::string& _right, const std::string& _left,
			const std::string& _top, const std::string& _bottom,
			const std::string& _front, const std::string& _back) noexcept;

		/// copy constructor
		CubemapImportInfos(const CubemapImportInfos& _other) noexcept;
    };
}

#endif
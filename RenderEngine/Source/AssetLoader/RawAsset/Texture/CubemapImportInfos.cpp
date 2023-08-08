#include "AssetLoader/RawAsset/Texture/CubemapImportInfos.hpp"

using namespace Loader;

CubemapImportInfos::CubemapImportInfos(const std::string& _right, const std::string& _left,
	const std::string& _top, const std::string& _bottom,
	const std::string& _front, const std::string& _back) noexcept :
	right{ _right },
	left{ _left },
	top{ _top },
	bottom{ _bottom },
	front{ _front },
	back{ _back }
{
}

CubemapImportInfos::CubemapImportInfos(const CubemapImportInfos& _other) noexcept :
	right{ _other.right },
	left{ _other.left },
	top{ _other.top },
	bottom{ _other.bottom },
	front{ _other.front },
	back{ _other.back }
{

}

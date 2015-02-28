#pragma once

#include "TextAtlas.h"
#include "System/BasicTypes.h"
#include <string>

namespace bv { namespace model {

class IFontEngine
{
public:
	virtual TextAtlasConstPtr	CreateAtlas( SizeType padding, const std::wstring & wcharsSet )						 = 0;
	virtual TextAtlasConstPtr	CreateAtlas( SizeType padding, SizeType outline, const std::wstring & wcharsSet )	 = 0;

};

DEFINE_CONST_PTR_TYPE(IFontEngine)
DEFINE_PTR_TYPE(IFontEngine)

} // bv
} // model
#pragma once

#include "TextAtlas.h"
#include "CoreDEF.h"
#include <string>

namespace bv { namespace model {

class IFontEngine
{
public:
	virtual TextAtlasConstPtr	CreateAtlas( UInt32 padding, const std::wstring & wcharsSet )						 = 0;
	virtual TextAtlasConstPtr	CreateAtlas( UInt32 padding, UInt32 outline, const std::wstring & wcharsSet )	 = 0;
};

DEFINE_CONST_PTR_TYPE(IFontEngine)
DEFINE_PTR_TYPE(IFontEngine)

} // bv
} // model
#pragma once

#include "System/BasicTypes.h"
#include <string>

namespace bv { namespace model {

class TextAtlas;

class IFontEngine
{
public:
	virtual const TextAtlas *	CreateAtlas( SizeType padding, const std::wstring & wcharsSet )						 = 0;
	virtual const TextAtlas *	CreateAtlas( SizeType padding, SizeType outline, const std::wstring & wcharsSet )	 = 0;

};

DEFINE_CONST_PTR_TYPE(IFontEngine)
DEFINE_PTR_TYPE(IFontEngine)

} // bv
} // model
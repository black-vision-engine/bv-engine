#pragma once

#include "System/BasicTypes.h"
#include <string>

namespace bv { namespace model {

class TextAtlas;

class IFontEngine
{
public:
	virtual const TextAtlas *			CreateAtlas( const std::string& fontFilePath, size_t fontSize, size_t padding, const std::wstring & wcharsSet ) const = 0;

};

DEFINE_CONST_PTR_TYPE(IFontEngine)
DEFINE_PTR_TYPE(IFontEngine)

} // bv
} // model
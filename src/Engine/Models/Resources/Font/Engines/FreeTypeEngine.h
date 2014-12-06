#pragma once


#include "Engine/Models/Resources/Font/IFontEngine.h"

#include "System/BasicTypes.h"
#include <string>

namespace bv { namespace model {

class TextAtlas;
class FreeTypeEngine;

DEFINE_CONST_PTR_TYPE(FreeTypeEngine)
DEFINE_PTR_TYPE(FreeTypeEngine)

class FreeTypeEngine : public IFontEngine
{
public:
	virtual const TextAtlas *			CreateAtlas( const std::string & fontFilePath, size_t fontSize, size_t padding, const std::wstring & wcharsSet ) const override;

	static FreeTypeEngineConstPtr		Create();
};

} // bv
} // model
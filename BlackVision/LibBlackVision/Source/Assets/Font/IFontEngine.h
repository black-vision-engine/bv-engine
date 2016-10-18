#pragma once

#include "TextAtlas.h"
#include "CoreDEF.h"
#include "Mathematics/glm_inc.h"

#include "FTContour.h"

#include <string>
#include <vector>


typedef std::vector< std::unique_ptr< FTContour > > ContoursList;

namespace bv { 

class IFontEngine
{
public:
	virtual TextAtlasConstPtr	CreateAtlas( UInt32 padding, const std::wstring & wcharsSet, bool makeSizesPowerOf2 = false )					 = 0;
	virtual TextAtlasConstPtr	CreateAtlas( UInt32 padding, UInt32 outline, const std::wstring & wcharsSet, bool makeSizesPowerOf2 = false )	 = 0;

    virtual ContoursList        Create3dVerticies   ( wchar_t ch, float size )                                                                   = 0;
};

DEFINE_CONST_PTR_TYPE(IFontEngine)
DEFINE_PTR_TYPE(IFontEngine)

} // bv

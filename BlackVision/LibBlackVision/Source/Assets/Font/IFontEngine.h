#pragma once

#include "2D/TextAtlas.h"
#include "3D/TextGeometry.h"
#include "FTContour.h"


namespace bv { 

class IFontEngine
{
public:

	virtual TextAtlasPtr	    CreateAtlas         ( UInt32 padding, const std::wstring & wcharsSet, bool makeSizesPowerOf2 = false )					 = 0;
	virtual TextAtlasPtr	    CreateAtlas         ( UInt32 padding, UInt32 outline, const std::wstring & wcharsSet, bool makeSizesPowerOf2 = false )	 = 0;

    virtual void                FillTextGeometry    ( TextGeometryPtr textGeom, const std::wstring & wcharsSet )                                         = 0;


    virtual ContoursList        Create3dVerticies   ( wchar_t ch, float size )                                                                           = 0;

};

DEFINE_CONST_PTR_TYPE(IFontEngine)
DEFINE_PTR_TYPE(IFontEngine)

} // bv

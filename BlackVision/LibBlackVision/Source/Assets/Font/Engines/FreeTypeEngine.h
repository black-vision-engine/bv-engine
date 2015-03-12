#pragma once


#include "Assets/Font/IFontEngine.h"
#include "Assets/Font/Text.h"

#include "CoreDEF.h"
#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

namespace bv { namespace model {

class TextAtlas;
class FreeTypeEngine;
struct Glyph;
class Spans;

DEFINE_CONST_PTR_TYPE(FreeTypeEngine)
DEFINE_PTR_TYPE(FreeTypeEngine)

class FreeTypeEngine : public IFontEngine
{
private:

	UInt32						m_maxHeight;
	UInt32						m_maxWidth;
	SizeType					m_fontSize;
	FT_Library					m_library;
	FT_Face 					m_face;

	Glyph *						RenderGlyph( wchar_t ch, Spans &, SizeType );


public:
	virtual TextAtlasConstPtr	CreateAtlas( UInt32 padding, const std::wstring & wcharsSet )					override;
	virtual TextAtlasConstPtr	CreateAtlas( UInt32 padding, UInt32 outline, const std::wstring & wcharsSet )	override;

								FreeTypeEngine( const std::string & fontFilePath, size_t fontSize );

	static FreeTypeEnginePtr	Create( const std::string & fontFilePath, size_t fontSize );
};

} // bv
} // model
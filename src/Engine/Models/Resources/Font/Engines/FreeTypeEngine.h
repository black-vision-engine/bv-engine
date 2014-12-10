#pragma once


#include "Engine/Models/Resources/Font/IFontEngine.h"

#include "System/BasicTypes.h"
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

	int							m_maxHeight;
	int							m_maxWidth;
	int							m_fontSize;
	FT_Library					m_library;
	FT_Face 					m_face;

	Glyph *						RenderGlyph( wchar_t ch, Spans & );


public:
	virtual const TextAtlas *	CreateAtlas( SizeType padding, const std::wstring & wcharsSet )						override;
	virtual const TextAtlas *	CreateAtlas( SizeType padding, SizeType outline, const std::wstring & wcharsSet )	override;

								FreeTypeEngine( const std::string & fontFilePath, size_t fontSize );

	static FreeTypeEnginePtr	Create( const std::string & fontFilePath, size_t fontSize );
};

} // bv
} // model
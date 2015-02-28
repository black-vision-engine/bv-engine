#pragma once

#include "TextAtlas.h"
#include "Engine/Models/Resources/IResource.h"
#include "Engine/Models/Resources/Font/IFontEngine.h"

#include <string>
#include <map>

namespace bv { namespace model {


struct Glyph;
class Text;
DEFINE_CONST_PTR_TYPE(Text)

class Text
{
private:
    std::wstring                        m_supportedCharsSet;
    std::string                         m_fontFile;
    TextAtlasConstPtr                   m_atlas;
    SizeType							m_fontSize;
    SizeType							m_blurSize;
	SizeType							m_outlineWidth; // if > 0 text is outlined
	IFontEnginePtr						m_fontEngine;

    void                                BuildAtlas			();
	void                                BuildAtlasOutlined	();
    TextAtlasConstPtr                   LoadFromCache		();

public:

    TextAtlasConstPtr                   GetAtlas() const { return m_atlas; }

	static TextConstPtr					Create(const std::wstring& supportedCharsSet
											, const std::string& fontFile
											, SizeType fontSize
											, SizeType blurSize
											, SizeType outlineSize );

    explicit                            Text( const std::wstring& supportedCharsSet
											, const std::string& fontFile
											, SizeType fontSize
											, SizeType blurSize
											, SizeType outlineSize );
};

} // model
} // bv

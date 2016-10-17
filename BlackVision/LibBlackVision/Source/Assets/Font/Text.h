#pragma once

#include "TextAtlas.h"
#include "Assets/Asset.h"
#include "Assets/Font/IFontEngine.h"

#include <string>
#include <map>

namespace bv { 


struct Glyph;
class Text;
DEFINE_CONST_PTR_TYPE(Text)

class Text
{
private:
    std::wstring                        m_supportedCharsSetFile;
    std::string                         m_fontFile;
    TextAtlasConstPtr                   m_atlas;
    UInt32								m_fontSize;
    UInt32								m_blurSize;
	UInt32								m_outlineWidth; // if > 0 text is outlined
	IFontEnginePtr						m_fontEngine;
	bool								m_withMipmaps;

    void                                BuildAtlas			();
	void                                BuildAtlasOutlined	();
	void								GenerateMipMaps		();
	void								BlurAtlas			();
	void								AddTexturesKey		();
    TextAtlasConstPtr                   LoadFromCache		();
	void								AddToCache			();

public:

    TextAtlasConstPtr                   GetAtlas            () const { return m_atlas; }

    std::vector< std::unique_ptr< FTContour > >     CreateCharacter3D   ( wchar_t ch, float size ) const;

	static TextConstPtr					Create(const std::wstring& supportedCharsSet
											, const std::string& fontFile
											, UInt32 fontSize
											, UInt32 blurSize
											, UInt32 outlineSize
											, bool withMipmaps );

    explicit                            Text( const std::wstring& supportedCharsSet
											, const std::string& fontFile
											, UInt32 fontSize
											, UInt32 blurSize
											, UInt32 outlineSize
											, bool withMipmaps );


    UInt32                              GetFontSize         () const    { return m_fontSize; }
};

} // bv

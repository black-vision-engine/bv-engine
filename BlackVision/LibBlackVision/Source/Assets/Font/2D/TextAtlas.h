#pragma once

#include "Assets/Texture/TextureAsset.h"
#include "Assets/Font/TextRepresentation.h"

#include "CoreDEF.h"


namespace bv {

class Text;
struct Glyph;

class TextAtlas;
DEFINE_PTR_TYPE( TextAtlas )
DEFINE_CONST_PTR_TYPE(TextAtlas)




class TextAtlas : public TextRepresentation
{
public: // Only for non intrusive serialization. Should be private

    TextureAssetConstPtr                                m_textureAsset;

    std::map< wchar_t, const Glyph * >                  m_outlineGlyphs;
    UInt32                                              m_blurSize;
    UInt32                                              m_glowBlurSize;


    void                    SetGlyph        ( wchar_t wch, const Glyph * glyph, bool outline = false );

public:

    UInt32                  m_glyphWidth;
    UInt32                  m_glyphHeight;


                            TextAtlas       ();
                            ~TextAtlas      ();
                            TextAtlas       ( UInt32 w, UInt32 h, UInt32 bitsPrePixel, UInt32 gw, UInt32 gh );

    UInt32                  GetBitsPerPixel () const;

    UInt32                  GetWidth        () const;
    UInt32                  GetHeight       () const;
    
    UInt32                  GetGlyphX       ( wchar_t c ) const;
    UInt32                  GetGlyphY       ( wchar_t c ) const;

    UInt32                  GetGlyphWidth   ( wchar_t c ) const;
    UInt32                  GetGlyphHeight  ( wchar_t c ) const;

    const Glyph *           GetGlyph        ( wchar_t c, bool outline = false ) const override;

    MemoryChunkConstPtr     GetData         () const;
    MemoryChunkPtr          GetWritableData ();
    SizeType                GetSizeInBytes  () const;

    static TextAtlasPtr     Create          ( UInt32 w, UInt32 h, UInt32 bitsPrePixel, UInt32 gw, UInt32 gh );

    TextureAssetConstPtr	GetAsset		() const;


    static TextureAssetDescConstPtr	GenerateTextAtlasAssetDescriptor( 
                                                const std::string & fontFileName, 
                                                UInt32 width, 
                                                UInt32 height, 
                                                SizeType fontSize, 
                                                SizeType blurSize,
                                                SizeType glowBlurSize,
                                                SizeType outlineSize,
                                                UInt32 mmLevels,
                                                const std::wstring & charSet );

    friend class Text;

    // serialization
    void                    Save    ( std::ostream & out ) const;
    void                    Load    ( std::istream & in );

};

} // bv

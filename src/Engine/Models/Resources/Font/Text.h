#pragma once

#include "Glyph.h"

#include "Engine/Models/Resources/IResource.h"

#include <string>
#include <map>

namespace bv { namespace model {

class Text;

struct GlyphCoords
{
    unsigned int            textureX;
    unsigned int            textureY;

    unsigned int            width;
    unsigned int            height;

    int                     bearingX;
    int                     bearingY;

    unsigned int            glyphX;
    unsigned int            glyphY;

    unsigned int            glyphWidth;
    unsigned int            glyphHeight;

    signed long             advanceX;
    signed long             advanceY;

    GlyphCoords(){}

    GlyphCoords(unsigned int tX,
                unsigned int tY,
                unsigned int w,
                unsigned int h,
                unsigned int gX,
                unsigned int gY,
                unsigned int gw,
                unsigned int gh,
                int bX,
                int bY,
                signed long aX,
                signed long aY)
                : textureX( tX )
                , textureY( tY )
                , width( w )
                , height( h )
                , glyphX( gX )
                , glyphY( gY )
                , glyphWidth( gw )
                , glyphHeight( gh )
                , bearingX( bX )
                , bearingY( bY )
                , advanceX( aX )
                , advanceY( aY )

    {}

    // serialization
    void        Save( std::ostream& out ) const;
    void        Load( std::istream& in );
};

class TextAtlas
{
public: // Only for non intrusive serialization. Should be private

    ResourceHandlePtr         m_textureHandle;
    //char*                   m_data;
    //unsigned int            m_width;
    //unsigned int            m_height;
    //unsigned int            m_bitsPerPixel;


    std::map< wchar_t, GlyphCoords >                        m_glyphsPositions;
    std::map< std::pair< wchar_t, wchar_t >, float >        m_kerningMap; 

    void                    SetGlyphCoords  ( wchar_t wch, const GlyphCoords& coords );

public:

    unsigned int            m_glyphWidth;
    unsigned int            m_glyphHeight;


    unsigned int            GetBitsPerPixel () const;

    unsigned int            GetWidth        () const;
    unsigned int            GetHeight       () const;
    
    unsigned int            GetGlyphX       ( wchar_t c ) const;
    unsigned int            GetGlyphY       ( wchar_t c ) const;

    unsigned int            GetGlyphWidth   ( wchar_t c ) const;
    unsigned int            GetGlyphHeight  ( wchar_t c ) const;

    const GlyphCoords*      GetGlyphCoords  ( wchar_t c ) const;

    float                   GetKerning      ( wchar_t c0, wchar_t c1 ) const;

    MemoryChunkConstPtr     GetData         () const;
    MemoryChunkConstPtr     GetWritableData ();
    unsigned int            GetSizeInBytes  () const;

    TextAtlas();
    TextAtlas( unsigned int w, unsigned int h, unsigned int bitsPrePixel, unsigned int gw, unsigned int gh );

    static TextAtlas*       Crate           ( unsigned int w, unsigned int h, unsigned int bitsPrePixel, unsigned int gw, unsigned int gh );

    friend class Text;

    // serialization
    void                    Save( std::ostream& out ) const;
    void                    Load( std::istream& in );
};

class Text
{
private:
    std::wstring                        m_supportedCharsSet;
    std::string                         m_fontFile;
    TextAtlas*                          m_atlas;
    unsigned int                        m_fontSize;
    unsigned int                        m_blurSize;

    void                                BuildAtlas      ();
    TextAtlas*                          LoadFromCache   ();

public:

    const TextAtlas*                    GetAtlas() const { return m_atlas; }

    explicit                            Text( const std::wstring& supportedCharsSet, const std::string& fontFile, unsigned int fontSize, unsigned int blurSize );
};

} // model
} // bv

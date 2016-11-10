#pragma once

#include "Assets/Font/IFontEngine.h"
#include "Mathematics/Rect.h"

#include FT_FREETYPE_H
#include FT_STROKER_H


namespace bv { 

class FreeTypeEngine;
DEFINE_CONST_PTR_TYPE( FreeTypeEngine )
DEFINE_PTR_TYPE( FreeTypeEngine )


struct GlyphDataInfo
{
    Glyph*          glyph;
    unsigned int    pitch;
    char*           data;

    GlyphDataInfo   ( Glyph * g, unsigned int p, char * d );
};


struct Span
{
    Span    ();
    Span    ( Int32 _x, Int32 _y, Int32 _width, Int32 _coverage );

    Int32 x, y, width, coverage;
};


class Spans
{
public:

    mathematics::Rect       m_boundingRect;
    std::vector< Span * >   m_spans;

public:

    ~Spans();

    Span *          operator []     ( SizeType i );
    const Span *    operator []     ( SizeType i ) const;

    SizeType        size            () const;
    void            push_back       ( Span * s );
    bool            empty           () const;

};


class FreeTypeEngine : public IFontEngine
{
private:

    UInt32                      m_maxHeight;
    UInt32                      m_maxWidth;
    SizeType                    m_fontSize;
    FT_Library                  m_library;
    FT_Face                     m_face;
    std::string                 m_fontFilePath;

    Glyph *                     RenderGlyph     ( wchar_t ch, Spans &, SizeType );

                                FreeTypeEngine  ( const std::string & fontFilePath, size_t fontSize );

public:

    virtual TextAtlasPtr        CreateAtlas     ( UInt32 padding, const std::wstring & wcharsSet, bool makeSizesPowerOf2 = false ) override;
    virtual TextAtlasPtr        CreateAtlas     ( UInt32 padding, UInt32 outline, const std::wstring & wcharsSet, bool makeSizesPowerOf2 = false ) override;

    virtual void                FillTextGeometry( TextGeometryPtr textGeom, const std::wstring & wcharsSet ) override;

    virtual ContoursList        Create3dVerticies   ( wchar_t ch, float size ) override;

    static FreeTypeEnginePtr    Create          ( const std::string & fontFilePath, size_t fontSize );

private:

    ContoursList                MakeContours    ( const FT_GlyphSlot glyph );

};

} // bv
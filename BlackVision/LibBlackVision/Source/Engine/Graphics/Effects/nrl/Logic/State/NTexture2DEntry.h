#pragma once

#include <string>
#include <vector>

#include "Engine/Graphics/Resources/Texture2D.h"


namespace bv { namespace nrl {

class NTexture2DEntry
{
private:

    Texture2DPtr            m_texture;

    std::string             m_samplerName;

    TextureWrappingMode     m_wrapX;
    TextureWrappingMode     m_wrapY;

    TextureFilteringMode    m_filteringMode;

public:

    NTexture2DEntry ();
    NTexture2DEntry ( Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode );

    Texture2DPtr            GetTexture      () const;
    
    const std::string &     GetSamplerName  () const;
    
    TextureWrappingMode     GetWrappingModeX() const;
    TextureWrappingMode     GetWrappingModeY() const;
    
    TextureFilteringMode    GetFilteringMode() const;

    void                    SetTexture      ( Texture2DPtr tex );

    void                    SetSamplerName  ( const std::string & samplerName );
    
    void                    SetWrappingModeX( TextureWrappingMode wrapX );
    void                    SetWrappingModeY( TextureWrappingMode wrapY );
    
    void                    SetFilteringMode( TextureFilteringMode filteringMode );

    void                    Set             ( Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode );

};

typedef std::vector< NTexture2DEntry > NTexture2DEntryVec;

} // nrl
} // bv

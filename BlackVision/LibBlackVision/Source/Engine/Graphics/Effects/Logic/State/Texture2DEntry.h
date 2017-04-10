#pragma once

#include "Engine/Graphics/Effects/Logic/State/SamplerEntry.h"


namespace bv { 

class Texture2DEntry
{
private:

    Texture2DPtr            m_texture;

    SamplerEntry           m_samplerEntry;

public:

    Texture2DEntry ();
    Texture2DEntry ( Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode );

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

typedef std::vector< Texture2DEntry > Texture2DEntryVec;


} // bv

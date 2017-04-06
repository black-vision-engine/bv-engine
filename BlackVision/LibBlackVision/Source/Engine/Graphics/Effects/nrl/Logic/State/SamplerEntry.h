#pragma once

#include <string>
#include <vector>

#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv { namespace nrl {

class SamplerEntry
{
private:

    std::string             m_samplerName;

    TextureWrappingMode     m_wrapX;
    TextureWrappingMode     m_wrapY;

    TextureFilteringMode    m_filteringMode;

public:

    SamplerEntry ();
    SamplerEntry ( const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode );


    const std::string &     GetSamplerName  () const;
    
    TextureWrappingMode     GetWrappingModeX() const;
    TextureWrappingMode     GetWrappingModeY() const;
    
    TextureFilteringMode    GetFilteringMode() const;

    void                    SetSamplerName  ( const std::string & samplerName );
    
    void                    SetWrappingModeX( TextureWrappingMode wrapX );
    void                    SetWrappingModeY( TextureWrappingMode wrapY );
    
    void                    SetFilteringMode( TextureFilteringMode filteringMode );

    void                    Set             ( const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode );

};

typedef std::vector< SamplerEntry > SamplerEntryVec;

} // nrl
} // bv

#pragma once

#include <vector>
#include <string>

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Resources/Texture2D.h"


namespace bv {

//TODO: additional data such as number and values of uv channels, additional channels, render state parameters and so on
class FullscreenEffectData
{
private:

    std::vector< Texture2DPtr > m_inputTextures;
    std::vector< std::string >  m_samplerNames;
    std::vector< IValuePtr >    m_values;

    std::string                 m_pixelShaderSource;

public:


                    FullscreenEffectData    ();
                    ~FullscreenEffectData   ();

    void            AppendInputTexture      ( Texture2DPtr tex, const std::string & samplerName );

    Texture2DPtr    GetInputTextureAt       ( unsigned int i ) const;
    std::string     GetSamplerNameAt        ( unsigned int i ) const;

    unsigned int    GetNumTextures          () const;

    void            AppendValue             ( IValuePtr val );
    IValuePtr       GetValueAt              ( unsigned int i ) const;

    unsigned int    GetNumValues            () const;

    void            SetPixelShaderSource    ( const std::string & src );
    std::string     GetPixelShaderSource    () const;

};

} //bv

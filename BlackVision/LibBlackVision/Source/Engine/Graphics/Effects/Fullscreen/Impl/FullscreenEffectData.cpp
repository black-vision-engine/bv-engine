#include "stdafx.h"

#include "FullscreenEffectData.h"

#include <cassert>


namespace bv {

// **************************
//
FullscreenEffectData::FullscreenEffectData                      ()
    : m_numInitializedTextures( 0 )
    , m_numExternalTextures( 0 )
    , m_blendEnabled( false )
    , m_cullEnabled( false )
    , m_depthTestEnabled( false )
{
}

// **************************
//
FullscreenEffectData::~FullscreenEffectData                     ()
{
}

// **************************
//
void    FullscreenEffectData::AppendInputTexture                ( Texture2DPtr tex, const std::string & samplerName, bool external )
{
    if( tex )
    {
        m_numInitializedTextures++;
    }

    if( external )
    {
        m_externalTexturesIndexes.insert( ( unsigned int ) m_inputTextures.size() );
        m_numExternalTextures++;
    }

    m_inputTextures.push_back( tex );
    m_samplerNames.push_back( samplerName );
}

// **************************
//
bool            FullscreenEffectData::IsExternal                ( unsigned int i ) const
{
    return m_externalTexturesIndexes.find( i ) != m_externalTexturesIndexes.end();
}

// **************************
//
void            FullscreenEffectData::SetInputTexture           ( Texture2DPtr tex, unsigned int idx )
{
    assert( idx < (unsigned int) m_inputTextures.size() );

    auto oldTx = m_inputTextures[ idx ];

    m_inputTextures[ idx ] = tex;

    if( oldTx == nullptr )
    {
        m_numInitializedTextures++;
    }

    if( tex == nullptr )
    {
        m_numInitializedTextures--;
    }
}

// **************************
//
Texture2DPtr    FullscreenEffectData::GetInputTextureAt         ( unsigned int i ) const
{
    assert( i < m_inputTextures.size() );

    return m_inputTextures[ i ];
}

// **************************
//
std::string     FullscreenEffectData::GetSamplerNameAt          ( unsigned int i ) const
{
    assert( i < m_samplerNames.size() );

    return m_samplerNames[ i ];   
}

// **************************
//
unsigned int    FullscreenEffectData::GetNumTextures            () const
{
    return (unsigned int) m_inputTextures.size();
}

// **************************
//
unsigned int    FullscreenEffectData::GetNumExternalTextures     () const
{
    return m_numExternalTextures;
}

// **************************
//
void            FullscreenEffectData::AppendValue               ( IValuePtr val )
{
    m_values.push_back( val );
}

// **************************
//
void            FullscreenEffectData::AppendValues              ( const std::vector< IValuePtr > & values )
{
    for( auto val : values )
    {
        this->AppendValue( val );
    }
}

// **************************
//
IValuePtr       FullscreenEffectData::GetValueAt                ( unsigned int i ) const
{
    assert( i < m_values.size() );

    return m_values[ i ];
}

// **************************
//
unsigned int    FullscreenEffectData::GetNumValues              () const
{
    return (unsigned int) m_values.size();
}

// **************************
//
void            FullscreenEffectData::SetPixelShaderSource      ( const std::string & src )
{
    m_pixelShaderSource = src;
}

// **************************
//
std::string     FullscreenEffectData::GetPixelShaderSource      () const
{
    assert( m_pixelShaderSource != "" );

    return m_pixelShaderSource;
}

// **************************
//
unsigned int    FullscreenEffectData::GetNumInitializedTextures () const
{
    return m_numInitializedTextures;
}

// **************************
//
std::vector< IValuePtr >    FullscreenEffectData::GetValues     () const
{
    return m_values;
}

// **************************
//
bool            FullscreenEffectData::IsBlendEnabled              () const
{
    return m_blendEnabled;
}

// **************************
//
void            FullscreenEffectData::SetBlendEnabled             ( bool enabled )
{
    m_blendEnabled = enabled;
}

// **************************
//
bool            FullscreenEffectData::IsCullEnabled               () const
{
    return m_cullEnabled;
}

// **************************
//
void            FullscreenEffectData::SetCullEnabled              ( bool enabled )
{
    m_cullEnabled = enabled;
}

// **************************
//
bool            FullscreenEffectData::IsDepthTestEnabled          () const
{
    return m_depthTestEnabled;
}

// **************************
//
void            FullscreenEffectData::SetDepthTestEnabled         ( bool enabled )
{
    m_depthTestEnabled = enabled;
}

} //bv

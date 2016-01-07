#include "FullscreenEffectData.h"

#include <cassert>


namespace bv {

// **************************
//
FullscreenEffectData::FullscreenEffectData          ( RenderTarget * outputRenderTarget )
    : m_outputRenderTarget( nullptr )
{
    SetOutputRenderTarget( outputRenderTarget );
}

// **************************
//
FullscreenEffectData::~FullscreenEffectData         ()
{
}

// **************************
//
void    FullscreenEffectData::SetOutputRenderTarget ( RenderTarget * rt )
{
    m_outputRenderTarget = rt;
}

// **************************
//
void    FullscreenEffectData::AppendInputTexture    ( Texture2DPtr tex )
{
    m_inputTextures.push_back( tex );
}

// **************************
//
unsigned int    FullscreenEffectData::GetNumTextures          () const
{
    return (unsigned int) m_inputTextures.size();
}

// **************************
//
Texture2DPtr    FullscreenEffectData::GetInputTextureAt       ( unsigned int i )
{
    assert( i < m_inputTextures.size() );

    return m_inputTextures[ i ];
}

// **************************
//
RenderTarget *  FullscreenEffectData::GetOutputRenderTarget   ()
{
    return m_outputRenderTarget;
}

} //bv

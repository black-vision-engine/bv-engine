#include "PdrTexture.h"

namespace bv
{

// *******************************
//
PdrTexture::PdrTexture()
	: m_textureID( 0 )
    , m_prevTextureID( 0 )
	, m_prevFrameUpdated( false )
    , m_curFrameUpdated( false )
{
}

// *******************************
//
PdrTexture::~PdrTexture()
{
}

// *******************************
//
void            PdrTexture::Enable        ( Renderer * renderer, int textureUnit )
{
    
    { renderer; } // FIXME: suppress unused

    //if( m_prevFrameUpdated && !m_curFrameUpdated )
    //{
    //    m_pboMem->Flush( textureUnit );
    //}

    BVGL::bvglActiveTexture( GL_TEXTURE0 + textureUnit );
    m_prevTextureID = Bind();

    m_prevFrameUpdated = m_curFrameUpdated;
}

// *******************************
//
void            PdrTexture::Disable       ( Renderer * renderer, int textureUnit )
{
    { renderer; } // FIXME: suppress unused
    BVGL::bvglActiveTexture ( GL_TEXTURE0 + textureUnit );
    Unbind();
}

// *******************************
//
void    PdrTexture::SetUpdated    ( bool updated )
{
    m_curFrameUpdated = updated;
}

// *******************************
//
GLuint      PdrTexture::GetTextureID    () const
{
    return m_textureID;
}

} // bv

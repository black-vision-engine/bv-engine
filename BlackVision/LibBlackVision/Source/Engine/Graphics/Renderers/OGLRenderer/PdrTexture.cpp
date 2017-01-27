#include "stdafx.h"

#include "PdrTexture.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{

// *******************************
//
PdrTexture::PdrTexture()
	: m_textureID( 0 )
    , m_prevTextureID( 0 )
	//, m_prevFrameUpdated( false )
    //, m_curFrameUpdated( false )
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
    
    { renderer; } 

    BVGL::bvglActiveTexture( GL_TEXTURE0 + textureUnit );
    m_prevTextureID = Bind();

    //m_prevFrameUpdated = m_curFrameUpdated;
}

// *******************************
//
void            PdrTexture::Disable       ( Renderer * renderer, int textureUnit )
{
    assert( false ); // FIXME: nrl - it seems that no textures are disabled but just in case they are - this is the place to find out
    { renderer; } // FIXME: suppress unused
    BVGL::bvglActiveTexture ( GL_TEXTURE0 + textureUnit );
    Unbind();
}

//// *******************************
////
//void    PdrTexture::SetUpdated    ( bool updated )
//{
//    m_curFrameUpdated = updated;
//}

// *******************************
//
GLuint      PdrTexture::GetTextureID    () const
{
    return m_textureID;
}

} // bv

#include "PdrTexture2D.h"
#include "Engine/Graphics/Resources/Texture2D.h"

#include <assert.h>


namespace bv
{

void            PdrTexture2D::Enable            ()
{
}

void            PdrTexture2D::Disable           ()
{
}

void            PdrTexture2D::Bind              ()
{
}

void            PdrTexture2D::Unbind            ()
{

}

PdrTexture2D*   PdrTexture2D::Create            ( const Texture2D* texture )
{
    return new PdrTexture2D( texture );
}

PdrTexture2D::PdrTexture2D                      ( const Texture2D* texture )
{
    glActiveTexture( GL_TEXTURE0 );
    glGenTextures( 1, &m_textureID );
    
    glBindTexture( GL_TEXTURE_2D, m_textureID );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture->GetWidth(), texture->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->GetData() );
    // FIXME: set format properly
    // FIXME: set type properly

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); // FIXME: set by sampler
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); // FIXME: set by sampler
}

} // bv
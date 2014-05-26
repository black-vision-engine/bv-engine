#include "Texture.h"

#include <cassert>
#include <cstring>


namespace bv
{

// *********************************
//  
Texture::Texture                                ( TextureFormat format, TextureType type, DataBuffer::Semantic semantic )
    : TextureBase( format, type )
    , m_semantic( semantic )
    , m_changed( true )
{
}

// *********************************
//
Texture::~Texture	                            ()
{
}

// *********************************
//
void                    Texture::SetChanged     ( bool changed ) const
{
    m_changed = changed;
}

// *********************************
//
bool                    Texture::Changed        () const
{
    return m_changed;
}

} //bv

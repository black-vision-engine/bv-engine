#include "TextureAnimatedSequence2D.h"

#include <cassert>


namespace bv {

// *********************************
//  
TextureAnimatedSequence2D::TextureAnimatedSequence2D                ( TextureFormat format, TextureType type, int width, int height )
    : TextureAnimatedSequence( format, type )
    , m_width( width )
    , m_height( height )
{
    assert( width > 0 );
    assert( height > 0 );
}

// *********************************
//  
TextureAnimatedSequence2D::~TextureAnimatedSequence2D               ()
{
}

// *********************************
//  
int                 TextureAnimatedSequence2D::GetWidth             () const
{
    return m_width;
}


// *********************************
//  
int                 TextureAnimatedSequence2D::GetHeight            () const
{
    return m_height;
}

// *********************************
//  
bool                TextureAnimatedSequence2D::AddTexture           ( Texture2D * tx )
{
    return Parent::AddTexture( tx );
}

// *********************************
//  
const Texture2D *   TextureAnimatedSequence2D::GetActiveTexture     () const
{
    return static_cast< const Texture2D * >( Parent::GetActiveTexture() );
}

// *********************************
//  
const Texture2D *   TextureAnimatedSequence2D::GetNextActiveTexture () const
{
    return static_cast< const Texture2D * >( Parent::GetNextActiveTexture() );
}

// *********************************
//  
bool                TextureAnimatedSequence2D::PreAddValidate       ( Texture * tx ) const
{
    return PreAddValidate( static_cast< Texture2D * >( tx ) );
}

// *********************************
//  
bool                TextureAnimatedSequence2D::PreAddValidate       ( Texture2D * tx ) const
{
    return tx && tx->GetFormat() == m_format&& tx->GetType() == m_type && tx->GetHeight() == m_height && tx->GetWidth() == m_width;
}

} //bv

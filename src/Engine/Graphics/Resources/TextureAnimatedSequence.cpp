#include "TextureAnimatedSequence.h"

#include <cassert>

#include "Engine/Graphics/Resources/Texture.h"


namespace bv
{

// *********************************
//  
TextureAnimatedSequence::TextureAnimatedSequence                ( TextureFormat format, TextureType type )
    : TextureBase( format, type )
    , m_needsUpdate( false )
    , m_activeTexture( 0 )
    , m_nextActiveTexture( 0 )
{
}

// *********************************
//
TextureAnimatedSequence::~TextureAnimatedSequence	            ()
{
    for( auto tx : m_sequence )
    {
        delete tx;
    }
}

// *********************************
//
unsigned int    TextureAnimatedSequence::NumTextures            () const
{
    return m_sequence.size();
}

// *********************************
//
size_t          TextureAnimatedSequence::SingleEntrySize        () const
{
    assert( m_sequence.size() > 0 );

    return m_sequence[ 0 ]->GetDataSize();
}

// *********************************
//
void            TextureAnimatedSequence::SetActiveTexture       ( unsigned int txNum )
{
    assert( txNum < m_sequence.size() );

    m_activeTexture = txNum;
    m_nextActiveTexture = txNum;
}

// *********************************
//
void            TextureAnimatedSequence::SetNextActiveTexture   ( unsigned int txNum )
{
    assert( txNum < m_sequence.size() );

    m_nextActiveTexture = txNum;
}

// *****************************
//
const Texture * TextureAnimatedSequence::GetActiveTexture       () const
{
    return GetTexture( m_activeTexture );
}

// *****************************
//
const Texture * TextureAnimatedSequence::GetNextActiveTexture   () const
{
    return GetTexture( m_nextActiveTexture );
}

// *****************************
//
unsigned int    TextureAnimatedSequence::GetActiveTextureNum    () const
{
    assert( m_sequence.size() > 0 );

    return m_activeTexture;
}

// *****************************
//
unsigned int    TextureAnimatedSequence::GetNextActiveTextureNum() const
{
    assert( m_sequence.size() > 0 );

    return m_nextActiveTexture;
}

// *****************************
//
bool            TextureAnimatedSequence::AddTexture             ( Texture * tx )
{
    if ( !PreAddValidate( tx ) )
    {
        assert( false );

        return false;
    }

    m_sequence.push_back( tx );

    return true;
}

// *****************************
//
const Texture * TextureAnimatedSequence::GetTexture             ( unsigned int txNum ) const
{
    assert( txNum < m_sequence.size() );

    return m_sequence[ txNum ];
}

// *********************************
//
bool            TextureAnimatedSequence::NeedsUpdate            () const
{
    return m_needsUpdate;
}

// *********************************
//
void            TextureAnimatedSequence::SetNeedsUpdate         ( bool needsUpdate ) const
{
    m_needsUpdate = needsUpdate;
}

// *****************************
//
Texture *       TextureAnimatedSequence::GetTexture             ( unsigned int txNum )
{
    assert( txNum < m_sequence.size() );

    return m_sequence[ txNum ];
}

// *****************************
//
Texture *       TextureAnimatedSequence::ActiveTexture          ()
{
    return GetTexture( m_activeTexture );
}

// *****************************
//
bool            TextureAnimatedSequence::PreAddValidate         ( Texture * tx ) const
{
    assert( tx );

    return true;
}

} //bv

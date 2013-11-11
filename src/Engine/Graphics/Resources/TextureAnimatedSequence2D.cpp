#include "TextureAnimatedSequence2D.h"

#include <cassert>


namespace bv {

// *****************************
//
TextureAnimatedSequence2D::TextureAnimatedSequence2D    ( TextureFormat format, int width, int height )
    : Texture( format, TextureType::T_2D, DataBuffer::Semantic::S_TEXTURE )
    , m_width( width )
    , m_height( height )
    , m_format( format )
    , m_needsUpdate( false )
    , m_activeTexture( 0 )
    , m_nextActiveTexture( 0 )
{
}

// *****************************
//
TextureAnimatedSequence2D::~TextureAnimatedSequence2D   ()
{
}

// *****************************
//
int                 TextureAnimatedSequence2D::GetWidth                    () const
{
    return m_width;
}

// *****************************
//
int                 TextureAnimatedSequence2D::GetHeight                   () const
{
    return m_height;
}

// *****************************
//
TextureFormat       TextureAnimatedSequence2D::GetFormat                    () const
{
    return m_format;
}

// *****************************
//
TextureType         TextureAnimatedSequence2D::GetType                      () const
{
    assert( m_sequence.size() > 0 );

    return m_sequence[ 0 ]->GetType();
}

// *****************************
//
DataBuffer::Semantic TextureAnimatedSequence2D::GetSemantic                 () const
{
    assert( m_sequence.size() > 0 );

    return m_sequence[ 0 ]->GetSemantic();    
}

// *****************************
//
int                 TextureAnimatedSequence2D::GetPixelSize                 () const
{
    assert( m_sequence.size() > 0 );

    return m_sequence[ 0 ]->GetPixelSize();    
}

// *****************************
//
size_t              TextureAnimatedSequence2D::GetDataSize                  () const
{
    assert( m_sequence.size() > 0 );

    return m_sequence[ 0 ]->GetDataSize();
}

// *****************************
//
char *              TextureAnimatedSequence2D::GetData                      ()
{
    return ActiveTexture()->GetData();
}

// *****************************
//
const char *        TextureAnimatedSequence2D::GetData                      () const
{
    return GetActiveTexture()->GetData();
}

// *****************************
//
bool                TextureAnimatedSequence2D::WriteToBuffer                ( const char * memPtr, size_t dataSize )
{
    assert( false ); //Cannot happen

    return false;
    //return ActiveTexture()->WriteToBuffer( memPtr, dataSize );
}

// *****************************
//
bool                TextureAnimatedSequence2D::HasSequence                 () const
{
    return true;
}

// *****************************
//
unsigned int        TextureAnimatedSequence2D::NumTextures                 () const
{
    return m_sequence.size();
}

// *****************************
//
void                TextureAnimatedSequence2D::SetActiveTexture            ( unsigned int txNum )
{
    assert( txNum < m_sequence.size() );

    m_activeTexture = txNum;
    m_nextActiveTexture = txNum;
}

// *****************************
//
void                TextureAnimatedSequence2D::SetNextActiveTexture        ( unsigned int txNum )
{
    assert( txNum < m_sequence.size() );

    m_nextActiveTexture = txNum;
}

// *****************************
//
bool                TextureAnimatedSequence2D::NeedsUpdate                 () const
{
    return m_needsUpdate;
}

// *****************************
//
void                TextureAnimatedSequence2D::SetNeedsUpdate              ( bool needsUpdate ) const
{
    m_needsUpdate = needsUpdate;
}

// *****************************
//
bool                TextureAnimatedSequence2D::AddTexture                  ( Texture2D * tx )
{
    assert( tx->GetFormat() == m_format );
    assert( tx->GetWidth() == m_width );
    assert( tx->GetHeight() == m_height );

    if( tx->GetFormat() != m_format || tx->GetWidth() != m_width || tx->GetHeight() != m_height )
    {
        return false;
    }

    m_sequence.push_back( tx );

    return true;
}

// *****************************
//
const Texture2D *   TextureAnimatedSequence2D::GetTexture                  ( unsigned int txNum ) const
{
    assert( txNum < m_sequence.size() );

    return m_sequence[ txNum ];
}

// *****************************
//
const Texture2D *   TextureAnimatedSequence2D::GetActiveTexture            () const
{
    return GetTexture( m_activeTexture );
}

// *****************************
//
const Texture2D *   TextureAnimatedSequence2D::GetNextActiveTexture        () const
{
    return GetTexture( m_nextActiveTexture );
}

// *****************************
//
unsigned int        TextureAnimatedSequence2D::GetActiveTextureNum         () const
{
    assert( m_sequence.size() > 0 );

    return m_activeTexture;
}

// *****************************
//
unsigned int        TextureAnimatedSequence2D::GetNextActiveTextureNum     () const
{
    assert( m_sequence.size() > 0 );

    return m_nextActiveTexture;
}

// *****************************
//
Texture2D *          TextureAnimatedSequence2D::GetTexture                 ( unsigned int txNum )
{
    assert( txNum < m_sequence.size() );

    return m_sequence[ txNum ];
}

// *****************************
//
Texture2D *         TextureAnimatedSequence2D::ActiveTexture               ()
{
    return GetTexture( m_activeTexture );
}

} //bv

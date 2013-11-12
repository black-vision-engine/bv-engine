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
    , m_data( nullptr )
    , m_dataSize( 0 )
{
}

// *********************************
//
Texture::~Texture	                            ()
{
}

// *********************************
//
size_t                   Texture::GetDataSize     () const
{
    return m_dataSize;
}

// *********************************
//
char *		            Texture::GetData        ()
{
    return m_data;
}

// *********************************
//
const char *            Texture::GetData	    () const
{
    return m_data;
}

// *********************************
//
bool    TextureAccessor::WriteData( Texture * tx, const char * data, size_t dataSize )
{
    if ( tx->GetDataSize() != dataSize )
    {
        delete[] tx->m_data;
        tx->m_data = new char[ dataSize ];
        memcpy( tx->m_data, data, dataSize );
        tx->m_dataSize = dataSize;
    }

    return true;
}

} //bv

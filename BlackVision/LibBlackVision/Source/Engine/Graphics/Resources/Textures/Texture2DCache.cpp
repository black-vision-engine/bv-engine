#include "Texture2DCache.h"

#include <iostream>
#include <cassert>

#include "Engine/Graphics/Resources/Texture2D.h"

#define PRINT_TEXTURE_CACHE_STATS

namespace bv
{

Texture2DCache GTexture2DCache;

// *********************************
//
Texture2DCache::Texture2DCache()
{
}

// *********************************
//
Texture2DCache::~Texture2DCache()
{
}

// *********************************
//
Texture2DPtr    Texture2DCache::GetTexture              ( const ITextureDescriptor * txParams )
{
    auto semantic   = txParams->GetSemantic();
    auto format     = txParams->GetFormat();
    auto width      = txParams->GetWidth();
    auto height     = txParams->GetHeight();
    
    Texture2DPtr tx = nullptr;

    if( semantic == DataBuffer::Semantic::S_STATIC || semantic == DataBuffer::Semantic::S_TEXTURE_STATIC )
    {
        auto it = m_tex2DCache.find( txParams->GetUID() );

        if( it != m_tex2DCache.end() )
        {
#ifdef PRINT_TEXTURE_CACHE_STATS
            printf( "Reading texture %08X from cache\n", it->first );
#endif 
            return it->second;
        }
    }

    tx = CreateEmptyTexture( format, width, height, semantic );
	tx->SetData( txParams->GetBits() );

    if( semantic == DataBuffer::Semantic::S_STATIC || semantic == DataBuffer::Semantic::S_TEXTURE_STATIC )
    {
        assert( m_tex2DSet.find( tx.get() ) == m_tex2DSet.end() );

#ifdef PRINT_TEXTURE_CACHE_STATS
        printf( "Registering texture %08X in cache\n", txParams->GetUID() );
#endif 
        m_tex2DCache[ txParams->GetUID() ] = tx;
        m_tex2DSet.insert( tx.get() );
    }

    return tx;
}

// *********************************
//
Texture2DPtr    Texture2DCache::GetSequence             ( const IAnimationDescriptor * animParams )
{
    auto format     = animParams->GetFormat();
    auto width      = animParams->GetWidth();
    auto height     = animParams->GetHeight();
    
    auto sequence   = CreateEmptyTexture( format, width, height, DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE ); //FIXME: are there any chances that other semantics can be used for animations??
	sequence->SetData( animParams->GetBits( animParams->CurrentFrame() ) );

    return sequence;
}

// *********************************
//
bool            Texture2DCache::IsRegistered            ( const ITextureDescriptor * txParams ) const
{
    return m_tex2DCache.find( txParams->GetUID() ) != m_tex2DCache.end();
}

// *********************************
//
bool            Texture2DCache::IsStored                ( Texture2DPtr tex ) const
{
    return m_tex2DSet.find( tex.get() ) != m_tex2DSet.end();
}

// *********************************
//
void            Texture2DCache::ClearCache              ()
{
#ifdef PRINT_TEXTURE_CACHE_STATS
    printf( "Removing %d entries from texture cache\n", m_tex2DCache.size() );
#endif 
    m_tex2DCache.clear();
    m_tex2DSet.clear();
}

// *********************************
//
Texture2DPtr         Texture2DCache::CreateEmptyTexture    ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic )
{
    auto texture = std::make_shared< Texture2D >( format, width, height, semantic );

    return texture;
}

// *********************************
//
Texture2DPtr         Texture2DCache::CreateTexture          ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic, MemoryChunkConstPtr data )
{
    auto texture = CreateEmptyTexture( format, width, height, semantic );
	std::vector< MemoryChunkConstPtr > tex;
	tex.push_back( data );
	texture->SetData( tex );

    return texture;
}

} // bv

#include "stdafx.h"

#include "Texture2DCache.h"

#include <iostream>
#include <cassert>

#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#ifdef _DEBUG
#define PRINT_TEXTURE_CACHE_STATS
#include "UseLoggerLibBlackVision.h"
#endif // DEBUG



#include "Memory/MemoryLeaks.h"



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

	if( txParams->GetBits( 0 ) &&
		( semantic == DataBuffer::Semantic::S_STATIC || semantic == DataBuffer::Semantic::S_TEXTURE_STATIC ) )
    {
        auto it = m_tex2DCache.find( txParams->GetUID() );

        if( it != m_tex2DCache.end() )
        {
#ifdef PRINT_TEXTURE_CACHE_STATS
            LOG_MESSAGE( SeverityLevel::info ) << "Reading texture [" << ( UInt32 )it->first << "] from cache.";
#endif 
            return it->second;
        }
    }

	tx = CreateEmptyTexture( format, width, height, semantic, txParams->GetNumLevels() );
	tx->SetData( txParams->GetBits(), txParams->GetFormat(), txParams->GetWidth(), txParams->GetHeight(), txParams->GetNumLevels() );

	if( txParams->GetBits( 0 ) &&
		( semantic == DataBuffer::Semantic::S_STATIC || semantic == DataBuffer::Semantic::S_TEXTURE_STATIC ) )
    {
        assert( m_tex2DSet.find( tx.get() ) == m_tex2DSet.end() );

#ifdef PRINT_TEXTURE_CACHE_STATS
        LOG_MESSAGE( SeverityLevel::info ) << "Registering texture [" << ( UInt32 )txParams->GetUID() << "] in cache.";
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
	
	//FIXME: assumption that animation texture has only one level (anim desc doesn't provide that info)
    auto sequence   = CreateEmptyTexture( format, width, height, DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE, 1 ); //FIXME: are there any chances that other semantics can be used for animations??
	sequence->SetData( animParams->GetBits( 0 ) );

    return sequence;
}


// *********************************
//
Texture2DPtr    Texture2DCache::GetTexture              ( ITextureDescriptor::uid_t uid ) const
{
    auto it = m_tex2DCache.find( uid );
    if( it != m_tex2DCache.end() )
    {
        return it->second;
    }

    return nullptr;
}

// *********************************
//
bool            Texture2DCache::IsRegistered            ( const ITextureDescriptor * txParams ) const
{
	return !txParams->GetBits().empty() && m_tex2DCache.find( txParams->GetUID() ) != m_tex2DCache.end();
}

// *********************************
//
bool            Texture2DCache::IsStored                ( Texture2DConstPtr tex ) const
{
    return m_tex2DSet.find( tex.get() ) != m_tex2DSet.end();
}

// *********************************
//
void            Texture2DCache::ClearCache              ()
{
#ifdef PRINT_TEXTURE_CACHE_STATS
    LOG_MESSAGE( SeverityLevel::info ) << "Removing all [" << ( UInt64 )m_tex2DCache.size() << "] entries from texture cache.";
#endif 
    m_tex2DCache.clear();
    m_tex2DSet.clear();
}

// *********************************
//
bool            Texture2DCache::ClearAsset              ( ITextureDescriptor::uid_t uid )
{
    if( m_tex2DCache.count( uid ) > 0 )
    {
        m_tex2DSet.erase( m_tex2DCache[ uid ].get() );
        m_tex2DCache.erase( uid );
        
        return true;
    }
    return false;
}

// *********************************
//
Texture2DPtr         Texture2DCache::CreateEmptyTexture    ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic, UInt32 levels )
{
    auto texture = std::make_shared< Texture2D >( format, width, height, semantic, levels );

    return texture;
}

// *********************************
//
Texture2DPtr         Texture2DCache::CreateTexture          ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic, MemoryChunkConstPtr data )
{
	//FIXME assumption that texture has only one level
    auto texture = CreateEmptyTexture( format, width, height, semantic, 1 );
	texture->SetData( data );

    return texture;
}

} // bv

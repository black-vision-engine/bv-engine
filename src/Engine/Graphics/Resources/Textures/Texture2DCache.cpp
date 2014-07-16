#include "Texture2DCache.h"

#include <iostream>
#include <cassert>

#include "Engine/Graphics/Resources/Texture2DImpl.h"
#include "Engine/Graphics/Resources/Texture2DSequenceImpl.h"

#include "Engine/Models/Resources/Resource.h"
#include "Engine/Models/Resources/TextureLoader.h"


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
    auto semantic = txParams->GetSemantic();
    auto format     = txParams->GetFormat();
    auto width      = txParams->GetWidth();
    auto height     = txParams->GetHeight();
    
    Texture2DImplPtr tx = nullptr;

    if( semantic == DataBuffer::Semantic::S_STATIC )
    {
        auto it = m_tex2DCache.find( txParams->GetUID() );

        if( it != m_tex2DCache.end() )
        {
            return it->second;
        }
    }

    tx = CreateEmptyTexture( format, width, height, semantic );
    tx->SetRawData( txParams->GetBits(), format, width, height );

    if( semantic == DataBuffer::Semantic::S_STATIC )
    {
        m_tex2DCache[ txParams->GetUID() ] = tx;
    }

    return tx;
}

// *********************************
//
Texture2DPtr    Texture2DCache::GetAnimation            ( const IAnimationDescriptor * animParams )
{
    auto format     = animParams->GetFormat();
    auto width      = animParams->GetWidth();
    auto height     = animParams->GetHeight();
    
    auto sequence   = CreateEmptySequence( format, width, height );

    for( unsigned int i = 0; i < animParams->NumTextures(); ++i )
    {
        bool bAdded = sequence->AddTextureSettingRawData( animParams->GetBits( i ), format, width, height );
        assert( bAdded );
    }

    return sequence;
}

// *********************************
//
void            Texture2DCache::ClearCache              ()
{
    m_tex2DCache.clear();
}

// *********************************
//
Texture2DImplPtr         Texture2DCache::CreateEmptyTexture    ( TextureFormat format, unsigned int width, unsigned int height, DataBuffer::Semantic semantic )
{
    auto texture = std::make_shared< Texture2DImpl >( format, width, height, semantic );

    return texture;
}

// *********************************
//
Texture2DSequenceImplPtr Texture2DCache::CreateEmptySequence   ( TextureFormat format, unsigned int width, unsigned int height )
{
    auto sequence   = std::make_shared< Texture2DSequenceImpl >( format, width, height );

    return sequence;
}

} // bv

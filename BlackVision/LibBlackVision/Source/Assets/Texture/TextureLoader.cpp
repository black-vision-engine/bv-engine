#include "stdafx.h"

#include "TextureLoader.h"
#include "LibImage.h"
#include "TextureUtils.h"
#include "Tools/IncludeJSON.h"

#include <cassert>



#include "Memory/MemoryLeaks.h"



namespace bv {


// ******************************
//
AssetConstPtr TextureLoader::LoadAsset( const AssetDescConstPtr & desc ) const
{
    auto typedDesc = QueryTypedDesc< TextureAssetDescConstPtr >( desc );

    assert( typedDesc );

    switch( typedDesc->GetLoadingType() )
    {
        case TextureAssetLoadingType::LOAD_ONLY_ORIGINAL_TEXTURE:
        {
            return TextureUtils::LoadOrginalTextureOnly( typedDesc );
        }

        case TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS:
        {
            return TextureUtils::LoadTextureAndMipMaps( typedDesc );
        }

        case TextureAssetLoadingType::LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS:
        {
            return TextureUtils::LoadTextureAndGenerateMipMaps( typedDesc );
        }

        default:
        {
            assert( !"Should never be here" );
            return nullptr;
        }
    }
}

// ******************************
//
AssetDescConstPtr TextureLoader::CreateDescriptor( const IDeserializer& deserializeObject ) const
{
    return std::static_pointer_cast<const AssetDesc>( TextureAssetDesc::Create( deserializeObject ) );
}

// ******************************
//
ThumbnailConstPtr TextureLoader::LoadThumbnail   ( const AssetDescConstPtr & desc ) const
{
    auto typedDesc = QueryTypedDesc< TextureAssetDescConstPtr >( desc );

    assert( typedDesc );

    return TextureUtils::LoadThumbnail( typedDesc );
}


} // bv

#include "TextureLoader.h"
#include "LibImage.h"
#include "TextureUtils.h"

#include <cassert>

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

} // bv

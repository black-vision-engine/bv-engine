#include "stdafx.h"

#include "AVAssetLoader.h"
#include "AVAssetUtils.h"

#include "Serialization/Json/JsonDeserializeObject.h"
#include "ProjectManager.h"


namespace bv {


// ***********************
//
AssetConstPtr		AVAssetLoader::LoadAsset			( const AssetDescConstPtr & desc ) const
{
	auto typedDesc = QueryTypedDesc< AVAssetDescConstPtr >( desc );

	assert( typedDesc );

    auto path = ProjectManager::GetInstance()->ToAbsPath( typedDesc->GetStreamPath() );
    
    if( !Path::IsFile( path ) )
        return nullptr;

    return std::make_shared<AVAsset>( path.Str(), typedDesc->GetTextureFormat(), typedDesc->GetWidth(), typedDesc->GetHeight(), typedDesc->GetFrameRate(), typedDesc->GetVideoFormat() );
}

// ***********************
//
AssetDescConstPtr	AVAssetLoader::CreateDescriptor	    ( const IDeserializer& deserializeObject ) const
{
    return std::static_pointer_cast< const AssetDesc >( AVAssetDesc::Create( deserializeObject ) );
}

// ***********************
//
ThumbnailConstPtr   AVAssetLoader::LoadThumbnail        ( const AssetDescConstPtr & desc ) const
{
    auto typedDesc = QueryTypedDesc< AVAssetDescConstPtr >( desc );

	assert( typedDesc );

    return AVAssetUtils::LoadThumbnail( typedDesc );
}




} //bv

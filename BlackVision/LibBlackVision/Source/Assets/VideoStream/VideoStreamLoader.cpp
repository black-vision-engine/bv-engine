#include "stdafx.h"

#include "VideoStreamLoader.h"
#include "VideoStreamUtils.h"


#include "Serialization/Json/JsonDeserializeObject.h"
#include "ProjectManager.h"



namespace bv
{


// ***********************
//
AssetConstPtr		VideoStreamLoader::LoadAsset			( const AssetDescConstPtr & desc ) const
{
	auto typedDesc = QueryTypedDesc< VideoStreamAssetDescConstPtr >( desc );

	assert( typedDesc );

    auto path = ProjectManager::GetInstance()->ToAbsPath( typedDesc->GetStreamPath() );
    
    if( !Path::IsFile( path ) )
        return false;

    return std::make_shared<VideoStreamAsset>( typedDesc->GetStreamPath() );
}

// ***********************
//
AssetDescConstPtr	VideoStreamLoader::CreateDescriptor	    ( const IDeserializer& deserializeObject ) const
{
    return std::static_pointer_cast<const AssetDesc>( VideoStreamAssetDesc::Create( deserializeObject ) );
}

// ***********************
//
ThumbnailConstPtr   VideoStreamLoader::LoadThumbnail        ( const AssetDescConstPtr & desc ) const
{
    auto typedDesc = QueryTypedDesc< VideoStreamAssetDescConstPtr >( desc );

	assert( typedDesc );

    return VideoStreamUtils::LoadThumbnail( typedDesc );
}




} //bv

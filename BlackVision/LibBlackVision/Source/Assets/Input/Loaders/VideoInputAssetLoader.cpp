#include "stdafx.h"
#include "VideoInputAssetLoader.h"


#include "Assets/Input/VideoInput/VideoInputTextureAsset.h"
#include "Assets/Input/VideoInput/VideoInputTextureAssetDesc.h"
#include "Assets/Input/VideoInput/VideoInputAssetDesc.h"
#include "Assets/Input/VideoInput/VideoInputAsset.h"

#include "Assets/AssetManager.h"



namespace bv
{

// ***********************
//
VideoInputAssetLoader::VideoInputAssetLoader  ( VideoInputSlotsPtr slots )
    : m_inputSlots( slots )
{}

// ***********************
//
AssetConstPtr           VideoInputAssetLoader::LoadAsset         ( const AssetDescConstPtr & desc ) const
{
    auto & type = desc->GetUID();
    if( type == VideoInputTextureAssetDesc::UID() )
    {
        auto typedDesc = std::static_pointer_cast< const VideoInputTextureAssetDesc >( desc );
        return m_inputSlots->CreateAsset( m_inputSlots, typedDesc );
    }
    else if( type == VideoInputAssetDesc::UID() )
    {
        auto typedDesc = std::static_pointer_cast< const VideoInputAssetDesc >( desc );

        auto fillDesc = typedDesc->CreateTextureDesc( videocards::VideoType::Fill );
        auto keyDesc = typedDesc->CreateTextureDesc( videocards::VideoType::Key );

        auto fillTexture = std::static_pointer_cast< const VideoInputTextureAsset >( AssetManager::GetInstance().LoadAsset( fillDesc ) );
        auto keyTexture = std::static_pointer_cast< const VideoInputTextureAsset >( AssetManager::GetInstance().LoadAsset( keyDesc ) );

        return VideoInputAsset::Create( fillTexture, keyTexture );
    }

    return AssetConstPtr();
}

// ***********************
//
AssetDescConstPtr       VideoInputAssetLoader::CreateDescriptor  ( const IDeserializer & deser ) const
{
    auto type = deser.GetAttribute( "type" );
    if( type == VideoInputTextureAssetDesc::UID() )
    {
        return std::static_pointer_cast< const AssetDesc >( VideoInputTextureAssetDesc::Create( deser ) );
    }
    else if( type == VideoInputAssetDesc::UID() )
    {

    }

    return nullptr;
}

// ***********************
//
ThumbnailConstPtr       VideoInputAssetLoader::LoadThumbnail     ( const AssetDescConstPtr & ) const
{
    return ThumbnailConstPtr();
}

}	// bv

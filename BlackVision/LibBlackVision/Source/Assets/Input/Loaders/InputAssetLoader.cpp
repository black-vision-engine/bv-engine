#include "stdafx.h"
#include "InputAssetLoader.h"


#include "Assets/Input/TextureInputAsset.h"
#include "Assets/Input/TextureInputAssetDesc.h"

#include "Assets/Input/AudioInputAssetDesc.h"
#include "Assets/Input/AudioInputAsset.h"


namespace bv
{

// ***********************
//
InputAssetLoader::InputAssetLoader  ( InputSlotsPtr slots )
    :   m_inputSlots( slots )
{}

// ***********************
//
AssetConstPtr           InputAssetLoader::LoadAsset         ( const AssetDescConstPtr & desc ) const
{
    if( desc->GetUID() == TextureInputAssetDesc::UID() )
    {
        auto typedDesc = std::static_pointer_cast< const TextureInputAssetDesc >( desc );
        return m_inputSlots->CreateAsset( m_inputSlots, typedDesc );
    }
    else if( desc->GetUID() == AudioInputAssetDesc::UID() )
    {
        auto typedDesc = std::static_pointer_cast< const AudioInputAssetDesc >( desc );
        return m_inputSlots->CreateAsset( m_inputSlots, typedDesc );
    }

    return AssetConstPtr();
}

// ***********************
//
AssetDescConstPtr       InputAssetLoader::CreateDescriptor  ( const IDeserializer & deser ) const
{
    auto type = deser.GetAttribute( "type" );
    if( type == TextureInputAssetDesc::UID() )
    {
        return std::static_pointer_cast< const AssetDesc >( TextureInputAssetDesc::Create( deser ) );
    }
    else if( type == AudioInputAssetDesc::UID() )
    {
        return std::static_pointer_cast< const AssetDesc >( AudioInputAssetDesc::Create( deser ) );
    }

    return nullptr;
}

// ***********************
//
ThumbnailConstPtr       InputAssetLoader::LoadThumbnail     ( const AssetDescConstPtr & ) const
{
    return ThumbnailConstPtr();
}

}	// bv

#include "AssetDescsWithUIDs.h"

#include "Engine/Serialization/SerializationObjects.inl"

namespace bv {

AssetDescsWithUIDs AssetDescsWithUIDs::instance;

template std::shared_ptr< AssetDescsWithUIDs >                                        DeserializeObjectLoadImpl( DeserializeObjectImpl*, std::string name );

void                                                    AssetDescsWithUIDs::AddAssetDesc( AssetDescConstPtr asset )
{
    auto uid = asset->GetProposedShortKey();
    if( m_uid2asset.find( uid ) == m_uid2asset.end() )
        AddAssetDescWithUID( AssetDescWithUID( asset, uid ) );
    else
        {
            auto baseUID = asset->GetProposedShortKey();
            int nTry = 1;
            std::string uid;
            while( uid = baseUID + std::to_string( nTry ), m_uid2asset.find( uid ) != m_uid2asset.end() )
            {
                if( m_uid2asset[ uid ]->GetKey() == asset->GetKey() )
                    return;
                nTry++;
            }
            AddAssetDescWithUID( AssetDescWithUID( asset, uid ) );            
        }
}

void                                                    AssetDescsWithUIDs::AddAssetDescWithUID( const AssetDescWithUID& assetWithUID )
{
    auto asset = assetWithUID.GetDesc();
    auto uid = assetWithUID.GetUID();

    assert( m_key2uid.find( asset->GetKey() ) == m_key2uid.end() );
    assert( m_uid2asset.find( uid ) == m_uid2asset.end() );

    m_key2uid[ asset->GetKey() ] = uid;
    m_uid2asset[ uid ] = asset;
}


}
#include "AssetDescsWithUIDs.h"

//#include "Serialization/SerializationObjects.inl"

namespace bv {

AssetDescsWithUIDs AssetDescsWithUIDs::instance;

template std::shared_ptr< AssetDescsWithUIDs >                                        DeserializeObjectLoadImpl( const IDeserializer&, std::string name );

// ********************************
//
void                                                    AssetDescsWithUIDs::AddAssetDesc( AssetDescConstPtr asset )
{
    if( m_key2uid.find( asset->GetKey() ) != m_key2uid.end() )
        return;

    auto baseUID = asset->GetProposedShortKey();
    std::string uid = baseUID;
    if( m_uid2asset.find( uid ) == m_uid2asset.end() )
        AddAssetDescWithUID( AssetDescWithUID( asset, uid ) );
    else
        {
            int nTry = 1;
            while( uid = baseUID + std::to_string( nTry ), m_uid2asset.find( uid ) != m_uid2asset.end() )
                nTry++;
            AddAssetDescWithUID( AssetDescWithUID( asset, uid ) );            
        }
}

// ********************************
//
void                                                    AssetDescsWithUIDs::AddAssetDescWithUID( const AssetDescWithUID& assetWithUID )
{
    auto asset = assetWithUID.GetDesc();
    auto uid = assetWithUID.GetUID();

    assert( m_key2uid.find( asset->GetKey() ) == m_key2uid.end() );
    assert( m_uid2asset.find( uid ) == m_uid2asset.end() );

    m_key2uid[ asset->GetKey() ] = uid;
    m_uid2asset[ uid ] = asset;
}

// ********************************
//
AssetDescVec                                            AssetDescsWithUIDs::GetAssetsDescs() const
{
    AssetDescVec ret;

    for( auto p : m_uid2asset )
    {
        ret.push_back( p.second );
    }

    return ret;
}

}
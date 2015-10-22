#pragma once

#include <map>

#include "AssetDescriptor.h"
#include "AssetSerialization.h"

namespace bv {

class AssetDescWithUID : public ISerializable
{
    AssetDescConstPtr desc;
    std::string uid;
public:
    AssetDescWithUID( AssetDescConstPtr desc, std::string uid ) : desc( desc ), uid( uid ) { }

    virtual void                            Serialize       ( SerializeObject & sob ) const
    {
        sob.SetName( "uid" );
        sob.SetValue( "uid", uid );
        desc->Serialize( sob );
        sob.Pop();
    }

    static ISerializablePtr                 Create          ( DeserializeObject & dob )
    {
        auto uid = dob.GetValue( "uid" );
        auto desc = dob.Load< const SerializedAssetDesc >( "asset" );
        return std::make_shared< AssetDescWithUID >( desc, uid );
    }

    AssetDescConstPtr                       GetDesc() const { return desc; }
    std::string                             GetUID() const { return uid; }
};

class AssetDescsWithUIDs : public ISerializable
{
    static AssetDescsWithUIDs instance;

    std::map< std::string, AssetDescConstPtr >              m_uid2asset;
    std::map< std::string, std::string >                    m_key2uid;

    void                                                    AddAssetDescWithUID( const AssetDescWithUID& asset );
public:
    static AssetDescsWithUIDs&                              GetInstance() { return instance; }
    static void                                             SetInstance( AssetDescsWithUIDs& i ) { instance = i; }

    virtual void                                            Serialize       ( SerializeObject & sob ) const
    {
        sob.SetName( "assets" );
        for( auto asset : m_uid2asset )
        {
            AssetDescWithUID( asset.second, asset.first ).Serialize( sob );
        }
        sob.Pop();
    }
    static ISerializablePtr                                 Create          ( DeserializeObject & dob )
    {
        auto assetsWithUIDs = dob.LoadProperties< AssetDescWithUID >( "uid" );

        auto assets = std::make_shared< AssetDescsWithUIDs >();
        for( auto asset : assetsWithUIDs )
            assets->AddAssetDescWithUID( *asset );
        return assets;
    }

    void                                                    AddAssetDesc( AssetDescConstPtr asset );

    AssetDescVec                                            GetAssetsDescs() const;

    std::string                                             Key2UID( std::string key ) { return m_key2uid[ key ]; }
    AssetDescConstPtr                                       UID2Asset( std::string uid ) { return m_uid2asset[ uid ]; }
};

} // bv
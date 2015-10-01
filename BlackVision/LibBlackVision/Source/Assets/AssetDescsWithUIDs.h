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

    AssetDescConstPtr                       GetDesc() { return desc; }
    std::string                             GetUID() { return uid; }
};

class AssetDescsWithUIDs : public ISerializable
{
    static AssetDescsWithUIDs instance;

    std::map< AssetDescConstPtr, std::string > asset2uid;
    std::map< std::string, AssetDescConstPtr > uid2asset;



public:
    static AssetDescsWithUIDs&              GetInstance() { return instance; }
    static void                             SetInstance( AssetDescsWithUIDs& i ) { instance = i; }

    virtual void                            Serialize       ( SerializeObject & sob ) const
    {
        sob.SetName( "assets" );
        for( auto asset : asset2uid )
        {
            AssetDescWithUID( asset.first, asset.second ).Serialize( sob );
        }
        sob.Pop();
    }
    static ISerializablePtr                 Create          ( DeserializeObject & dob )
    {
        auto assetsWithUIDs = dob.LoadProperties< AssetDescWithUID >( "uid" );

        auto assets = std::make_shared< AssetDescsWithUIDs >();
        for( auto asset : assetsWithUIDs )
            assets->AddAssetDescWithUID( asset->GetDesc(), asset->GetUID() );
        return assets;
    }

    void                                    AddAssetDescWithUID( AssetDescConstPtr asset, std::string uid )
    {
        asset2uid[ asset ] = uid;
        uid2asset[ uid ] = asset;
    }

    std::string                             GenerateUID( AssetDescConstPtr asset );

    std::string                             Asset2UID( AssetDescConstPtr asset ) { return asset2uid[ asset ]; }
    AssetDescConstPtr                       UID2Asset( std::string uid ) { return uid2asset[ uid ]; }
};

} // bv
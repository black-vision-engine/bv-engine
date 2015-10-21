#pragma once

#include <map>

#include "AssetDescriptor.h"
#include "AssetManager.h"
#include "Serialization/SerializationObjects.h"
#include "Serialization/SerializationObjects.inl"

namespace bv {

class AssetDescWithUID : public ISerializable
{
    AssetDescConstPtr desc;
    std::string uid;
public:
    AssetDescWithUID( AssetDescConstPtr desc, std::string uid ) : desc( desc ), uid( uid ) { }

    virtual void                            Serialize       ( ISerializer& sob ) const
    {
        sob.EnterChild( "uid" );
        sob.SetAttribute( "uid", uid );
        desc->Serialize( sob );
        sob.ExitChild();
    }

    static ISerializablePtr                 Create          ( ISerializer& dob )
    {
        auto uid = dob.GetAttribute( "uid" );
        
        //auto desc = DeserializeObjectLoadImpl< const SerializedAssetDesc >( dob, "asset" );
        auto desc = AssetManager::GetInstance().CreateDesc( dob );
        //dob.ExitChild(); // FIXME this so f***ing wrong
        
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

    virtual void                                            Serialize       ( ISerializer& sob ) const
    {
        sob.EnterChild( "assets" );
        for( auto asset : m_uid2asset )
        {
            AssetDescWithUID( asset.second, asset.first ).Serialize( sob );
        }
        sob.ExitChild();
    }
    static ISerializablePtr                                 Create          ( ISerializer& dob )
    {
        auto assetsWithUIDs = DeserializeObjectLoadPropertiesImpl< AssetDescWithUID >( dob, "uid" );

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
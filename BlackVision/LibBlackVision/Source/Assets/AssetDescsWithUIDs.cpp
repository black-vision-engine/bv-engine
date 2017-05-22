#include "stdafx.h"

#include "AssetDescsWithUIDs.h"

#include "Serialization/SerializationHelper.h"

#include "Engine/Models/BasicNode.h"



#include "Memory/MemoryLeaks.h"



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

    static ISerializablePtr                 Create          ( const IDeserializer& deser )
    {
        auto uid = deser.GetAttribute( "uid" );

        auto desc = AssetManager::GetInstance().CreateDesc( deser );
        
        // Outside world is responsible for handling this.
        //assert( desc );
        
        return std::make_shared< AssetDescWithUID >( desc, uid );
    }

    AssetDescConstPtr                       GetDesc() const { return desc; }
    std::string                             GetUID() const { return uid; }
};

// *******************************
//
void GetAssetsWithUIDs( AssetDescsWithUIDs & map, const model::BasicNode * root, bool recursive )
{
    if( root )
    {
        auto plugins = root->GetPluginList();
        for( unsigned int i = 0; i < root->GetNumPlugins(); i++ )
        {
            auto lassets = plugins->GetPlugin( i )->GetLAssets();
            for( auto lasset : lassets )
            {
                map.AddAssetDesc( lasset.assetDesc );
            }
        }

        auto effect = root->GetNodeEffect();

        if( effect != nullptr )
        {
            for( auto ea : effect->GetAssets() )
            {
                map.AddAssetDesc( ea );
            }
        }

        if( recursive )
        {
            for( unsigned int i = 0; i < root->GetNumChildren(); i++ )
            {
                GetAssetsWithUIDs( map, root->GetChild( i ), true );
            }
        }
    }
}

// *******************************
//
void GetAssetsWithUIDs( AssetDescsWithUIDs & map, const model::IPlugin * plugin )
{
    auto lassets = plugin->GetLAssets();
    for( auto lasset : lassets )
    {
        map.AddAssetDesc( lasset.assetDesc );
    }
}

// *******************************
//
AssetDescsWithUIDsPtr                          AssetDescsWithUIDs::Create          ()
{
    return AssetDescsWithUIDsPtr( new AssetDescsWithUIDs() );
}

// *******************************
//
AssetDescsWithUIDsPtr                          AssetDescsWithUIDs::Create          ( const IDeserializer & deser )
{
    auto assetsWithUIDs = SerializationHelper::DeserializeProperties< AssetDescWithUID >( deser, "uid" );

    auto assets = AssetDescsWithUIDs::Create();
    for( auto asset : assetsWithUIDs )
    {
        if( asset->GetDesc() != nullptr )
        {
            assets->AddAssetDescWithUID( asset->GetDesc(), asset->GetUID() );
        }
        else
        {
            auto deserContext = Cast< BVDeserializeContext* >( deser.GetDeserializeContext() );
            LOG_MESSAGE( SeverityLevel::warning ) << "Asset descriptor with UID: [" << asset->GetUID() << "] couldn't be created while loading scene: [" << deserContext->GetSceneName() << "].";
        }
    }

    return assets;
}

// *******************************
//
void                                            AssetDescsWithUIDs::Serialize       ( ISerializer & ser ) const
{
    ser.EnterArray( "assets" );
    for( auto asset : m_uid2asset )
    {
        AssetDescWithUID( asset.second, asset.first ).Serialize( ser );
    }
    ser.ExitChild();
}


AssetDescsWithUIDs AssetDescsWithUIDs::instance;

//template std::shared_ptr< AssetDescsWithUIDs >                                        DeserializeObjectLoadImpl( const IDeserializer&, std::string name );

// ********************************
//
void                                              AssetDescsWithUIDs::AddAssetDesc( AssetDescConstPtr asset )
{
    if( m_key2uid.find( asset->GetKey() ) != m_key2uid.end() )
        return;

    auto baseUID = asset->GetProposedShortKey();
    std::string uid = baseUID;
    if( m_uid2asset.find( uid ) == m_uid2asset.end() )
        AddAssetDescWithUID( asset, uid );
    else
        {
            int nTry = 1;
            while( uid = baseUID + std::to_string( nTry ), m_uid2asset.find( uid ) != m_uid2asset.end() )
                nTry++;
            AddAssetDescWithUID( asset, uid );            
        }
}

// ********************************
//
void                                                    AssetDescsWithUIDs::AddAssetDescWithUID( AssetDescConstPtr asset, std::string uid )
{
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
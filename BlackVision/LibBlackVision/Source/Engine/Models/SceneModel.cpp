#include "SceneModel.h"

#include "Assets/AssetDescsWithUIDs.h"

namespace bv { namespace model {

// *******************************
//
SceneModelPtr    SceneModel::Create( std::string name, model::TimelineManagerPtr pTimelineManager, model::BasicNodePtr pModelSceneRoot )
{
    return SceneModelPtr( new SceneModel( name, pTimelineManager, pModelSceneRoot ) );
}

// *******************************
//
SceneModel::SceneModel( std::string name, model::TimelineManagerPtr pTimelineManager, model::BasicNodePtr pModelSceneRoot )
    : m_name( name )
    , m_pTimelineManager( pTimelineManager )
    , m_pModelSceneRoot( pModelSceneRoot )
{
}

// *******************************
//
void GetAssetsWithUIDs( AssetDescsWithUIDs& map, model::BasicNodePtr root )
{
    auto plugins = root->GetPlugins();
    for( unsigned int i = 0; i < root->GetNumPlugins(); i++ )
    {
        auto assets = root->GetPlugins()->GetPlugin( i )->GetAssets();
        for( auto asset : assets )
        {
            map.AddAssetDesc( asset );
        }
    }

    for( unsigned int i = 0; i < root->GetNumChildren(); i++ )
        GetAssetsWithUIDs( map, root->GetChild( i ) );
}

// *******************************
//
void            SceneModel::Serialize           ( ISerializer& ser) const
{
ser.EnterChild( "scene" );

    ser.SetAttribute( "name", m_name );

    model::TimelineManager::SetInstance( m_pTimelineManager.get() );

    //auto& assets = AssetDescsWithUIDs::GetInstance();
    AssetDescsWithUIDs assets;
    GetAssetsWithUIDs( assets, m_pModelSceneRoot );
    AssetDescsWithUIDs::SetInstance( assets );

    assets.Serialize( ser );

    m_pTimelineManager->Serialize( ser );
    m_pModelSceneRoot->Serialize( ser );

ser.ExitChild();
}

// *******************************
//
ISerializablePtr        SceneModel::Create          ( const IDeserializer& deser )
{
// assets
    auto assets = SerializationHelper::DeserializeObjectLoadImpl< AssetDescsWithUIDs >( deser, "assets" );
    AssetDescsWithUIDs::SetInstance( *assets );

// timelines
    auto tm = SerializationHelper::DeserializeObjectLoadImpl< model::TimelineManager >( deser, "timelines" );
    TimelineManager::SetInstance( tm.get() );

// nodes
    auto node = SerializationHelper::DeserializeObjectLoadImpl< model::BasicNode >( deser, "node" );
    assert( node );

    auto obj = std::make_shared< SceneModel >( deser.GetAttribute( "name" ), tm, node );
    return ISerializablePtr( obj );
}

} // model
} // bv

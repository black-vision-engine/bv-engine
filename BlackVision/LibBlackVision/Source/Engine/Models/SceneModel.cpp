#include "SceneModel.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Assets/AssetDescsWithUIDs.h"
#include "Serialization/SerializationHelper.h"

namespace bv {

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

    model::TimelineManager::SetInstance( m_pTimelineManager );

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
    auto tm = model::TimelineManager::GetInstance();

    auto timelines = SerializationHelper::DeserializeObjectLoadArrayImpl< model::TimeEvaluatorBase< model::ITimeEvaluator > >( deser, "timelines" );
    for( auto timeline : timelines )
        tm->AddTimeline( timeline );

// nodes
    auto node = SerializationHelper::DeserializeObjectLoadImpl< model::BasicNode >( deser, "node" );
    assert( node );

    auto obj = new SceneModel( deser.GetAttribute( "name" ), tm, node );
    return ISerializablePtr( obj );
}

} // bv

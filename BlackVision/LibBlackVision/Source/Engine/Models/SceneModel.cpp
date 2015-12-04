#include "SceneModel.h"

#include "Assets/AssetDescsWithUIDs.h"
#include "Engine/Models/ModelSceneEditor.h"

#include "Serialization/CloneViaSerialization.h"

namespace bv { namespace model {

// *******************************
//
SceneModelPtr    SceneModel::Create		( std::string name, BasicNodePtr rootNode, Camera * camera )
{
    return std::make_shared< SceneModel >( name, rootNode, camera );
}

// *******************************
//
				SceneModel::SceneModel	( std::string name, BasicNodePtr rootNode, Camera * camera )
    : m_name( name )
    , m_sceneRootNode( rootNode )
	, m_camera( camera )
{
	m_modelSceneEditor = new ModelSceneEditor( m_sceneRootNode );
}

// *******************************
//
				SceneModel::~SceneModel	()
{
	delete m_modelSceneEditor;
}

// *******************************
//
void            SceneModel::Serialize           ( ISerializer& ser) const
{
ser.EnterChild( "scene" );

    ser.SetAttribute( "name", m_name );

    //model::TimelineManager::SetInstance( m_timelineManager.get() );

    //auto& assets = AssetDescsWithUIDs::GetInstance();
    AssetDescsWithUIDs assets;
    GetAssetsWithUIDs( assets, m_sceneRootNode );
    AssetDescsWithUIDs::SetInstance( assets );

    assets.Serialize( ser );

	//FIXME: shouldn't serialize timeline manager (all timelines), only one used in the scene
	model::TimelineManager::GetInstance()->Serialize( ser );
    m_sceneRootNode->Serialize( ser );                                    

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
	//FIXME: add deserialized timelines as child to global timeline?
    //TimelineManager::SetInstance( tm.get() );
	TimelineManager::GetInstance()->AddTimeline( tm->GetRootTimeline() );

// nodes
    auto node = SerializationHelper::DeserializeObjectLoadImpl< model::BasicNode >( deser, "node" );
    assert( node );

	//FIXME: pass nullptr as camera because we don't have camera model yet
    auto obj = std::make_shared< SceneModel >( deser.GetAttribute( "name" ), node, nullptr );

    return ISerializablePtr( obj );
}

// *******************************
//
model::SceneModelPtr		SceneModel::Clone		() const
{
	return CloneViaSerialization::Clone( this, "scene" );
}

// *******************************
//
BasicNodePtr				SceneModel::GetRootNode	() const
{
	return m_sceneRootNode;
}

// *******************************
//
const std::string &			SceneModel::GetName		() const
{
	return m_name;
}

// *******************************
//
Camera *					SceneModel::GetCamera              ()  const
{
    return m_camera;
}

// *******************************
//
ModelSceneEditor *			SceneModel::GetModelSceneEditor		() const
{
	return m_modelSceneEditor;
}

// *******************************
//
SceneModelPtr				SceneModel::CreateEmptyScene		( const std::string & name )
{
	//FIXME: timeevaluator and camera can be nullptr because they're not used yet
	return SceneModel::Create( name, BasicNode::Create( std::string(), nullptr ), nullptr );
}

} // model
} // bv

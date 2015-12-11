#include "SceneModel.h"

#include "Assets/AssetDescsWithUIDs.h"
#include "Engine/Models/ModelSceneEditor.h"

#include "Serialization/CloneViaSerialization.h"

namespace bv { namespace model {

// *******************************
//
SceneModelPtr    SceneModel::Create		( std::string name, Camera * camera )
{
    return std::make_shared< SceneModel >( name, camera );
}

// *******************************
//
				SceneModel::SceneModel	( std::string name, Camera * camera )
    : m_name( name )
    , m_sceneRootNode( nullptr )
    , m_timeline( new model::OffsetTimeEvaluator( name, TimeType( 0.0 ) ) )
	, m_camera( camera )
	, m_modelSceneEditor( nullptr )
{
	//m_modelSceneEditor = new ModelSceneEditor( m_sceneRootNode );
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

    //auto& assets = AssetDescsWithUIDs::GetInstance();
    AssetDescsWithUIDs assets;
    GetAssetsWithUIDs( assets, m_sceneRootNode );
    AssetDescsWithUIDs::SetInstance( assets );

    assets.Serialize( ser );

	m_timeline->Serialize( ser );

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

	//FIXME: pass nullptr as camera because we don't have camera model yet
    auto obj = std::make_shared< SceneModel >( deser.GetAttribute( "name" ), nullptr );

// timelines
	auto sceneTimeline = obj->GetTimeline();
    auto timelines = SerializationHelper::DeserializeObjectLoadArrayImpl< OffsetTimeEvaluator >( deser, "timelines" );
	for( auto timeline : timelines )
    {
		sceneTimeline->AddChild( timeline );
    }
    dynamic_cast< BVDeserializeContext* >( deser.GetDeserializeContext() )->m_sceneTimeline = sceneTimeline;

// nodes
    auto node = SerializationHelper::DeserializeObjectLoadImpl< model::BasicNode >( deser, "node" );
    assert( node );
	obj->SetRootNode( node );


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
void						SceneModel::SetRootNode	( BasicNodePtr rootNode )
{
	m_sceneRootNode = rootNode;

	if( m_modelSceneEditor )
	{
		delete m_modelSceneEditor;
	}
	m_modelSceneEditor = new ModelSceneEditor( m_sceneRootNode );
}

// *******************************
//
BasicNodePtr				SceneModel::GetRootNode	() const
{
	return m_sceneRootNode;
}

// *******************************
//
void						SceneModel::SetName		( std::string name )
{
	m_name = name;
	m_timeline->SetName( name );
}

// *******************************
//
const std::string &			SceneModel::GetName		() const
{
	return m_name;
}

// *******************************
//
OffsetTimeEvaluatorPtr		SceneModel::GetTimeline	()  const
{
    return m_timeline;
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
	auto scene = SceneModel::Create( name, nullptr );
	scene->SetRootNode( BasicNode::Create( std::string(), nullptr ) );
	return scene;
}

} // model
} // bv

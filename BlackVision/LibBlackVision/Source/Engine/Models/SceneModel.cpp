#include "SceneModel.h"

#include "Assets/AssetDescsWithUIDs.h"
#include "Engine/Models/ModelSceneEditor.h"

#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"

#include "Serialization/CloneViaSerialization.h"
#include "Serialization/BVSerializeContext.h"

namespace bv { namespace model {

const std::string       SceneModel::DEFAULT_ROOT_NAME = "root";
const std::string       SceneModel::DEFAULT_TRANSFORM_PLUGIN_NAME = "transform";

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
    , m_timeline( new model::OffsetTimeEvaluator( name, TimeType( 0.0 ) ) )
	, m_camera( camera )
	, m_modelSceneEditor( nullptr )
{
    m_sceneRootNode = BasicNode::Create( DEFAULT_ROOT_NAME, nullptr );
    m_sceneRootNode->AddPlugin( DefaultTransformPluginDesc::UID(), DEFAULT_TRANSFORM_PLUGIN_NAME, m_timeline );

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
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );

    ser.EnterChild( "scene" );

        ser.SetAttribute( "name", m_name );

        if( context->detailedInfo )
        {
            //auto& assets = AssetDescsWithUIDs::GetInstance();
            AssetDescsWithUIDs assets;
            GetAssetsWithUIDs( assets, m_sceneRootNode );
            AssetDescsWithUIDs::SetInstance( assets );

            assets.Serialize( ser );

            ser.EnterArray( "timelines" );
            for( auto timeline : m_timeline->GetChildren() )
                timeline->Serialize( ser );
            ser.ExitChild(); // timelines
        }

        if( m_sceneRootNode )
        {
            m_sceneRootNode->Serialize( ser );
        }

    ser.ExitChild();
}

// *******************************
//
SceneModel *        SceneModel::Create          ( const IDeserializer& deser )
{
// assets
    auto assets = SerializationHelper::DeserializeObject< AssetDescsWithUIDs >( deser, "assets" );
    AssetDescsWithUIDs::SetInstance( *assets );

	//FIXME: pass nullptr as camera because we don't have camera model yet
    auto obj = new SceneModel( deser.GetAttribute( "name" ), nullptr );

// timelines
	auto sceneTimeline = obj->GetTimeline();
    auto timelines = SerializationHelper::DeserializeArray< TimeEvaluatorBase< ITimeEvaluator > >( deser, "timelines" );

    for( auto timeline : timelines )
    {
		sceneTimeline->AddChild( timeline );
    }

    dynamic_cast< BVDeserializeContext* >( deser.GetDeserializeContext() )->m_sceneTimeline = sceneTimeline;

// nodes
    auto node = SerializationHelper::DeserializeObjectPtr< model::BasicNode >( deser, "node" );

    if( node )
    {
        obj->SetRootNode( node );
    }

	return obj;
}

// *******************************
//
model::SceneModel *		SceneModel::Clone		() const
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
	//FIXME:camera can be nullptr because it's not used yet
	return SceneModel::Create( name, nullptr );
}

} // model
} // bv

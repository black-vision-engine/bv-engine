#include "stdafx.h"

#include "SceneModel.h"

#include "Assets/AssetDescsWithUIDs.h"
#include "Engine/Models/ModelSceneEditor.h"

#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Timeline/TimelineHelper.h"

#include "Serialization/BV/CloneViaSerialization.h"
#include "Serialization/BV/BVSerializeContext.h"


namespace bv { namespace model {

// *******************************
//
SceneModelPtr    SceneModel::Create		( const std::string & name, Camera * camera )
{
    return std::make_shared< SceneModel >( name, camera );
}

// *******************************
//
				SceneModel::SceneModel	( const std::string & name, Camera * camera )
    : m_name( name )
    , m_timeline( new model::OffsetTimeEvaluator( name, TimeType( 0.0 ) ) )
	, m_camera( camera )
	, m_modelSceneEditor( nullptr )
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
void            SceneModel::Serialize           ( ISerializer & ser) const
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );

    ser.EnterChild( "scene" );

        ser.SetAttribute( "name", m_name );

        if( context->detailedInfo )
        {
            auto assets = std::make_shared< AssetDescsWithUIDs >();
            GetAssetsWithUIDs( *assets, m_sceneRootNode.get() );
            context->SetAssets( assets );

            assets->Serialize( ser );

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
SceneModel *        SceneModel::Create          ( const IDeserializer & deser )
{
    auto bvDeserCo = Cast< BVDeserializeContext* >( deser.GetDeserializeContext() );
// assets
    auto assets = SerializationHelper::DeserializeObjectPtr< AssetDescsWithUIDs >( deser, "assets" );
    bvDeserCo->SetAssets( assets );

	//FIXME: pass nullptr as camera because we don't have camera model yet
    auto sceneName = deser.GetAttribute( "name" );
    auto obj = new SceneModel( sceneName, nullptr );

    // Add scene name to context
    bvDeserCo->SetSceneName( sceneName );

// timelines
	auto sceneTimeline = obj->GetTimeline();
    auto timelines = SerializationHelper::DeserializeArray< TimeEvaluatorBase< ITimeEvaluator > >( deser, "timelines" );

    for( auto timeline : timelines )
    {
		sceneTimeline->AddChild( timeline );
    }

	bvDeserCo->SetSceneTimeline( sceneTimeline );

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
	return CloneViaSerialization::Clone( this, "scene", nullptr, nullptr ); // FIXME probably
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
void						SceneModel::SetName		( const std::string & name )
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

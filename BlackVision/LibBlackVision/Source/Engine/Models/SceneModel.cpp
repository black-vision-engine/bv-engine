#include "stdafx.h"

#include "SceneModel.h"
#include "ModelState.h"

#include "Assets/AssetDescsWithUIDs.h"
#include "Engine/Models/ModelSceneEditor.h"

#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Timeline/TimelineHelper.h"

#include "Engine/Models/Lights/ModelBaseLight.h"

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
    , m_timeline( model::OffsetTimeEvaluator::Create( name, TimeType( 0.0 ) ) )
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
    auto context = static_cast< BVSerializeContext * >( ser.GetSerializeContext() );

    ser.EnterChild( "scene" );

        ser.SetAttribute( "name", m_name );

        if( context->detailedInfo )
        {
            ser.EnterArray( "timelines" );
            for( auto timeline : m_timeline->GetChildren() )
            {
                timeline->Serialize( ser );
            }
            ser.ExitChild(); // timelines

            ser.EnterArray( "lights" );
            for( auto & light : m_lights )
            {
                light->Serialize( ser );
            }
            ser.ExitChild(); // lights

            m_sceneVariables.Serialize( ser );
        }

        if( m_sceneRootNode )
        {
            m_sceneRootNode->Serialize( ser );
        }

        if( context->detailedInfo )
        {
            context->GetAssets()->Serialize( ser );
        }


    ser.ExitChild();
}

// *******************************
//
SceneModelPtr        SceneModel::Create          ( const IDeserializer & deser )
{
    auto bvDeserCo = Cast< BVDeserializeContext* >( deser.GetDeserializeContext() );
// assets
    auto assets = SerializationHelper::DeserializeObject< AssetDescsWithUIDs >( deser, "assets" );
    bvDeserCo->SetAssets( assets );

	//FIXME: pass nullptr as camera because we don't have camera model yet
    auto sceneName = deser.GetAttribute( "name" );
    auto obj = SceneModel::Create( sceneName, nullptr );

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

// lights
    auto lights = SerializationHelper::DeserializeArray< model::ModelBaseLight >( deser, "lights" );
    for( auto light : lights )
    {
        obj->AddLight( light );
    }

// editor scene varables

    if( deser.EnterChild( "sceneVariables" ) )
    {
        auto & editorSceneVariables = obj->GetSceneVariables();
        editorSceneVariables.Deserialize( deser );
        deser.ExitChild();
    }

// nodes
    auto node = SerializationHelper::DeserializeObject< model::BasicNode >( deser, "node" );

    if( node )
    {
        obj->SetRootNode( node );
    }

	return obj;
}

// *******************************
//
model::SceneModelPtr		SceneModel::Clone		() const
{
	return CloneViaSerialization::Clone( this, "scene", nullptr, nullptr ); // FIXME probably
}

// *******************************
//
void						SceneModel::Update	    ( TimeType t )
{
    for( auto & light : m_lights )
    {
        light->Update( t );
    }
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
void                        SceneModel::AddLight            ( IModelLightPtr light ) 
{
    m_lights.push_back( light );
}

// *******************************
//
bool                        SceneModel::RemoveLight         ( UInt32 idx ) 
{
    if( idx < m_lights.size() )
    {
        m_lights.erase( m_lights.begin() + idx );
        return true;
    }
    return false;
}

// *******************************
//
IModelLight *               SceneModel::GetLight            ( UInt32 idx ) 
{
    if( idx < m_lights.size() )
    {
        return m_lights[ idx ].get();
    }

    return nullptr;
}

// *******************************
//
SizeType                    SceneModel::NumLights            () const
{
    return ( SizeType )m_lights.size();
}

// *******************************
//
ModelSceneEditor *			SceneModel::GetModelSceneEditor		() const
{
	return m_modelSceneEditor;
}

// *******************************
//
SceneVariables &            SceneModel::GetSceneVariables   ()
{
    return m_sceneVariables;
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

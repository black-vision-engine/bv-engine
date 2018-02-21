#include "stdafx.h"

#include "SceneModel.h"
#include "ModelState.h"

#include "Assets/AssetDescsWithUIDs.h"
#include "Engine/Editors/ModelSceneEditor.h"
#include "Engine/Models/Updaters/UpdatersHelpers.h"

#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Timeline/TimelineHelper.h"

#include "Engine/Models/Lights/ModelBaseLight.h"

#include "Serialization/BV/CloneViaSerialization.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Editors/EditorVariables/ParametersDescriptors/EndUserParamsLogic.h"

#include "Version/Version.h"
#include "Exceptions/InvalidSceneVersion.h"

#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

const UInt32 SceneModel::MAX_LIGTHS = 8;

// *******************************
//
SceneModelPtr    SceneModel::Create		( const std::string & name )
{
    return std::make_shared< SceneModel >( name );
}

// *******************************
//
SceneModel::SceneModel	( const std::string & name )
    : m_name( name )
    , m_timeline( model::OffsetTimeEvaluator::Create( name, TimeType( 0.0 ) ) )
    , m_modelSceneEditor( nullptr )
    , m_camerasLogic( m_timeline )
    , m_endUserParams( new EndUserParamsLogic( this ) )
    , m_renderChannelIdx( 0 )
{
    m_modelSceneEditor = new ModelSceneEditor( m_sceneRootNode );
}

// *******************************
//
                SceneModel::~SceneModel	()
{
    delete m_modelSceneEditor;
    delete m_endUserParams;
}

// *******************************
//
void            SceneModel::Serialize           ( ISerializer & ser) const
{
    auto context = static_cast< BVSerializeContext * >( ser.GetSerializeContext() );

    ser.EnterChild( "scene" );

        ser.SetAttribute( "name", m_name );
        ser.SetAttribute( "RenderChannelIdx", Convert::T2String( m_renderChannelIdx ) );

        auto version = Version::GetCurrentVersion();
        version.Serialize( ser );

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

            m_camerasLogic.Serialize( ser );
            m_sceneVariables.Serialize( ser );
            m_gridLinesLogic.Serialize( ser );
            m_endUserParams->Serialize( ser );
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

    // Check scene version
    Version version = Version::Create( deser );

    if( !version.IsValid() )
        bvDeserCo->AddWarning( std::make_shared< InvalidSceneVersion >( "This file does not contain scene version. It may be corrupted and loaded incorrectly." ) );
    else if( version.SerializerVersion != Version::GetCurrentVersion().SerializerVersion )
        bvDeserCo->AddWarning( std::make_shared< InvalidSceneVersion >( "Version of the scene file does not match engine version. This scene may be loaded incorrectly" ) );

    // Add scene name to context
    auto sceneName = deser.GetAttribute( "name" );
    bvDeserCo->SetSceneName( sceneName );

// assets
    auto assets = SerializationHelper::DeserializeObject< AssetDescsWithUIDs >( deser, "assets" );
    bvDeserCo->SetAssets( assets );

    //FIXME: pass nullptr as camera because we don't have camera model yet
    auto obj = SceneModel::Create( sceneName );

    obj->SetRenderChannelIdx( Convert::String2T< UInt32 >( deser.GetAttribute( "RenderChannelIdx" ), 0 ) );

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

// cameras
    auto & cameraLogic = obj->GetCamerasLogic();
    cameraLogic.Deserialize( deser );

// editor scene varables
    auto & editorSceneVariables = obj->GetSceneVariables();
    editorSceneVariables.Deserialize( deser );

// gridlines
    obj->GetGridLinesLogic().Deserialize( deser );

// nodes
    auto node = SerializationHelper::DeserializeObject< model::BasicNode >( deser, "node" );

    if( node )
    {
        obj->SetRootNode( node );
    }

    // end user params
    // Must be after attaching nodes.

    auto & endUserParams = obj->GetEndUserParams();
    endUserParams.Deserialize( deser );

    return obj;
}

// *******************************
//
model::SceneModelPtr		SceneModel::Clone		() const
{
    auto assets = std::make_shared< AssetDescsWithUIDs >();

    return CloneViaSerialization::Clone( this, "scene", assets, nullptr ); // FIXME probably
}

// *******************************
//
void						SceneModel::Update	    ( TimeType t )
{
    m_camerasLogic.Update( t );

    Camera tempCamera;
    UpdatersHelpers::UpdateCamera( &tempCamera, GetCamerasLogic().GetCurrentCamera() );

    for( auto & light : m_lights )
    {
        light->Update( t );
        light->UpdateToCameraSpace( tempCamera.GetViewMatrix() );
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
bool                        SceneModel::AddLight            ( IModelLightPtr light ) 
{
	if( m_lights.size() < MAX_LIGTHS - 1 )
	{
		m_lights.push_back( light );
		return true;
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::error ) << "Cannot add more than " << MAX_LIGTHS << " lights.";
		return false;
	}
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
IModelLightPtr              SceneModel::GetLight            ( UInt32 idx ) const
{
    if( idx < m_lights.size() )
    {
        return m_lights[ idx ];
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

// ***********************
//
GridLinesLogic &            SceneModel::GetGridLinesLogic   ()
{
    return m_gridLinesLogic;
}

// ***********************
//
CamerasLogic &              SceneModel::GetCamerasLogic     ()
{
    return m_camerasLogic;
}

// ***********************
//
EndUserParamsLogic &        SceneModel::GetEndUserParams    ()
{
    return *m_endUserParams;
}

// ***********************
//
OperationHistory &          SceneModel::GetHistory          ()
{
    return m_history;
}

// *******************************
//
SceneModelPtr				SceneModel::CreateEmptyScene		( const std::string & name )
{
    //FIXME:camera can be nullptr because it's not used yet
    return SceneModel::Create( name );
}

// *******************************
//
void                        SceneModel::SetRenderChannelIdx     ( UInt32 channelIdx )
{
    m_renderChannelIdx = channelIdx;
}

// *******************************
//
UInt32                      SceneModel::GetRenderChannelIdx     () const
{
    return m_renderChannelIdx;
}

} // model
} // bv

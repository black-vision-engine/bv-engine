#include "stdafx.h"

#include "BVProject.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Audio/AudioRenderer.h"

#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/BasicNode.h"
#include "MainFuckingRootNode.h"
#include "Engine/Editors/ModelSceneEditor.h"
#include "Engine/Editors/BVProjectEditor.h"

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Models/Lights/HelperModelLights.h"


namespace bv {

const std::string	BVProject::MAIN_ROOT_NAME			= "main root";
const std::string	BVProject::GLOBAL_TIMELINE_NAME	= "global timeline";

// *******************************
//
void                    BVProject::Serialize           ( ISerializer & ser ) const
{
    ser.EnterArray( "scenes" );

    for( auto & sm : m_sceneModelVec )
    {
        sm->Serialize( ser );
    }

    ser.ExitChild();
}

// *******************************
//
BVProjectPtr  BVProject::Create              ( Renderer * renderer, audio::AudioRenderer * audioRenderer, const IConfig* config )
{
    struct make_shared_enabler_BVProject : public BVProject { make_shared_enabler_BVProject( Renderer * renderer, audio::AudioRenderer * audioRenderer, const IConfig* config ): BVProject( renderer, audioRenderer, config ){} };
    return std::make_shared< make_shared_enabler_BVProject >( renderer, audioRenderer, config );
}

// *******************************
//
BVProject::BVProject    ( Renderer * renderer, audio::AudioRenderer * audioRenderer, const IConfig* config )
    : m_renderer( renderer )
    , m_audioRenderer( audioRenderer )
    , m_engineSceneRoot( nullptr )
    , m_timelineManager( std::make_shared < model::TimelineManager >() )
    , m_updaters( nullptr )
    , m_globalTimeline( model::OffsetTimeEvaluator::Create( GLOBAL_TIMELINE_NAME, TimeType( 0.0 ) ) )
{
    //initialize static managers in correct order
    GetDefaultEventManager();
    m_updaters = UpdatersManager::GetShared();

    m_timelineManager->RegisterRootTimeline( m_globalTimeline );
    model::TimelineManager::SetInstance( m_timelineManager.get() );

    m_rootNode = model::MainFuckingRootNode::Create( MAIN_ROOT_NAME );

    m_projectEditor = new BVProjectEditor( this, config );

    m_assetTracker = std::unique_ptr< AssetTracker >( new AssetTracker( renderer, audioRenderer, m_projectEditor ) );
}


// *******************************
//
BVProject::~BVProject         ()
{
    m_assetTracker = nullptr;       // force asset tracker destructor first

    delete m_engineSceneRoot;
    delete m_projectEditor;

	UpdatersManager::Deinitialize();
}

// *******************************
//
void            BVProject::Update( TimeType t )
{
    m_globalTimeline->SetGlobalTime( t );

    if( m_rootNode )
    {
        m_rootNode->Update( t );

        for( auto & scene : m_sceneModelVec )
        {
            scene->Update( t );
        }

        m_updaters->UpdateStep();

        auto scenes = GetScenes();
        auto identityTrans = Transform();

        for( auto & scene : scenes )
        {
            scene->GetRoot()->Update( identityTrans );
        }
    }
}

// *******************************
//
model::BasicNodePtr		BVProject::GetModelSceneRoot  () const
{
    return m_rootNode;
}

// *******************************
//
SceneNode *             BVProject::GetEngineSceneRoot   ()  const
{
    return m_engineSceneRoot;
}

// *******************************
//
BVProjectEditor *       BVProject::GetProjectEditor	    () const
{
    return m_projectEditor;
}

// *******************************
//
AssetTracker *          BVProject::GetAssetTracker      () const
{
    return m_assetTracker.get();
}

// *******************************
//
StringVector            BVProject::ListScenesNames      () const
{
    StringVector ret;
    for( auto & sm : m_sceneModelVec )
    {
        ret.push_back( sm->GetName() );
    }

    return ret;
}

// *******************************
//
model::SceneModelPtr    BVProject::GetModelScene            ( const std::string & name ) const
{
    for( unsigned int i = 0; i < m_sceneModelVec.size(); ++i )
    {
        if( m_sceneModelVec[ i ]->GetName() == name )
        {
            return m_sceneModelVec[ i ];
        }
    }
    return nullptr;
}

// *******************************
//
model::SceneModelPtr    BVProject::GetModelScene            ( UInt32 idx ) const
{
    if( idx < m_sceneModelVec.size() )
    {
        return m_sceneModelVec[ idx ];
    }
    return nullptr;
}

// *******************************
//
const model::SceneModelVec &    BVProject::GetModelScenes   () const
{
    return m_sceneModelVec;
}

// *******************************
//
SceneVec                        BVProject::GetScenes        () const
{
    SceneVec ret;
    for( auto & scenePtr : m_sceneVec )
    {
        ret.push_back( scenePtr.get() );
    }
    return ret;
}

// *******************************
//
void							BVProject::SetStartTime	    ( unsigned long millis )
{
    m_globalTimeline->SetTimeOffset( -TimeType( millis ) * TimeType( 0.001 ) );
}


// ***********************
//
void                            BVProject::RemoveNodeEffect ( SceneNode * engineNode )
{
    m_renderer->FreeNodeEffectPDR_DIRTY_HACK( engineNode->GetNodeEffect().get() );
}

// *******************************
//
void                            BVProject::AddEngineScene   ( std::unique_ptr< Scene > scene, UInt32 idx )
{
    if( idx < m_sceneVec.size() )
    {
        m_sceneVec.insert( m_sceneVec.begin() + idx, std::move( scene ) );
    }
    else
    {
        m_sceneVec.push_back( std::move( scene ) );
    }
}

// *******************************
//
void                            BVProject::RemoveEngineScene    ( Scene * scene )
{
    m_sceneVec.erase( std::remove_if( m_sceneVec.begin(), m_sceneVec.end(), [ & ] ( SceneUPtr const & scenePtr )
    { 
        return scenePtr.get() == scene; 
    }
    ), m_sceneVec.end() );
}

} // bv

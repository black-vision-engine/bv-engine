#include "stdafxBVApp.h"

#include "BVAppLogic.h"

#include "Services/BVServiceProvider.h"

// FIXME: nrl - render logic replacement
#include "Engine/Graphics/Effects/Logic/RenderLogicImpl.h"

#include "Engine/Models/NodeLogics/NodeLogicFactory.h"

#include "System/Env.h"
#include "ProjectManager.h"
#include "VideoCardManager.h"

#include "TestingScenes/MockScenes.h"
#include "DefaultPlugins.h"
#include "LibEffect.h"
#include "Assets/DefaultAssets.h"
#include "Assets/AssetManager.h"


//FIXME: remove
#include "TestAI/TestGlobalEffectKeyboardHandler.h"
#include "TestAI/TestEditorsKeyboardHandler.h"
#include "TestAI/TestVideoStreamDecoderKeyboardHandler.h"
#include "TestAI/TestInnerEvents.h"

#include "TestAI/TestVideoOutputKeyboardHandler.h"
//FIXME: end of remove

#include "UseLoggerBVAppModule.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "EndUserAPI/EventHandlers/RemoteEventsHandlers.h"
#include "EndUserAPI/JsonCommandsListener/JsonCommandsListener.h"

#include "Initialization/RenderLogicInitializer.h"




namespace bv
{


extern HighResolutionTimer GTimer;



// *********************************
//
BVAppLogic::BVAppLogic              ( Renderer * renderer, audio::AudioRenderer * audioRenderer )
    : m_bvProject( BVProject::Create( renderer, audioRenderer, &DefaultConfig ) )
    , m_pluginsManager( &model::PluginsManager::DefaultInstanceRef() )
    , m_renderer( nullptr )
    , m_audioRenderer( nullptr )
    , m_renderLogic( nullptr )
    , m_state( BVAppState::BVS_INVALID )
    , m_statsCalculator( DefaultConfig.StatsMAWindowSize() )
    , m_gain( 1.f )
    , m_videoCardManager( new videocards::VideoCardManager )
{
    GTimer.StartTimer();

    m_renderer = renderer;
    m_audioRenderer = audioRenderer;

    SetNodeLogicFactory( new NodeLogicFactory() );

    m_renderLogic = RenderLogicInitializer::CreateInstance( DefaultConfig );

    m_remoteHandlers = new RemoteEventsHandlers;
    m_remoteController = new JsonCommandsListener;

    model::ModelState::GetInstance().RegisterBVProject( m_bvProject.get() );
    //BVServiceProvider::GetInstance().RegisterBVProject( m_bvProject.get() );
}


// ========================================================================= //
// Initialization
// ========================================================================= //

// *********************************
//
void                    BVAppLogic::Initialize         ()
{
    m_renderMode.Init( m_renderLogic, m_renderer );

    model::PluginsManager::DefaultInstanceRef().RegisterDescriptors( model::DefaultBVPluginDescriptors() );

    bv::effect::InitializeLibEffect( m_renderer );

    InitializeKbdHandler();
    InitializeRemoteCommunication();
    InitializeCommandsDebugLayer();
    InitializeDefaultAssets();

    ProjectManager::SetPMFolder( DefaultConfig.PMFolder() );

    m_gain = DefaultConfig.GlobalGain();

    if( DefaultConfig.ReadbackFlag() )
    {
        //FIXME: maybe config should be read by bvconfig
        //FIXME: move this initialization to some other place
        auto & videCardsFactory = m_videoCardManager->GetFactory();

        videCardsFactory.RegisterDefaultCreators();
        auto descriptors = videCardsFactory.ReadDescriptorsFromConfig( DefaultConfig.GetNode( "config" ) );

        m_videoCardManager->CreateVideoCards( descriptors );
    }

    m_videoCardManager->Start();

    BVServiceProvider::GetInstance().RegisterVideoCardManager( m_videoCardManager );

    InitializeInputSlots();
}


// *********************************
//
void                    BVAppLogic::LoadScenes          ( const PathVec & pathVec )
{
    m_bvProject->GetProjectEditor()->RemoveAllScenes();

    for( auto p : pathVec )
    {
        auto scene = ProjectManager::GetInstance()->LoadScene( "", p );
        if( scene.IsValid() )
            m_bvProject->GetProjectEditor()->AddScene( scene );
        else
            LOG_MESSAGE_FILE_LINE( SeverityLevel::warning ) << "Unable to load scene: " << p << " - " << scene.GetErrorReason();
    }
}


// *********************************
//
void                    BVAppLogic::UnloadScenes        ()
{
    m_bvProject->GetProjectEditor()->RemoveAllScenes();
}

// *********************************
//
void                    BVAppLogic::LoadScene           ( void )
{
    auto projectEditor = m_bvProject->GetProjectEditor();
    auto pmSceneName = DefaultConfig.LoadSceneFromProjectManager();

    if( !pmSceneName.empty() )
    {
        auto pm = ProjectManager::GetInstance();
        auto sceneModel = pm->LoadScene( "", pmSceneName );

        if( sceneModel.IsValid() )
        {
            projectEditor->AddScene( sceneModel );
        }
        else
        {
            LOG_MESSAGE_FILE_LINE( SeverityLevel::error ) << "Unable to load scene: " << pmSceneName << ", reason: " << sceneModel.GetErrorReason();
        }
    }
    else if( DefaultConfig.LoadSceneFromEnv() )
    {
        model::SceneModelPtr sceneModel = nullptr;

        if( GetEnvScene() == "SERIALIZED_TEST" )
        {
            //sceneModel = TestScenesFactory::CreateSerializedTestScene( m_pluginsManager );
            //projectEditor->AddScene( sceneModel );
        }
        else
        {
            auto sceneName = "sceneFromEnv@ " + GetEnvScene();

            sceneModel = model::SceneModel::Create( sceneName );
            projectEditor->AddScene( sceneModel );

            auto defaultTimeline = projectEditor->GetSceneDefaultTimeline( sceneModel );
            auto node = TestScenesFactory::CreateSceneFromEnv( GetEnvScene(), defaultTimeline, m_pluginsManager );
            projectEditor->AddChildNode( sceneModel, nullptr, node );

            defaultTimeline->Play();
        }
    }
}

// *********************************
//
std::string                     BVAppLogic::GetEnvScene()
{
    auto s = DefaultConfig.SceneFromEnvName();
    if( s != "" )
        return s;
    else
        return Env::GetVar( DefaultConfig.DefaultSceneEnvVarName() );
}


// *********************************
//
void                            BVAppLogic::InitializeKbdHandler()
{
    auto envScene = GetEnvScene();

    if( envScene == "GLOBAL_EFFECT_05" )
    {
        m_kbdHandler = new TestGlobalEfectKeyboardHandler();
    }
    else if( envScene == "SERIALIZED_TEST" )
    {
        m_kbdHandler = new TestEditorsKeyboardHandler();
    }
    else if( envScene == "VIDEO_STREAM_TEST_SCENE" )
    {
        m_kbdHandler = new TestVideoStreamDecoderKeyboardHandler();
    }
    else if( envScene == "LIGHT_SCATTERING_EFFECT" )
    {
        m_kbdHandler = new TestEditorsKeyboardHandler();
    }
    else if( envScene == "GLOBAL_EFFECT_VIDEO_OUTPUT" )
    {
        m_kbdHandler = new TestVideoOutputKeyboardHandler();
    }
    else if( envScene == "SCENE_STRUCTURE_INNER_EVENTS" )
    {
        m_kbdHandler = new TestInnerEvents();
    }
    else
    {
        m_kbdHandler = new TestKeyboardHandler();
    }
}

// *********************************
//
void                            BVAppLogic::InitializeRemoteCommunication()
{
    m_remoteHandlers->InitializeHandlers( this );

    auto editorPort = DefaultConfig.SockerServerPort();
    m_remoteController->InitializeServer( editorPort );
}

// ***********************
//
void                            BVAppLogic::InitializeCommandsDebugLayer()
{
    if( DefaultConfig.UseDebugLayer() )
    {
        m_remoteController->InitializeDebugLayer( DefaultConfig.DebugFilePath() );
    }
}

// ***********************
//
void                            BVAppLogic::InitializeDefaultAssets()
{
    if( DefaultConfig.OnFailedTexLoadBehavior() == "LoadChecker" )
        DefaultAssets::Instance().InitTex2DFallback( DefaultAssets::Tex2DFallback::Checker );
    else if( DefaultConfig.OnFailedTexLoadBehavior() == "LoadTransparent" )
        DefaultAssets::Instance().InitTex2DFallback( DefaultAssets::Tex2DFallback::Transparent );
    else if( DefaultConfig.OnFailedTexLoadBehavior() == "LeavePrevious" )
        DefaultAssets::Instance().InitTex2DFallback( DefaultAssets::Tex2DFallback::None );
    else
        DefaultAssets::Instance().InitTex2DFallback( DefaultAssets::Tex2DFallback::Checker );
}

// ***********************
//
void                            BVAppLogic::InitializeInputSlots()
{
    RenderLogicInitializer::InitializeInputSlots( m_renderLogic, DefaultConfig, m_videoCardManager );
}

}	// bv




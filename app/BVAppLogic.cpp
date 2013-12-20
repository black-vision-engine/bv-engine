#include "BVAppLogic.h"

#include "Engine/Events/Interfaces/IEventManager.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/ModelScene.h"

#include "System/HerarchicalProfiler.h"

#include "MockScenes.h"

#include "System/HRTimer.h"
#include "BVConfig.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace bv
{
extern HighResolutionTimer GTimer;
namespace
{
    //FIXME: temporary
    char * GfbBuf = nullptr;

    TransformSetEventPtr  GTransformSetEvent;

    void GownoWFormieKebaba( TimeType t )
    {
        float tx = float( sin( t ) );
        glm::vec3 kebab( tx, 0.f, 0.f );
    
        //gowno
        GTransformSetEvent->SetTranslation( kebab );
    
        GetDefaultEventManager().QueueEvent( GTransformSetEvent );
    }
}

// *********************************
//
BVAppLogic::BVAppLogic              ()
    : m_startTime( 0 )
    , m_modelScene( nullptr )
    , m_mockSceneEng( nullptr )
    , m_state( BVAppState::BVS_INVALID )
    , m_statsCalculator( DefaultConfig.StatsMAWindowSize() )

{
    GTransformSetEvent = TransformSetEventPtr( new TransformSetEvent() );
    GfbBuf = new char[ 2048 * 2048 * 4 ]; //FIXME: naive hack
    GTimer.StartTimer();
}

// *********************************
//
BVAppLogic::~BVAppLogic             ()
{
    delete m_modelScene;
    delete m_mockSceneEng;

    delete[] GfbBuf;
}

// *********************************
//
void BVAppLogic::Initialize         ()
{
}

// *********************************
//
void BVAppLogic::LoadScene          ( void )
{
    //FIXME: czy ten caly smietnik jest tu potrzebny?
    //auto atlasCache = model::FontAtlasCache::Load( "fontcache.db" );
    //model::FontAtlasCacheData data( nullptr, 0, 0, 0 );

    //atlasCache->AddEntry( data, "dupa", 0, "dupa_file" );
    //atlasCache->GetEntry( "dupa", 0, "dupa_file" );

    //FIXME: remove me pleaZe
    //const std::string fontFile = "../dep/Media/fonts/arial.ttf";
    //simple_freetype_test();
    //basic_write_atlas( fontFile, "ascii_arial_atlas.raw" );


    //model::BasicNode * root = TestScenesFactory::SimpeTextTestScene();
    //model::BasicNode * root = TestScenesFactory::SimpleMultiCCScene();
    //model::BasicNode * root = TestScenesFactory::AnotherTestScene(); 
    //model::BasicNode * root = TestScenesFactory::XMLTestScene();
    //model::BasicNode * root = TestScenesFactory::TestSceneVariableTopology();
    //model::BasicNode * root = TestScenesFactory::AnimatedTestScene();

    /*
    model::BasicNode * rodos = TestScenesFactory::GreenRectTestScene();
    model::BasicNode * root0 = TestScenesFactory::SequenceAnimationTestScene();
    model::BasicNode * root1 = TestScenesFactory::NonGeometryParent();
    model::BasicNode * root3 = TestScenesFactory::AnimatedTestScene();
    model::BasicNode * rootu = TestScenesFactory::StackThemNow( root0, root1 );
    model::BasicNode * roota = TestScenesFactory::StackThemNow( root3, rootu );
    model::BasicNode * root  = TestScenesFactory::StackThemNow( roota, rodos );
    model::BasicNode * root = TestScenesFactory::TexturedRectTestScene();
    model::BasicNode * root = TestScenesFactory::NaiveTimerTestScene();
    */

    model::BasicNode * root = TestScenesFactory::AnotherTestScene(); 
    m_modelScene = model::ModelScene::Create( root, new Camera() );
    m_mockSceneEng = m_modelScene->GetSceneRoot()->BuildScene();    
}

// *********************************
//
void BVAppLogic::InitCamera         ( Renderer * renderer, int w, int h )
{
    Camera * cam = m_modelScene->GetCamera();

    renderer->SetCamera( cam );

    cam->SetFrame( DefaultConfig.CameraPosition(), DefaultConfig.CameraDirection(), DefaultConfig.CameraUp() );
    cam->SetPerspective( DefaultConfig.FOV(), float( w ) / float( h ), DefaultConfig.NearClippingPlane(), DefaultConfig.FarClippingPlane() );
    cam->SetFrame( glm::vec3( 0.f, 0.f, 1.3f ), glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );

    //FIXME: read from configuration file and change appropriately when resoultion changes
}

// *********************************
//
void BVAppLogic::SetStartTime       ( unsigned long millis )
{
    m_startTime = millis;
}

// *********************************
//
void BVAppLogic::OnUpdate           ( unsigned long millis, Renderer * renderer, HWND handle )
{
    //HPROFILER_FUNCTION( "BVAppLogic::OnUpdate" );

    assert( m_state != BVAppState::BVS_INVALID );

    if( m_state == BVAppState::BVS_RUNNING )
    {
        {
            FRAME_STATS_FRAME();
            FRAME_STATS_SECTION( DefaultConfig.FrameStatsSection() );

            //FIXME: debug timer - don't get fooled
            //float t = float(frame) * 0.1f; ///10 fps

            TimeType t = TimeType( millis ) * TimeType( 0.001 );
            GownoWFormieKebaba( t );

            {
                //FRAME_STATS_SECTION( "update total" );
                //HPROFILER_SECTION( "update total" );

                {
                    //FRAME_STATS_SECTION( "model Update" );
                    //HPROFILER_SECTION( "m_modelScene->Update" );

                    m_modelScene->Update( t );
                }
                {
                    //FRAME_STATS_SECTION( "updaters manager Update" );
                    //HPROFILER_SECTION( "UpdatersManager::Get().UpdateStep" );
                    UpdatersManager::Get().UpdateStep( t );
                }
                {
                    //FRAME_STATS_SECTION( "engine scene Update" );
                    //HPROFILER_SECTION( "m_mockSceneEng->Update" );

                    auto viewMat = m_modelScene->GetCamera()->GetViewMatrix();

                    //FIXME:
                    std::vector< bv::Transform > vec;
                    vec.push_back(Transform(viewMat, glm::inverse(viewMat)));
                    m_mockSceneEng->Update( t, vec );
                }
            }
            {
                //FRAME_STATS_SECTION( "Render" );
                //HPROFILER_SECTION( "Render" );

                renderer->ClearBuffers();
                RenderScene( renderer );
                renderer->DisplayColorBuffer();

                FrameRendered( renderer );
            }

        } //Frame Stats Collecting

//        if( m_statsCalculator )
//        {
//        }

        //static unsigned int frame = 0;
        //    static TimeType tt = TimeType( 0 );
        //    double elasp = GTimer.CurElapsed();
        //    if( elasp > 0.01 )
        //    {
        //        printf ( "%d %1.4f s -> g: %1.4f u: %1.4f r: %1.4f \n", frame, 1000. * elasp, 1000. * ( ge - gs ), 1000. * ( ue - us ), 1000. * ( GTimer.Re() ) );
        //        t = TimeType( 0 );
        //    }
   
        //    tt += t;
        //    frame++;

        DWORD ftime = timeGetTime() - millis;
        if( ftime < DefaultConfig.FrameTimeMillis() )
        {
            Sleep( DefaultConfig.FrameTimeMillis() - ftime );
            printf( "Sleeping: %d\n", DefaultConfig.FrameTimeMillis() - ftime );
        }
    }

    GTimer.StartTimer();
}

// *********************************
//
void BVAppLogic::OnKey           ( unsigned char c )
{
    //TODO: implement whatever you want here
}

// *********************************
//
void BVAppLogic::ChangeState     ( BVAppState state )
{
    if( state == BVAppState::BVS_INITALIZING )
    {
        assert( m_state != BVAppState::BVS_RUNNING );
    }

    m_state = state;
}

// *********************************
//
void BVAppLogic::ShutDown           ()
{
    //TODO: any required deinitialization
}

// *********************************
//
void BVAppLogic::FrameRendered      ( Renderer * renderer )
{
    static int w = 0;
    static int h = 0;

    if( !DefaultConfig.ReadbackFlag() )
    {
        return;
    }

    if( w != renderer->GetWidth() || h != renderer->GetHeight() )
    {
        w = renderer->GetWidth();
        h = renderer->GetHeight();

        printf( "Framebuffer resulotion changed to %dx%d\n", w, h );
    }

    static double totalElapsed = 0.0;
    static int nFrames = 1;
    static int nPasses = 0;

    double readbackStart = GTimer.CurElapsed();
    renderer->NaiveReadback( GfbBuf, w, h );
    double readbackTime = GTimer.CurElapsed() - readbackStart;

    totalElapsed += readbackTime;

    if( nFrames % 50 == 0 )
    {
        double avg = totalElapsed / (double) nFrames;

        nPasses++;
        totalElapsed = 0.0;

        if (nPasses % 3 == 0 )
        {
            nPasses = 0;

            printf( "Avg readback time from last %d frames took %.4f ms\n", nFrames, avg * 1000 );
        }

        nFrames = 0;
    }

    nFrames++;

    //TODO: code used to push data to playback cards
}

// *********************************
//
void    BVAppLogic::PostFrameLogic   ( unsigned int millis )
{
    if( m_statsCalculator.CurFrame() == DefaultConfig.MAVWarmupRounds() * m_statsCalculator.WindowSize() || m_statsCalculator.CurFrame() % DefaultConfig.StatsRecalcFramesDelta() == 0 )
    {
        m_statsCalculator.RecalculateStats();
        printf( "Recalc done\n" );
    }
    /*
    static unsigned int srame = 0;
    srame++;
    FrameStats stats;

    unsigned int frame = HPROFILER_GET_ACTIVE_FRAME();
    const ProfilerSample * samples = HPROFILER_GET_ONE_FRAME_SAMPLES( frame );

    double duration = samples[ 0 ].durationSecs;

    stats.frameMillis = float( duration * 1000.0 );
    stats.fps = 1.f / float( duration );

    if ( srame % 1500 == 0 )
    { 
        const ProfilerSample * samples = HPROFILER_GET_ONE_FRAME_SAMPLES( frame );
        unsigned int numSamples = HPROFILER_GET_NUM_SAMPLES();

        for( unsigned int i = 0; i < numSamples; ++i )
        {
            const ProfilerSample & sample = samples[ i ];
            const char * section = sample.type == AutoProfileType::APT_FUNCTION ? "F" : "S";

            for( unsigned int k = 0; k < sample.depth * 2; ++k )
                printf( " " );

            printf( "%s %s    %2.4f ms\n", section, sample.name, sample.durationSecs * 1000.0 );
            //printf( "%*s %s duration: %2.4f ms\n", sample.depth * 6, section, sample.name, sample.durationSecs * 1000.0 );
        }
    }
    */
    //return stats;

}

// *********************************
//
const FrameStatsCalculator &     BVAppLogic::FrameStats () const
{
    return m_statsCalculator;
}

// *********************************
//
void BVAppLogic::RenderScene     ( Renderer * renderer )
{
    renderer->PreDraw();
    RenderNode( renderer, m_mockSceneEng );
    renderer->PostDraw();

}

// *********************************
//
void BVAppLogic::RenderNode      ( Renderer * renderer, SceneNode * node )
{
    if ( node->IsVisible() )
    {
        //HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor" );
        renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetAnchor() ) );

        for( int i = 0; i < node->NumTransformables(); ++i )
        {
            //HPROFILER_SECTION( "RenderNode::renderer->Draw sibling" );
            renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable( i ) ) );
        }

        for ( int i = 0; i < node->NumChildrenNodes(); i++ )
        {
            //HPROFILER_SECTION( "RenderNode::RenderNode" );
            RenderNode( renderer, node->GetChild( i ) ); 
        }
    }
}

//// *********************************
////
//void BlackVisionApp::ReadBackFrameBuffer ()
//{
//    if ( !DefaultConfig.ReadbackFlag() )
//    {
//        return;
//    }
//
//    GframeRenderedEvent->SetResolution( m_Width, m_Height );
//    GEventManager->TriggerEvent( GframeRenderedEvent );
//}

} //bv

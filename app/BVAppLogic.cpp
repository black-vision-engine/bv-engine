#include "BVAppLogic.h"

#include "Engine/Events/Interfaces/IEventManager.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/ModelScene.h"
#include "MockScenes.h"

#include "System/HRTimer.h"
#include "BVConfig.h"

#define _USE_MATH_DEFINES
#include <math.h>


namespace bv
{

namespace
{

    bv::HighResolutionTimer GTimer;

    IEventManager *         GEventManager = nullptr;

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
{
    GTransformSetEvent = TransformSetEventPtr( new TransformSetEvent() );
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
    GEventManager = &bv::GetDefaultEventManager();

    //Initialize all known listeners
    GEventManager->AddListener( fastdelegate::MakeDelegate( this, &BVAppLogic::FrameRendered ), FrameRenderedEvent::Type() );
    //FIXME: temporary
    GfbBuf = new char[ 2048 * 2048 * 4 ];
}

// *********************************
//
void BVAppLogic::LoadScene          ( void )
{
    //auto atlasCache = model::FontAtlasCache::Load( "fontcache.db" );

    //model::FontAtlasCacheData data( nullptr, 0, 0, 0 );

    //atlasCache->AddEntry( data, "dupa", 0, "dupa_file" );
    //atlasCache->GetEntry( "dupa", 0, "dupa_file" );

    //FIXME: remove me pleaZe
    //const std::string fontFile = "../dep/Media/fonts/arial.ttf";
    //simple_freetype_test();
    //basic_write_atlas( fontFile, "ascii_arial_atlas.raw" );

    //model::BasicNode * root = TestScenesFactory::SimpeTextureTestScene();
    //model::BasicNode * root1 = TestScenesFactory::SimpeTextureTestScene();

    //root->AddChild( root1 );


    //model::BasicNode * root = TestScenesFactory::SimpeTextTestScene();
    //model::BasicNode * root = TestScenesFactory::SimpleMultiCCScene();
    model::BasicNode * root = TestScenesFactory::AnotherTestScene(); 
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
    */

    //model::BasicNode * root = TestScenesFactory::TexturedRectTestScene();
    //model::BasicNode * root = TestScenesFactory::GreenRectTestScene();
    //model::BasicNode * root = TestScenesFactory::NaiveTimerTestScene();

    //model::BasicNode * root = TestScenesFactory::SequenceAnimationTestScene();
    m_modelScene = model::ModelScene::Create( root, new Camera() );
    m_mockSceneEng = m_modelScene->GetSceneRoot()->BuildScene();    

}

// *********************************
//
void BVAppLogic::InitCamera         ( Renderer * renderer, int w, int h )
{
    renderer->SetCamera( m_modelScene->GetCamera() );

    //m_modelScene->GetCamera()->SetFrame( glm::vec3( 0.f, 0.f, 0.001f ), glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );
    //m_modelScene->GetCamera()->SetFrame( glm::vec3( 0.f, -4.f, 3.5f ), glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );
    m_modelScene->GetCamera()->SetFrame( glm::vec3( 0.f, 0.f, 1.3f ), glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );
//    AddCameraAnimation  ();
    //m_modelScene->GetCamera()->SetFrame( glm::vec3( 0.f, 0.f, 1.2f ), glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );
    //m_modelScene->GetCamera()->SetFrame( glm::vec3( 0.f, 0.f, 8.0f ), glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );
    //AddCameraAnimation  ();
    //AddCameraAnimation2  ();
    //FIXME: read from configuration file and change appropriately when resoultion changes
    m_modelScene->GetCamera()->SetPerspective( DefaultConfig.FOV(), float( w ) / float( h ), DefaultConfig.NearClippingPlane(), DefaultConfig.FarClippingPlane() );
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
    assert( m_state != BVAppState::BVS_INVALID );

    if( m_state == BVAppState::BVS_RUNNING )
    {
        static unsigned int frame = 0;
        static double totalPassed = 0.0;
        static unsigned int longestFrame = 0;
        static double longestTime = 0.;
        static unsigned int movingAvgAccum = 0;
        static unsigned int lastCount = 1;
        static DWORD movingAvgStart = timeGetTime();
        static float movingAvgTime = 0.001f;
        static DWORD startTime = timeGetTime();
        static bool init = true;
    
        if( init ) //FIXME: to jest szit dopiero
        {
            init = false;
            GTimer.StartTimer();
        }

        ++movingAvgAccum;

        //bv::Profiler pf("One frame " , &std::cout);

        //FIXME: debug timer - don't get fooled
        //float t = float(frame) * 0.1f; ///10 fps

        TimeType t = TimeType( millis - startTime ) * TimeType( 0.001 );
        GownoWFormieKebaba( t );

            m_modelScene->Update( t );

        double modelUpdate = GTimer.CurElapsed();

            UpdatersManager::Get().UpdateStep( t );

        double managerUpdate = GTimer.CurElapsed();

            auto viewMat = m_modelScene->GetCamera()->GetViewMatrix();

            //FIXME:
            std::vector< bv::Transform > vec;
            vec.push_back(Transform(viewMat, glm::inverse(viewMat)));
            m_mockSceneEng->Update( t, vec );
            //m_mockSceneEng->Update( t, Transform( viewMat, glm::inverse( viewMat ) ) );

        double engineUpdate = GTimer.CurElapsed();

            renderer->ClearBuffers();
            RenderScene( renderer );
            renderer->DisplayColorBuffer();

        DWORD ftime = timeGetTime() - millis;
        if( ftime < DefaultConfig.FrameTimeMillis() )
        {
            Sleep( DefaultConfig.FrameTimeMillis() - ftime );
            printf( "Sleeping: %d\n", DefaultConfig.FrameTimeMillis() - ftime );
        }

        double frameUpdate = GTimer.CurElapsed();

        GTimer.StopTimer();
        double frameTime = GTimer.GetElapsedTime();

        double modeldt = modelUpdate;
        double managerdt = managerUpdate - modelUpdate;
        double enginedt = engineUpdate - managerUpdate;
        double renderdt = frameUpdate - engineUpdate;

        totalPassed += frameUpdate;

        if( frameTime > longestTime )
        {
            longestTime = frameTime;
            longestFrame = frame;

            printf( "Longest frame: %d so far took: %f\n", frame, 1000. * frameTime );
        }

        if ( totalPassed > 0.2 )
        {
		    std::ostringstream  s;
            totalPassed = 0.0;

            std::cout.precision(4);
            s << "FPS: " << 1.0 / frameUpdate <<  " FPS: " << 1.0 / frameTime << " frame time: " << frameUpdate * 1000.0 << " ms longest frame: " << longestFrame << " took: " << longestTime * 1000.0;
        
            if( movingAvgAccum >= 38 )
            {
                lastCount = movingAvgAccum;
                movingAvgTime = (float(timeGetTime() - movingAvgStart) * 0.001f) / float(movingAvgAccum);
                //std::cout << "FPS: " << 1.0 / frameUpdate << std::endl;
                //std::cout << "Vertex "<<vertexCount<<" Model: " << modeldt * 1000.0 << "  Manager: " << managerdt * 1000.0 << "  Engine: " << enginedt * 1000.0 << " Render: " << renderdt * 1000.0 << " Total: " << frameUpdate * 1000.0 << std::endl;
		        
                movingAvgAccum = 0;
                movingAvgStart = timeGetTime();
            }
    
            s << "     Avg[" << lastCount << "] FPS: " << 1.0f / movingAvgTime << " frame time: " << 1000.0f * movingAvgTime << " ms" << std::endl;
            s << std::endl;

		    std::string ss = s.str();
		    std::wstring stemp = std::wstring( ss.begin(), ss.end() );
		    LPCWSTR sw = stemp.c_str();
		    SetWindowTextW(handle,sw);

        }

        frame++;

        GTimer.StartTimer();    
    }
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
void BVAppLogic::FrameRendered      ( IEventPtr evt )
{
    static int w = 0;
    static int h = 0;

    FrameRenderedEventPtr frameRenderedEvent = std::static_pointer_cast<FrameRenderedEvent>( evt );
    Renderer * renderer = frameRenderedEvent->Renderer();

    if( w != frameRenderedEvent->Width() || h != frameRenderedEvent->Height() )
    {
        w = frameRenderedEvent->Width();
        h = frameRenderedEvent->Height();

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

    //TODO: code used to push data to 
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
        renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetAnchor() ) );

        for( int i = 0; i < node->NumTransformables(); ++i )
        {
            renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable( i ) ) );
        }

        for ( int i = 0; i < node->NumChildrenNodes(); i++ )
        {
            RenderNode( renderer, node->GetChild( i ) ); 
        }
    }
}

} //bv

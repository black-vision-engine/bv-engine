#include "bvApp.h"

#include "System/InitSubsystem.h"
#include "System/Profiler.h"
#include "System/HRTimer.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/ModelScene.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Plugins/PluginsFactory.h"

#include "MockFonts/fttester.h"
#include "MockScenes.h"

#include "System/Threading/Thread.h"


bv::HighResolutionTimer GTimer;

#define FULLSCREEN_MODEL

#ifdef FULLSCREEN_MODE
    const bool          GFullScreen = true;
    const int           GWidth = 1920;
    const int           GHeight = 1080;
#else
    const bool          GFullScreen = false;
    const int           GWidth = 960;
    const int           GHeight = 540;
#endif

const unsigned int  FPS = 5000;
const unsigned int  GFrameMillis = 1000 / FPS;

#define _USE_MATH_DEFINES
#include <math.h>

// *********************************
// FIXME: move it to a valid BV windowed version of engine and wrap with a macro
void			bv::BlackVisionApp::StaticInitializer	()
{
    bv::ApplicationBase::MainFun = &bv::WindowedApplication::MainImpl;
    bv::ApplicationBase::ApplicationInstance = new bv::BlackVisionApp();
}

// *********************************
// FIXME: move it to valid BV windowed version of engine and wrap with a macro
bool			bv::BlackVisionApp::RegisterInitializer	()
{
    bv::InitSubsystem::AddInitializer( bv::BlackVisionApp::StaticInitializer );

    return true;
}

bool bv::BlackVisionApp::m_sWindowedApplicationInitialized = bv::BlackVisionApp::RegisterInitializer();

namespace bv {

// *********************************
//
BlackVisionApp::BlackVisionApp	()
    : WindowedApplication( "Migajace tlo z klirem dzialajacym jak nalezy to nalezy", 0, 0, GWidth, GHeight, GFullScreen )
    , m_modelScene(nullptr)
    , m_mockSceneEng(nullptr)
{
}

// *********************************
//
void BlackVisionApp::OnIdle		()
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

    if( frame == 1 )
    {
        totalPassed = 0;
        longestFrame = 0;
        longestTime = 0.;
        startTime = timeGetTime();
        movingAvgAccum = 0;
        movingAvgStart = startTime;
        GTimer.StartTimer();
    }

    ++movingAvgAccum;


    //bv::Profiler pf("One frame " , &std::cout);

    DWORD curTime = timeGetTime();

    //FIXME: debug timer - don't get fooled
    //float t = float(frame) * 0.1f; ///10 fps

    TimeType t = TimeType(curTime - startTime) * TimeType(0.001);

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

        m_Renderer->ClearBuffers();
        RenderScene();
        m_Renderer->DisplayColorBuffer();

    DWORD ftime = timeGetTime() - curTime;
    if( ftime < GFrameMillis )
    {
        Sleep( GFrameMillis - ftime );
        printf( "Sleeping: %d\n", GFrameMillis - ftime );
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

		string ss = s.str();
		std::wstring stemp = std::wstring(ss.begin(), ss.end());
		LPCWSTR sw = stemp.c_str();
		SetWindowTextW(handle,sw);

    }

    frame++;

    GTimer.StartTimer();
}

// *********************************
//
bool BlackVisionApp::RenderScene        ()
{
    m_Renderer->PreDraw();
    bool bSuccess = RenderNode( m_mockSceneEng );
    m_Renderer->PostDraw();

    return bSuccess;
}

// *********************************
//
bool BlackVisionApp::RenderNode         ( SceneNode *   node )
{
    bool bSuccess = true;
    
    if ( node->IsVisible() )
    {
        m_Renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetAnchor() ) );


        for(int i = 0; i < node->NumTransformables(); ++i)
        {
            bSuccess &= m_Renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable( i ) ) );
        }

        for (int i = 0; i < node->NumChildrenNodes(); i++)
        {
            bSuccess &= RenderNode( node->GetChild( i ) ); 
        }
    }

    return bSuccess;
}

// *********************************
//FIXME: implement proper console
bool BlackVisionApp::OnInitialize       ()
{
    if(AllocConsole())
    {
        FILE * dummy;
        SetConsoleTitleA("Debug Console");
        freopen_s(&dummy, "CONOUT$", "wb", stdout);
        freopen_s(&dummy, "CONOUT$", "wb", stderr);
    }

    //FIXME: remove me pleaZe
    //const std::string fontFile = "../dep/Media/fonts/arial.ttf";
    //simple_freetype_test();
    //basic_write_atlas( fontFile, "ascii_arial_atlas.raw" );

    //model::BasicNode * root = TestScenesFactory::SimpeTextureTestScene();
    //model::BasicNode * root1 = TestScenesFactory::SimpeTextureTestScene();

    //root->AddChild( root1 );


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
    */

    //model::BasicNode * root = TestScenesFactory::TexturedRectTestScene();
    //model::BasicNode * root = TestScenesFactory::GreenRectTestScene();
    //model::BasicNode * root = TestScenesFactory::NaiveTimerTestScene();

    model::BasicNode * root = TestScenesFactory::SequenceAnimationTestScene();
    m_modelScene = model::ModelScene::Create( root, new Camera() );

    m_Renderer->SetCamera( m_modelScene->GetCamera() );

    m_mockSceneEng = m_modelScene->GetSceneRoot()->BuildScene();    
    //m_modelScene->GetCamera()->SetFrame( glm::vec3( 0.f, 0.f, 0.001f ), glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );
    //m_modelScene->GetCamera()->SetFrame( glm::vec3( 0.f, -4.f, 3.5f ), glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );
    m_modelScene->GetCamera()->SetFrame( glm::vec3( 0.f, 0.f, 1.3f ), glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );
//    AddCameraAnimation  ();
    //m_modelScene->GetCamera()->SetFrame( glm::vec3( 0.f, 0.f, 1.2f ), glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );
    //m_modelScene->GetCamera()->SetFrame( glm::vec3( 0.f, 0.f, 8.0f ), glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );
    //AddCameraAnimation  ();
    //AddCameraAnimation2  ();
    //FIXME: read from configuration file and change appropriately when resoultion changes
    m_modelScene->GetCamera()->SetPerspective( 90.f, float(m_Width) / float(m_Height), 0.1f, 100.f );

    return WindowedApplication::OnInitialize();
}

// *********************************
//
void BlackVisionApp::OnTerminate        ()
{
#ifdef DEBUG_INFO
    m_file.close();
#endif
    model::TimelineManager::Cleanup();
    WindowedApplication::OnTerminate();
}

// *********************************
//
void BlackVisionApp::AddCameraAnimation  ()
{
    Vec3Interpolator * position     = new Vec3Interpolator();
    Vec3Interpolator * direction    = new Vec3Interpolator();
    Vec3Interpolator * up           = new Vec3Interpolator();

    TestParametersFactory::CameraAnimation_ver1( position, direction, up );

    m_modelScene->SetCamereParameters(  model::ParametersFactory::CreateParameter( "position", *position )
                                      , model::ParametersFactory::CreateParameter( "direction", *direction )
                                      , model::ParametersFactory::CreateParameter( "up", *up ) );
}

// *********************************
//
void BlackVisionApp::AddCameraAnimation2  () //smietnik lekko oczyszczony ze smieci
{
    Vec3Interpolator * position     = new Vec3Interpolator();
    Vec3Interpolator * direction    = new Vec3Interpolator();
    Vec3Interpolator * up           = new Vec3Interpolator();

    TestParametersFactory::CameraAnimation_ver2( position, direction, up );

    m_modelScene->SetCamereParameters(  model::ParametersFactory::CreateParameter( "position", *position )
                                      , model::ParametersFactory::CreateParameter( "direction", *direction )
                                      , model::ParametersFactory::CreateParameter( "up", *up ) );
}

}

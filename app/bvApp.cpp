#include "bvApp.h"
#include "Renderer.h"
#include "InitSubsystem.h"
#include "Engine\Models\BasicNode.h"
#include "Engine\Models\ModelFactory.h"

#include "Engine\Models\BasicNode.h"
#include "Mathematics\Transform\MatTransform.h"
#include "Engine\Graphics\SceneGraph\Camera.h"
#include "Engine\Models\Updaters\IUpdater.h"
#include "Engine/Models/ModelScene.h"
#include "System/Profiler.h"
#include "System/HRTimer.h"
#include "MockScenes.h"

#include "MockFonts\fttester.h"

bv::HighResolutionTimer GTimer;

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
    : WindowedApplication( "Migajace tlo z klirem dzialajacym jak nalezy to nalezy", 0, 0, 800, 600 )
    , m_modelScene(nullptr)
    , m_mockSceneEng(nullptr)
{
}

// *********************************
//
void BlackVisionApp::OnIdle		()
{
    static double totalPassed = 0.0;

    GTimer.StartTimer();

    //bv::Profiler pf("One frame " , &std::cout);

    static DWORD startTime = GetTickCount();
    DWORD curTime = GetTickCount();

    float t = float(curTime - startTime) * 0.001f;

        m_modelScene->Update( t );

    double modelUpdate = GTimer.CurElapsed();

        UpdatersManager::get().Update( t );

    double managerUpdate = GTimer.CurElapsed();

        auto viewMat = m_modelScene->GetCamera()->GetViewMatrix();

        //FIXME:
        std::vector< bv::Transform > vec;
        vec.push_back(Transform(viewMat, glm::inverse(viewMat)));
        m_mockSceneEng->Update( t, vec);
        //m_mockSceneEng->Update( t, Transform( viewMat, glm::inverse( viewMat ) ) );

    double engineUpdate = GTimer.CurElapsed();

        m_Renderer->ClearBuffers();
        RenderScene();
        m_Renderer->DisplayColorBuffer();

    double frameUpdate = GTimer.CurElapsed();

    double modeldt = modelUpdate;
    double managerdt = managerUpdate - modelUpdate;
    double enginedt = engineUpdate - managerUpdate;
    double renderdt = frameUpdate - engineUpdate;

    totalPassed += frameUpdate;

    if ( totalPassed > 0.2 )
    {
        totalPassed = 0.0;
        std::cout.precision(4);
        //std::cout << "FPS: " << 1.0 / frameUpdate << std::endl;
        //std::cout << "Vertex "<<vertexCount<<" Model: " << modeldt * 1000.0 << "  Manager: " << managerdt * 1000.0 << "  Engine: " << enginedt * 1000.0 << " Render: " << renderdt * 1000.0 << " Total: " << frameUpdate * 1000.0 << std::endl;
        std::cout << "FPS: " << 1.0 / frameUpdate << " frame time: " << frameUpdate * 1000.0 << std::endl;
    }
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
    m_Renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetAnchor() ) );

    bool bSuccess = true;

    for(int i = 0; i < node->NumTransformables(); ++i)
    {
        bSuccess &= m_Renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable( i ) ) );
    }

    for (int i = 0; i < node->NumChildrenNodes(); i++)
    {
        bSuccess &= RenderNode( node->GetChild( i ) ); 
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
    model::BasicNode * root = TestScenesFactory::SimpleMultiCCScene();

    m_modelScene = model::ModelScene::Create( root, new Camera() );

    m_Renderer->SetCamera( m_modelScene->GetCamera() );

    m_mockSceneEng = m_modelScene->GetSceneRoot()->BuildScene();    
    m_modelScene->GetCamera()->SetFrame( glm::vec3( 0.f, 0.f, 1.5f ), glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );

    //FIXME: read from configuration file and change appropriately when resoultion changes
    m_modelScene->GetCamera()->SetPerspactive( 90.f, float(m_Width) / float(m_Height), 0.1f, 100.f );

    //FIXME: state changes should be implemented some other way around
    m_Renderer->DisableFaceCulling();

    return WindowedApplication::OnInitialize();
}

void BlackVisionApp::OnTerminate        ()
{
#ifdef DEBUG_INFO
    m_file.close();
#endif
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

    m_modelScene->AddCameraInterpolators( position, direction, up );
}

// *********************************
//
void BlackVisionApp::AddCameraAnimation2  () //smietnik lekko oczyszczony ze smieci
{
    Vec3Interpolator * position     = new Vec3Interpolator();
    Vec3Interpolator * direction    = new Vec3Interpolator();
    Vec3Interpolator * up           = new Vec3Interpolator();

    TestParametersFactory::CameraAnimation_ver2( position, direction, up );

    m_modelScene->AddCameraInterpolators( position, direction, up );
}

}

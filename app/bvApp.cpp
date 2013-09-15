#include "bvApp.h"
#include "Renderer.h"
#include "InitSubsystem.h"
#include "Engine\Models\BasicNode.h"
#include "Engine\Models\ModelFactory.h"

#include "Engine\Models\Plugins\Solid.h"
#include "Engine\Models\Plugins\Extrude.h"
#include "Engine\Models\Plugins\Transform\PluginTransformSimple.h"
#include "Engine\Models\BasicNode.h"
#include "Mathematics\Transform\MatTransform.h"
#include "Engine/Models/Plugins/Geometry/PluginGeometryRect.h"
#include "Engine\Graphics\SceneGraph\Camera.h"
#include "Engine\Models\Updaters\IUpdater.h"
#include "Engine/Models/ModelScene.h"
#include "System/Profiler.h"
#include "System/HRTimer.h"
#include "MockScenes.h"

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
        m_mockSceneEng->Update( t, Transform( viewMat, glm::inverse( viewMat ) ) );

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
//
bool BlackVisionApp::OnInitialize       ()
{
    if(AllocConsole())
    {
        FILE * dummy;
        SetConsoleTitleA("Debug Console");
        freopen_s(&dummy, "CONOUT$", "wb", stdout);
        freopen_s(&dummy, "CONOUT$", "wb", stderr);
    }

    //bv::BasicNode* root = SceneExamples::CreateSceneMock3(nullptr);

    bv::BasicNode* root = SceneExamples::BuildMockScene();

    m_modelScene = ModelScene::Create(root, new Camera());
    m_Renderer->SetCamera(m_modelScene->GetCamera());
    m_mockSceneEng = m_modelScene->GetSceneRoot()->buildScene();
    
    m_modelScene->GetCamera()->SetFrame(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));


    m_modelScene->GetCamera()->SetPerspactive(90.f, float(m_Width) / float(m_Height), 0.1f, 100.f);

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

namespace {
    float AddRotKeys( bv::Vec3Interpolator * it, float startAngle, double endAngle, float speed, float & t )
    {
        float alpha = startAngle;
        for(; alpha <= (float) endAngle ; alpha += speed )
        {
            float z = 2.5f * cosf(alpha);
            float x = 2.5f * sinf(alpha);
            int dev = rand();
            it->addKey(t, glm::vec3(x, float(dev) / float(RAND_MAX), z));
            t += 0.1f;
        }

        it->addKey(t, glm::vec3(1.f, 1.f, 1.f));

        return alpha;
    }
}

void BlackVisionApp::AddCameraAnimation  ()
{
    bv::Vec3Interpolator* position = new bv::Vec3Interpolator();

    position->setWrapMethod(WrapMethod::repeat, WrapMethod::repeat);

    float t = 0.f;
    double endTime = 2. * M_PI;
    double dt = endTime / 10.f;

    float alpha = AddRotKeys( position, 0.f, dt, 0.05f, t );
    alpha = AddRotKeys( position, alpha, 3. * dt, 0.5f, t );
    alpha = AddRotKeys( position, alpha, 5. * dt, 0.05f, t );
    alpha = AddRotKeys( position, alpha, 7. * dt, 0.5f, t );
    alpha = AddRotKeys( position, alpha, 8. * dt, 0.01f, t );
    alpha = AddRotKeys( position, alpha, endTime, 0.2f, t );
    alpha = AddRotKeys( position, 0.f, 2. * M_PI, 0.05f, t );
 
    //float alpha = 0.f;
    //for(float alpha = 0.f; alpha <= 2 * M_PI ; alpha += 0.05f)
    //{
    //    float z = 2 * cosf(alpha);
    //    float x = 2 * sinf(alpha);
    //    int dev = rand();
    //    position->addKey(t, glm::vec3(x, float(dev) / float(RAND_MAX), z));
    //    t += 0.1f;
    //}


    bv::Vec3Interpolator* direction = new bv::Vec3Interpolator();

    direction->addKey(0.f, glm::vec3(0.f, 0.f, 0.f));


    bv::Vec3Interpolator* up = new bv::Vec3Interpolator();

    up->addKey(0.f, glm::vec3(0.f, 1.f, 0.f));

    m_modelScene->AddCameraInterpolators(direction, position, up);
}


void BlackVisionApp::AddCameraAnimation2  () //smietnik
{
    bv::Vec3Interpolator* position = new bv::Vec3Interpolator();

    position->setWrapMethod(WrapMethod::repeat, WrapMethod::pingPong);

    float t = 0.f;
    double endTime = 2. * M_PI;
    double dt = endTime / 10.f;
    position->addKey(0.f, glm::vec3(-2.f, 0.1f, 3.f));
    position->addKey(20.f, glm::vec3(2.f, 0.1f, 3.f));
    

    //float alpha = 0.f;
    //for(float alpha = 0.f; alpha <= 2 * M_PI ; alpha += 0.05f)
    //{
    //    float z = 2 * cosf(alpha);
    //    float x = 2 * sinf(alpha);
    //    int dev = rand();
    //    position->addKey(t, glm::vec3(x, float(dev) / float(RAND_MAX), z));
    //    t += 0.1f;
    //}


    bv::Vec3Interpolator* direction = new bv::Vec3Interpolator();

    direction->addKey(0.f, glm::vec3(0.f, 0.f, 0.f));


    bv::Vec3Interpolator* up = new bv::Vec3Interpolator();

    up->addKey(0.f, glm::vec3(0.f, 1.f, 0.f));
    up->addKey(3.f, glm::vec3(0.f, 1.f, 0.f));

    m_modelScene->AddCameraInterpolators(direction, position, up);
}

}

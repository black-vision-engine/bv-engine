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

#pragma warning (disable: 4996) // 'freopen': This function or variable may be unsafe. Consider using freopen_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.

//#define DEBUG_INFO

// *********************************
// FIXME: move it to a valid BV windowed version of engine and wrap with a macro
namespace
{

void RenderMockScene(bv::BasicNode* tree, std::ostream& out)
{
    static float t = 0.f;

    tree->Update(t);

    out << "---------------------------------TIME : " << t << " ---------------------------";

    out << std::endl;

    tree->Print(out);

    out << "----------END TREE------------" << std::endl << std::endl;

    t += 0.1f;
}

bv::BasicNode* BuildMockScene(bv::BasicNode* parent = nullptr)
{
    bv::BasicNode* mockTree = new bv::BasicNode();

    // SOLID
    bv::Vec4Interpolator color;

    float rs = 10.f;

    color.addKey(rs * .25f, glm::vec4(1,0.4,0.5,1));
    color.addKey(rs *.5f, glm::vec4(0.3,0.4,0.5,1));
    color.addKey(rs *.75f, glm::vec4(0.1,1.0,0.8,1));
    color.addKey(rs *.95f, glm::vec4(0.7,0.2,0.4,1));
    color.addKey(rs *1.15f, glm::vec4(0.4,0.1,1.,1));
    color.addKey(rs *1.33f, glm::vec4(1,1.0,0.1,1));
    color.addKey(rs *1.6f, glm::vec4(1,0.4,0.2,1));
    color.addKey(rs *1.9f, glm::vec4(0,0.4,1.0,1));

    color.setWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);

    bv::SolidColorPlugin* solid1 = new bv::SolidColorPlugin(color);

    mockTree->setPixelShaderPlugin(solid1);

    bv::FloatInterpolator si;

    si.setWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
    float s = 5.0f;
    //si.addKey(0.f, 0.5f);si.addKey(0.4f, 0.5f);si.addKey(0.5f, s * 0.6f);si.addKey(0.65f, 0.4f);si.addKey(0.8f, s * 0.9f);si.addKey(0.95f, 0.35f);si.addKey(1.05f, 0.5f);
    si.addKey(0.f, 0.1f);
    si.addKey(2.4f, 2.5f);//si.addKey(0.5f, s * 0.6f);si.addKey(0.65f, 0.4f);si.addKey(0.8f, s * 0.9f);si.addKey(0.95f, 0.35f);si.addKey(1.05f, 0.5f);
    
    mockTree->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));

    mockTree->addGeometryPlugin(new bv::PluginGeometryRect());

    // TRANSFORMATION
    bv::FloatInterpolator angle; angle.setWrapPostMethod(bv::WrapMethod::pingPong);
    bv::FloatInterpolator x;
    bv::FloatInterpolator y;
    bv::FloatInterpolator z;

    x.addKey(0.f, 0.f);
    y.addKey(0.f, 0.f);
    z.addKey(0.f, -1.f);
    angle.addKey(0.f, 0.f);
    angle.addKey(5.f, 180.f);

    bv::TransformF* trans = new bv::TransformF();
    trans->addTransform(new bv::RotationF(angle, x,y,z));
    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);

    mockTree->addTransformPlugin(transPlugin);

    if(parent != nullptr)
    {
        parent->addChild(mockTree);
    }

    return mockTree;
}

bv::BasicNode* BuildMockScene2(bv::BasicNode* parent)
{
    bv::BasicNode* mockTree = new bv::BasicNode();

    // SOLID
    bv::Vec4Interpolator color;

    color.addKey(.0f, glm::vec4(1,0.4,0.5,1));
    color.addKey(3.f, glm::vec4(0.3,0.4,0.5,0));
    color.addKey(1.f, glm::vec4(0.1,0.2,0.8,1));

    color.addKey(2.f, glm::vec4(1,0.4,0.5,0));

    color.setWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);

    bv::SolidColorPlugin* solid1 = new bv::SolidColorPlugin(color);

    mockTree->setPixelShaderPlugin(solid1);

    bv::FloatInterpolator scaleInt;

    scaleInt.setWrapMethod(bv::WrapMethod::pingPong, bv::WrapMethod::pingPong);

    scaleInt.addKey(0.f, -3.5f);
    scaleInt.addKey(-1.30f, 3.5f);

    mockTree->setGeometryShaderPlugin(new bv::ExtrudePlugin(scaleInt));

    mockTree->addGeometryPlugin(new bv::PluginGeometryRect());

    // TRANSFORMATION
    bv::FloatInterpolator x;
    bv::FloatInterpolator y;
    bv::FloatInterpolator z;

    float s = 2.0f;
    float l = 1.4f;
    x.addKey(0.f, 0.5f);x.addKey(0.4f, 0.5f);x.addKey(0.5f, s * 0.6f);x.addKey(0.65f, 0.4f);x.addKey(0.8f, s * 0.9f);x.addKey(0.95f, 0.35f);x.addKey(1.05f, 0.5f);
    y.addKey(0.f, 0.5f);y.addKey(0.4f, 0.5f);y.addKey(0.5f, l * 0.6f);y.addKey(0.65f, 0.4f);y.addKey(0.8f, l * 0.9f);y.addKey(0.95f, 0.35f);y.addKey(1.05f, 0.5f);
    z.addKey(0.f, 0.5f);

    x.setWrapPostMethod(bv::WrapMethod::repeat);
    y.setWrapPostMethod(bv::WrapMethod::repeat);
    
    bv::TransformF* trans = new bv::TransformF();

    bv::FloatInterpolator mx;
    bv::FloatInterpolator my;
    bv::FloatInterpolator mz;

    mx.addKey(0.f, 0.0f);
    my.addKey(0.f, 0.0f);
    mz.addKey(0.f, 0.3f);

    mx.addKey(1.f, 1.0f);
    my.addKey(2.f, -2.0f);
    mz.addKey(3.f, -1.3f);

    trans->addTranslation(mx, my, mz);
    trans->addScale(x,y,z);

    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);

    mockTree->addTransformPlugin(transPlugin);

    if(parent != nullptr)
    {
        parent->addChild(mockTree);
    }

    return mockTree;
}

}


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

    auto veiwMat = m_modelScene->GetCamera()->GetViewMatrix();
    m_mockSceneEng->Update( t, Transform(veiwMat, glm::inverse(veiwMat)));

#ifdef DEBUG_INFO
    RenderMockScene(m_mockScene, m_file);
#endif

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

    if ( totalPassed > 1.0 )
    {
        totalPassed = 0.0;
        std::cout.precision(4);
        std::cout << "FPS: " << 1.0 / frameUpdate << std::endl;
        std::cout << "Model: " << modeldt * 1000.0 << "  Manager: " << managerdt * 1000.0 << "  Engine: " << enginedt * 1000.0 << " Render: " << renderdt * 1000.0 << " Total: " << frameUpdate * 1000.0 << std::endl;
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
        SetConsoleTitleA("Debug Console");
        freopen("CONOUT$", "wb", stdout);
        freopen("CONOUT$", "wb", stderr);
    }

	bv::BasicNode* root = SceneExamples::CreateSceneMock3(nullptr);

   // bv::BasicNode* root = BuildMockScene();
    //BuildMockScene2(root);
    //node = BuildMockScene(root);
    //node = BuildMockScene(node);
    //node = BuildMockScene(node);
    //node = BuildMockScene(node);
    //node = BuildMockScene(node);
    //node = BuildMockScene(node);
    //node = BuildMockScene(node);

    m_modelScene = ModelScene::Create(root, new Camera());
    m_Renderer->SetCamera(m_modelScene->GetCamera());
    m_mockSceneEng = m_modelScene->GetSceneRoot()->buildScene();
    
    m_modelScene->GetCamera()->SetFrame(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

    //AddCameraAnimation();
	AddCameraAnimation2();

    m_modelScene->GetCamera()->SetPerspactive(90.f, float(m_Width) / float(m_Height), 0.1f, 100.f);

    m_Renderer->DisableFaceCulling();

#ifdef DEBUG_INFO
    m_file.open("debug.txt", std::ios_base::out);
#endif
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
    alpha = AddRotKeys( position, 0.f, 2.f * M_PI, 0.05, t );
 
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

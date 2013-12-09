#pragma once

#include "Engine/Events/Events.h"
#include "MockFrameReader.h"

#include <windows.h>


namespace bv
{

namespace model
{
    class BasicNode;
    class ModelScene;
}

class Renderer;
class SceneNode;
class Camera;
class MockFrameReader;
class ProcessManager;

//FIXME: possibly add an interface such as IAppLogic (if necessary)
class BVAppLogic
{
private:

    model::ModelScene *         m_modelScene;
    SceneNode *                 m_mockSceneEng;

    unsigned long               m_startTime;

public:

                    BVAppLogic      ();
                    ~BVAppLogic     ();

    void            Initialize      ();

    //FIXME: this initialization has to be refactored and started in separate process (threaded)
    void            LoadScene       ( void );
    void            InitCamera      ( Renderer * renderer, int w, int h );

    void            SetStartTime    ( unsigned long millis );

    virtual void    OnUpdate        ( unsigned long millis, Renderer * renderer, HWND handle );
    //virtual void  OnChangeState   (<type> state); //FIXME: implement if necessary and any interesting states are added to the main loop

    virtual void    ShutDown        ();

    //Delegates
    void            FrameRendered   ( IEventPtr evt );

private:

    void            RenderScene     ( Renderer * renderer );
    void            RenderNode      ( Renderer * renderer, SceneNode * node );

};

} //bv

//// *********************************
////
//void BlackVisionApp::AddCameraAnimation  ()
//{
//    Vec3Interpolator * position     = new Vec3Interpolator();
//    Vec3Interpolator * direction    = new Vec3Interpolator();
//    Vec3Interpolator * up           = new Vec3Interpolator();
//
//    TestParametersFactory::CameraAnimation_ver1( position, direction, up );
//
//    m_modelScene->SetCamereParameters(  model::ParametersFactory::CreateParameter( "position", *position )
//                                      , model::ParametersFactory::CreateParameter( "direction", *direction )
//                                      , model::ParametersFactory::CreateParameter( "up", *up ) );
//
//    delete position;
//    delete direction;
//    delete up;
//}
//
//// *********************************
////
//void BlackVisionApp::AddCameraAnimation2  () //smietnik lekko oczyszczony ze smieci
//{
//    Vec3Interpolator * position     = new Vec3Interpolator();
//    Vec3Interpolator * direction    = new Vec3Interpolator();
//    Vec3Interpolator * up           = new Vec3Interpolator();
//
//    TestParametersFactory::CameraAnimation_ver2( position, direction, up );
//
//    m_modelScene->SetCamereParameters(  model::ParametersFactory::CreateParameter( "position", *position )
//                                      , model::ParametersFactory::CreateParameter( "direction", *direction )
//                                      , model::ParametersFactory::CreateParameter( "up", *up ) );
//
//    delete position;
//    delete direction;
//    delete up;
//}

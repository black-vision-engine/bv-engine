#pragma once

#include "Engine/Events/Events.h"
#include "BVForwards.h"

#include <windows.h> //FIXME: remove when this code is refactored


namespace bv
{

enum class BVAppState : int
{
    BVS_INITALIZING = 0,
    BVS_RUNNING,
    BVS_INVALID,

    BVS_TOTAL
};

//FIXME: possibly add an interface such as IAppLogic (if necessary)
class BVAppLogic
{
private:

    BVAppState                  m_state;

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

    virtual void    ChangeState     ( BVAppState state );

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

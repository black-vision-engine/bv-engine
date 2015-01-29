#pragma once

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/ModelScene.h"

#include "Engine/Events/Events.h"

#include "FrameStatsService.h"

#define HIDE_PROFILE_STATS


namespace bv
{

class SimpleTimer;
class RenderLogic;

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

    BVAppState                      m_state;

    FrameStatsCalculator            m_statsCalculator;

    model::TimelineManager *        m_timelineManager;
    const model::PluginsManager *   m_pluginsManager;
    model::OffsetTimeEvaluatorPtr   m_globalTimeline;

    model::ModelScenePtr            m_modelScene;
    SceneNode *                     m_engineScene;

    RenderLogic *                   m_renderLogic;

    unsigned long                   m_startTime;

public:

                    BVAppLogic      ();
                    ~BVAppLogic     ();

    void            Initialize      ();

    //FIXME: this initialization has to be refactored and started in separate process (threaded)
    void            LoadScene       ( void );
    void            InitCamera      ( Renderer * renderer, unsigned int w, unsigned int h );

    void            SetStartTime    ( unsigned long millis );

    virtual void    OnUpdate        ( unsigned int millis, const SimpleTimer & timer, Renderer * renderer, HWND handle );
    virtual void    OnKey           ( unsigned char c );

    virtual void    ChangeState     ( BVAppState state );

    virtual void    ShutDown        ();

    void            PostFrameLogic  ( const SimpleTimer & timer, unsigned int millis );

    const FrameStatsCalculator &     FrameStats () const;

    void            ResetScene      ();
    void            ReloadScene     ();

private:

    void            OnUpdateParam   ( IEventPtr evt );
	void            OnNodeAppearing ( IEventPtr evt );
	void            OnNodeLeaving   ( IEventPtr evt );
	void            OnNoMoreNodes   ( IEventPtr evt );

public:

    //Convenience API - generalized model accessors
    model::TimelineManager *        GetTimelineManager  ();
    model::ModelScenePtr            GetModelScene       ();
    const model::PluginsManager *   GetPluginsManager   () const;
};

} //bv

#define FRAME_STATS_FRAME()                         FrameStatsFrameScope COMBINE(stats_new_rame_,__LINE__)( &m_statsCalculator ) 
#define FRAME_STATS_SECTION( name )                 FrameStatsSectionScope COMBINE(stats_new_section_,__LINE__)( &m_statsCalculator, name ) 

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
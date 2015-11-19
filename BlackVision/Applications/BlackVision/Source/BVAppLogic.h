#pragma once

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/BVScene.h"
#include "System/Path.h"

#include "Engine/Events/Events.h"

#include "TestAI/TestKeyboardHandler.h"

#include "FrameStatsService.h"

#include "EndUserAPI/RemoteController.h"

//#define HIDE_PROFILE_STATS

//pablito
#include "Solution.h"
#include "VideoCardManager.h"
#include "structure/AssetManager.h"

//hackvideoinput
#include "VideoInput/DefaultVideoInputResourceDescr.h"
#include "hack_videoinput/TestVideoInput.h"



namespace bv
{

class SimpleTimer;
class RenderLogic;
class Renderer;
class RemoteEventsHandlers;


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

    //hackvideoinput
    TestVideoInput*                  VideoInput;

    BVAppState                      m_state;

    FrameStatsCalculator            m_statsCalculator;

    model::TimelineManagerPtr       m_timelineManager;
    const model::PluginsManager *   m_pluginsManager;
    model::OffsetTimeEvaluatorPtr   m_globalTimeline;

    BVScenePtr                      m_bvScene;

    Renderer *                      m_renderer;
    RenderLogic *                   m_renderLogic;
    TestKeyboardHandler *           m_kbdHandler;

    RemoteEventsHandlers*           m_remoteHandlers;
    RemoteController*               m_remoteController;

    unsigned long                   m_startTime;

	//pablito
	Solution						m_solution;
	bv::videocards::VideoCardManager* m_videoCardManager;
	std::string                     m_grabFramePath;
    
    void            RefreshVideoInputScene  ();

public:

                    BVAppLogic      ( Renderer * renderer );
                    ~BVAppLogic     ();

    void            Initialize      ();

    //FIXME: this initialization has to be refactored and started in separate process (threaded)
    void            LoadScene       ( void );
    void            InitCamera      ( unsigned int w, unsigned int h );

    void            SetStartTime    ( unsigned long millis );

	//pablito:
	void			SetVideoCardManager(bv::videocards::VideoCardManager* videoCardManager);
	FrameStatsCalculator* GetStatsCalculator(){return &m_statsCalculator;};

    virtual void    OnUpdate        ( unsigned int millis, Renderer * renderer );
    virtual void    OnKey           ( unsigned char c );
    
    virtual void    ChangeState     ( BVAppState state );

    virtual void    ShutDown        ();

    void            PostFrameLogic  ( const SimpleTimer & timer, unsigned int millis );

    const FrameStatsCalculator &     FrameStats () const;

    void            ResetScene      ();
    void            ReloadScene     ();

	void            GrabCurrentFrame(  const std::string & path );

public:

    //Convenience API - generalized model accessors
    model::TimelineManagerPtr       GetTimelineManager  ();
    model::OffsetTimeEvaluatorPtr   GetGlobalTimeline   ();
    BVScenePtr                      GetBVScene          ();
    const model::PluginsManager *   GetPluginsManager   () const;


    void            LoadScenes      ( const PathVec & pathVec );

private:

    void                            InitializeKbdHandler();

    void                            InitializeScenesTimelines();
    std::string                     GetEnvScene();

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

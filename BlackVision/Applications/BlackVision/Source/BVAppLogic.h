#pragma once

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/BVProject.h"
#include "System/Path.h"

#include "Engine/Events/Events.h"
#include "RenderMode.h"

#include "Tools/SimpleTimer.h"

#include "TestAI/TestKeyboardHandler.h"

#include "Statistics/FrameStatsService.h"

#include "EndUserAPI/RemoteController.h"

//#define HIDE_PROFILE_STATS

//hackvideoinput
#include "Engine/Models/Plugins/Simple/VideoInput/DefaultVideoInputResourceDescr.h"
#include "hack_videoinput/TestVideoInput.h"



namespace bv {

class SimpleTimer;
class RenderLogic;
class Renderer;
class RemoteEventsHandlers;

namespace audio {
    class AudioRenderer;
}

class RenderLogic;

namespace videocards {
    class VideoCardManager;
}

enum class BVAppState : int
{
    BVS_INITALIZING = 0,
    BVS_RUNNING,
    BVS_CLOSING,
    BVS_INVALID,

    BVS_TOTAL
};

//FIXME: possibly add an interface such as IAppLogic (if necessary)
class BVAppLogic
{
private:

    SimpleTimer                     m_timer;

    BVAppState                      m_state;

    FrameStatsCalculator            m_statsCalculator;

    const model::PluginsManager *   m_pluginsManager;

    BVProjectPtr                    m_bvProject;

    Renderer *                      m_renderer;

	Float32							m_gain;
    audio::AudioRenderer *          m_audioRenderer;
    
    // FIXME: nrl - render logic replacement
    RenderLogic *                   m_renderLogic;
    //RenderLogic *                   m_renderLogic;
    //FrameRenderLogic *              m_renderLogic;
    TestKeyboardHandler *           m_kbdHandler;

    RemoteEventsHandlers *          m_remoteHandlers;
    RemoteController *              m_remoteController;

    RenderMode                      m_renderMode;

    videocards::VideoCardManager *  m_videoCardManager;

    unsigned int                    m_frameStartTime;

    
    void            RefreshVideoInputScene  ();

public:

                    BVAppLogic      ( Renderer * renderer, audio::AudioRenderer * audioRenderer );
                    ~BVAppLogic     ();

    void            Initialize      ();
    void            Deinitialize    ();

    //FIXME: this initialization has to be refactored and started in separate process (threaded)
    void            LoadScene       ( void );
    void            UnloadScenes    ();

    unsigned int    StartTime       ();
    unsigned int    GetTime         () const;

    virtual void    OnUpdate        ( Renderer * renderer, audio::AudioRenderer * audioRenderer );
    virtual void    OnKey           ( unsigned char c );
    virtual void    OnMouse         ( MouseAction action, int posX, int posY );

    virtual void    ShutDown        ();

    void            PostFrameLogic  ();
    void            HandleFrame     ( TimeType time, Renderer * renderer, audio::AudioRenderer * audioRenderer );

	FrameStatsCalculator *          GetStatsCalculator  ();
    const FrameStatsCalculator &    FrameStats          () const;

    void            ResetScene      ();
    void            ReloadScene     ();

	void            GrabCurrentFrame(  const std::string & path );

    virtual void            ChangeState     ( BVAppState state );
    virtual BVAppState      GetState        ();

	void			SetGain			( Float32 gain );

public:

    //Convenience API - generalized model accessors
    BVProjectPtr                    GetBVProject        () const;
    const model::PluginsManager *   GetPluginsManager   () const;
    RenderLogic *             GetRenderLogic      () const;
    RenderMode &                    GetRenderMode       ();

    void                            LoadScenes          ( const PathVec & pathVec );

private:

    void                            InitializeKbdHandler();
    void                            InitializeRemoteCommunication();
    void                            InitializeCommandsDebugLayer();

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

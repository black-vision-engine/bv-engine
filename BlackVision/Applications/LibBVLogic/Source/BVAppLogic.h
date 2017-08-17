#pragma once

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Editors/BVProject.h"
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
protected:

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
    virtual void    LoadScene       ( void );
    void            UnloadScenes    ();
    void            ResetScene      ();
    void            ReloadScene     ();

    virtual void    ShutDown        ();


    unsigned int    StartTime       ();
    unsigned int    GetTime         () const;

    virtual void    OnUpdate        ( Renderer * renderer, audio::AudioRenderer * audioRenderer );
    virtual void    OnKey           ( unsigned char c );
    virtual void    OnMouse         ( MouseAction action, int posX, int posY );


    virtual void            ChangeState     ( BVAppState state );
    virtual BVAppState      GetState        ();

	void			SetGain			( Float32 gain );
    virtual void    PostFrameLogic  ();

public:

    //Convenience API - generalized model accessors
    BVProjectPtr                    GetBVProject        () const;
    const model::PluginsManager *   GetPluginsManager   () const;
    RenderLogic *                   GetRenderLogic      () const;
    RenderMode &                    GetRenderMode       ();
    Renderer *                      GetRenderer         () const { return m_renderer; }

    void                            LoadScenes          ( const PathVec & pathVec );

    FrameStatsCalculator *          GetStatsCalculator  ();
    const FrameStatsCalculator &    FrameStats          () const;

protected:

    TimeType                        ComputeFrameTime    ();
    void                            EventsPhase         ();
    void                            ModelUpdatePhase    ( TimeType time );
    void                            RenderPhase         ( TimeType time, Renderer * renderer, audio::AudioRenderer * audioRenderer );
    void                            CheckDropFrame      ( TimeType time );

private:

    void                            InitializeKbdHandler();
    void                            InitializeRemoteCommunication();
    void                            InitializeCommandsDebugLayer();
    void                            InitializeDefaultAssets();

    std::string                     GetEnvScene();

};

} //bv

#define FRAME_STATS_FRAME()                         FrameStatsFrameScope COMBINE(stats_new_rame_,__LINE__)( &m_statsCalculator ) 
#define FRAME_STATS_SECTION( name )                 FrameStatsSectionScope COMBINE(stats_new_section_,__LINE__)( &m_statsCalculator, name ) 



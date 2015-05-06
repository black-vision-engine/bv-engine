#pragma once

#include "win_sock.h"

#include "CoreDEF.h"


#define MAX_PROFILER_SAMPLES 120
#define MAX_PROFILER_FRAMES 10


namespace bv
{

enum class AutoProfileType : int
{
    APT_FUNCTION = 0,
    APT_SECTION
};

enum class AutoProfileState : int
{
    APS_START = 0,
    APS_END
};

struct ProfilerSample
{
    const char *        name;

    LARGE_INTEGER       duration;
    double              durationSecs;

    AutoProfileType     type;
    unsigned int        depth;

};

struct ProfilerLiveSample
{
    const char *        name;
   
    LARGE_INTEGER       timestamp;
    AutoProfileType     type;
    AutoProfileState    state;

};

class AutoProfile
{
private:

    static ProfilerLiveSample   m_liveSamples[ 2 * MAX_PROFILER_SAMPLES * MAX_PROFILER_FRAMES ];
    static ProfilerSample       m_samples[ MAX_PROFILER_SAMPLES * MAX_PROFILER_FRAMES ];

    static unsigned int         m_curSample;
    static unsigned int         m_curFrame;
    static unsigned int         m_activeFrame;
    static unsigned int         m_displayStatsWaitMillis;

    const char *                m_name;

public:

    inline                  AutoProfile             ( const char * name, AutoProfileType type );
    inline                  ~AutoProfile            ();

private:

    static  void            StartFrame              ();
    static  void            EndFrame                ();

public:

    static void             SetStatsDisplayWaitMs   ( unsigned int millis );
    static unsigned int     GetStatsDisplayWaitMs   ();

    static unsigned int     NumSamples              ();
    static unsigned int     NumFrames               ();
    static unsigned int     ActiveFrame             ();

    static LARGE_INTEGER    QueryCounterFrequency   ();

    static const ProfilerSample *   OneFrameSamples ( unsigned int frame );
    static const ProfilerSample *   AveragedSamples ();

    friend class AutoFrameProfile;
};

class AutoFrameProfile
{
public:

    typedef void (*PtrDisplayCallback)( const char * );

private:

    static bool                 m_showStats;
    static PtrDisplayCallback   m_displayCallback;

public:

    inline  AutoFrameProfile    ();
    inline  ~AutoFrameProfile   ();

    static void    SetDisplayStats          ();
    static void    RegisterDisplayCallback  ( PtrDisplayCallback callback );

};

} //bv

#define HPROFILER_NEW_FRAME()                       AutoFrameProfile COMBINE(frame_sample_,__LINE__)

#define HPROFILER_FUNCTION( name )                  AutoProfile COMBINE(function_sample_,__LINE__) ( name, AutoProfileType::APT_FUNCTION )
#define HPROFILER_SECTION( name )                   AutoProfile COMBINE(section_sample_,__LINE__) ( name, AutoProfileType::APT_SECTION )

#define HPROFILER_GET_ONE_FRAME_SAMPLES( frame )    AutoProfile::OneFrameSamples( frame )
#define HPROFILER_GET_AVERAGED_SAMPLES()            AutoProfile::AveragedSamples()

#define HPROFILER_GET_NUM_SAMPLES()                 AutoProfile::NumSamples()
#define HPROFILER_GET_NUM_FRAMES()                  AutoProfile::NumFrames()
#define HPROFILER_GET_ACTIVE_FRAME()                AutoProfile::ActiveFrame()

#define HPROFILER_SET_DISPLAY_WAIT_MILLIS( millis ) AutoProfile::SetStatsDisplayWaitMs( millis )
#define HPROFILER_REGISTER_DISPLAY_CALLBACK( cb )   AutoFrameProfile::RegisterDisplayCallback( cb )
#define HPROFILER_SET_FORCED_DISPLAY()              AutoFrameProfile::SetDisplayStats()

#include "HerarchicalProfiler.inl"

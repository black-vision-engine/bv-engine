#pragma once

#include <windows.h>

#define MAX_PROFILER_SAMPLES 6
#define MAX_PROFILER_FRAMES 5


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

public:

    inline                  AutoProfile             ( const char * name, AutoProfileType type );
    inline                  ~AutoProfile            ();

private:

    static  void            StartFrame              ();
    static  void            EndFrame                ();

public:

    static unsigned int     NumSamples              ();
    static unsigned int     NumFrames               ();

    static LARGE_INTEGER    QueryCounterFrequency   ();

    static const ProfilerSample *   OneFrameSamples ( unsigned int frame );
    static const ProfilerSample *   AveragedSamples ();

    friend class AutoFrameProfile;
};

class AutoFrameProfile
{
public:

    inline  AutoFrameProfile    ();
    inline  ~AutoFrameProfile   ();

};

} //bv


#define HPROFILER_NEW_FRAME()                       AutoFrameProfile()

#define HPROFILER_FUNCTION( name )                  AutoProfile( name, AutoProfileType::APT_FUNCTION )
#define HPROFILER_SECTION( name )                   AutoProfile( name, AutoProfileType::APT_SECTION )

#define HPROFILER_GET_ONE_FRAME_SAMPLES( frame )    AutoProfile::OneFrameSamples( frame )
#define HPROFILER_GET_AVERAGED_SAMPLES()            AutoProfile::AveragedSamples()

#define HPROFILER_GET_NUM_SAMPLES()                 AutoProfile::NumSamples()
#define HPROFILER_GET_NUM_FRAMES()                  AutoProfile::NumFrames()

#include "HerarchicalProfiler.inl"

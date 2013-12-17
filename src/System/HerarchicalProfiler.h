#pragma once

#include <windows.h>

#define MAX_PROFILER_SAMPLES 200
#define MAX_PROFILER_FRAMES 200


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

    static unsigned int m_curSample;
    static unsigned int m_curFrame;

public:

    inline              AutoProfile                 ( const char * name, AutoProfileType type );
    inline              ~AutoProfile                ();

    static  void        StartFrame                  ();
    static  void        EndFrame                    ();

    static unsigned int NumSamples                  ();
    static unsigned int NumFrames                   ();

    static const ProfilerSample *   OneFrameSamples ( unsigned int frame );
    static const ProfilerSample *   AveragedSamples ();
};

} //bv

#define PROFILE_FRAME_START() AutoProfile::StartFrame()
#define PROFILE_FRAME_END() AutoProfile::EndFrame()

#define PROFILE_FUNCTION( name ) AutoProfile( name, AutoProfileType::APT_FUNCTION )
#define PROFILE_SECTION( name ) AutoProfile( name, AutoProfileType::APT_SECTION )

#define PROFILE_GET_ONE_FRAME_SAMPLES( frame ) AutoProfile::OneFrameSamples( frame )
#define PROFILE_GET_AVERAGWED_SAMPLES() AutoProfile::AveragedSamples()

#include "HerarchicalProfiler.inl"

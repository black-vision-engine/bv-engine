#pragma once

#include <windows.h>

#define MAX_PROFILER_SAMPLES 200
#define MAX_PROFILER_FRAMES 50


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
public:

    const char *        name;

    LARGE_INTEGER       duration;

    AutoProfileType     type;

    unsigned int        depth;

public:

    double  Duration    () const;

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

#include "HerarchicalProfiler.inl"

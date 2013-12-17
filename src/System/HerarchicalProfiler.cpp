#include "HerarchicalProfiler.h"

#include <cassert>


namespace bv
{

ProfilerLiveSample  AutoProfile::m_liveSamples[ 2 * MAX_PROFILER_SAMPLES * MAX_PROFILER_FRAMES ];
ProfilerSample      AutoProfile::m_samples[ MAX_PROFILER_SAMPLES * MAX_PROFILER_FRAMES ];

unsigned int    AutoProfile::m_curSample = 0;
unsigned int    AutoProfile::m_curFrame = 0;


// *******************************
//
void            AutoProfile::StartFrame         ()
{
    m_curSample = 0;
}

// *******************************
//
void            AutoProfile::EndFrame           ()
{
    assert( NumSamples() <= MAX_PROFILER_SAMPLES );
    m_curFrame = ( m_curFrame + 1 ) % MAX_PROFILER_FRAMES;

    //TODO: anything useful
}

// *******************************
//
unsigned int    AutoProfile::NumSamples         ()
{
    assert( m_curSample % 2 == 0 );

    return m_curSample / 2;
}

// *******************************
//
LARGE_INTEGER    AutoProfile::QueryCounterFrequency   ()
{
    LARGE_INTEGER pf;

    QueryPerformanceFrequency( &pf );

    return pf;
}

// *******************************
//
unsigned int    AutoProfile::NumFrames          ()
{
    assert( m_curFrame <= MAX_PROFILER_FRAMES );

    return m_curFrame;
}

// *******************************
//
const ProfilerSample *   AutoProfile::OneFrameSamples ( unsigned int frame )
{
    LARGE_INTEGER freq = QueryCounterFrequency();
    double freqd = (double) freq.QuadPart;

    ProfilerSample * stack[ MAX_PROFILER_SAMPLES ];

    unsigned int numSamples = NumSamples();
    unsigned int numLiveSamples = 2 * numSamples;

    assert( numSamples <= MAX_PROFILER_SAMPLES );

    ProfilerLiveSample * liveSamples = &m_liveSamples[ frame * 2 * MAX_PROFILER_SAMPLES ];
    ProfilerSample * samples = &m_samples[ frame * MAX_PROFILER_SAMPLES ];

    unsigned int depth = 0;
    for( unsigned int i = 0, k = 0; i < numLiveSamples; ++i )
    {
        ProfilerLiveSample & liveSample = liveSamples[ i ];
        ProfilerSample & sample = samples[ k ];

        if( liveSample.state == AutoProfileState::APS_START )
        {
            stack[ depth ] = &sample;

            sample.depth = depth;
            sample.name = liveSample.name;
            sample.type = liveSample.type;
            sample.duration = liveSample.timestamp;
            
            ++depth;
            ++k;
        }
        else //APS_END
        {
            sample.duration.QuadPart = liveSample.timestamp.QuadPart - stack[ depth ]->duration.QuadPart;
            sample.durationSecs = (double) sample.duration.QuadPart / freqd;
            --depth;
        }
    }

    return samples;
}

// *******************************
//
const ProfilerSample *   AutoProfile::AveragedSamples ()
{
    LARGE_INTEGER accum[ MAX_PROFILER_SAMPLES ];
    double accumSecs[ MAX_PROFILER_SAMPLES ];

    memset( accum, 0, MAX_PROFILER_SAMPLES * sizeof( LARGE_INTEGER ) );
    memset( accumSecs, 0, MAX_PROFILER_SAMPLES * sizeof( double ) );

    unsigned int numFrames = NumFrames();
    for( unsigned int i = 0; i < numFrames; ++i )
    {
        auto dummy = OneFrameSamples( i );
    }

    unsigned int numSamples = NumSamples();
    for( unsigned int i = 0; i < numSamples; ++i )
    {
        for( unsigned int fr = 0; fr < numFrames; ++fr )
        {
            accum[ i ].QuadPart += m_samples[ fr * MAX_PROFILER_SAMPLES + i ].duration.QuadPart;
            accumSecs[ i ] += m_samples[ fr * MAX_PROFILER_SAMPLES + i ].durationSecs;
        }
    }

    for( unsigned int i = 0; i < numSamples; ++i )
    {
        m_samples[ i ].duration.QuadPart = accum[ i ].QuadPart / numFrames;
        m_samples[ i ].durationSecs = accumSecs[ i ] / (double) numFrames;
    }

    return m_samples;
}

} // bv

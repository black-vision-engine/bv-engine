#include "HerarchicalProfiler.h"

#include <cassert>


namespace bv
{

CPUThreadSamples	AutoProfile::m_threads[ MAX_PROFILER_THREADS ];
ProfilerSample      AutoProfile::m_samples[ MAX_PROFILER_SAMPLES * MAX_PROFILER_FRAMES ];

unsigned int    AutoProfile::m_displayStatsWaitMillis = 500;
bool            AutoFrameProfile::m_showStats = false;

AutoFrameProfile::PtrDisplayCallback  AutoFrameProfile::m_displayCallback = nullptr;


// *******************************
//
void            AutoProfile::StartFrame         ( unsigned int threadID )
{
	assert( threadID < MAX_PROFILER_THREADS );

	m_threads[ threadID ].m_curSample = 0;
    m_threads[ threadID ].m_activeFrame = m_threads[ threadID ].m_curFrame;
}

// *******************************
//
void            AutoProfile::EndFrame           ( unsigned int threadID )
{
    assert( NumSamples() <= MAX_PROFILER_SAMPLES );
	
	m_threads[ threadID ].m_numFrameSamples[ m_threads[ threadID ].m_curFrame ] = m_threads[ threadID ].m_curSample;		//Remember number of samples in frame.
    m_threads[ threadID ].m_curFrame = ( m_threads[ threadID ].m_curFrame + 1 ) % MAX_PROFILER_FRAMES;
	m_threads[ threadID ].m_framesToSend++;
    //TODO: anything useful
}

// *******************************
//
void            AutoProfile::SetStatsDisplayWaitMs   ( unsigned int millis )
{
    m_displayStatsWaitMillis = millis;
}

// *******************************
//
unsigned int    AutoProfile::GetStatsDisplayWaitMs   ()
{
    return m_displayStatsWaitMillis;
}

// *******************************
//
unsigned int    AutoProfile::NumSamples         ( unsigned int threadID )
{
	assert( threadID < MAX_PROFILER_THREADS );
	return m_threads[ threadID ].m_curSample/* - 1*/;
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
unsigned int    AutoProfile::NumFrames          ( unsigned int threadID )
{
	assert( threadID < MAX_PROFILER_THREADS );
    assert( m_threads[ threadID ].m_curFrame <= MAX_PROFILER_FRAMES );

    return m_threads[ threadID ].m_activeFrame + 1;
}

// *******************************
//
unsigned int     AutoProfile::ActiveFrame             ( unsigned int threadID )
{
	assert( threadID < MAX_PROFILER_THREADS );
    return m_threads[ threadID ].m_activeFrame;
}

// *******************************
//
CPUThreadSamples*	AutoProfile::GetCPUThreadSamples		( unsigned int thread )
{
	return &m_threads[ thread ];
}

// *******************************
// This function survived only for compatibility.
const ProfilerSample *   AutoProfile::OneFrameSamples ( unsigned int frame )
{
    LARGE_INTEGER freq = QueryCounterFrequency();
    double freqd = (double) freq.QuadPart;

    unsigned int numLiveSamples = NumSamples( 0 );		// Num samples for first thread

    assert( numLiveSamples <= MAX_PROFILER_SAMPLES );

	ProfilerLiveSample * liveSamples = &m_threads[ 0 ].m_liveSamples[ frame * MAX_PROFILER_SAMPLES ];
    ProfilerSample * samples = &m_samples[ frame * MAX_PROFILER_SAMPLES ];

    for( unsigned int i = 0, k = 0; i < numLiveSamples; ++i )
    {
        ProfilerLiveSample & liveSample = liveSamples[ i ];
        ProfilerSample & sample = samples[ k ];

		sample.depth = liveSample.depth;
		sample.name = liveSample.name;
		sample.type = liveSample.type;

		sample.duration.QuadPart = liveSample.timeEnd.QuadPart - liveSample.timeStart.QuadPart;
		sample.durationSecs = (double)sample.duration.QuadPart / freqd;

		++k;
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
        { dummy; } //FIXME: suppress warning
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

// *******************************
//
void    AutoFrameProfile::SetDisplayStats           ()
{
    m_showStats = true;
}

// *******************************
//
void    AutoFrameProfile::RegisterDisplayCallback   ( PtrDisplayCallback callback )
{
    m_displayCallback = callback;
}

} // bv

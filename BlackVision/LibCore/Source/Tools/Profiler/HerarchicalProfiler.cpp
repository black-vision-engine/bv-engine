#include "stdafxCore.h"

#include <winsock2.h>
#include <windows.h>


#include "HerarchicalProfiler.h"

#include <cassert>


namespace bv
{

CPUThreadSamples	AutoProfile::m_threads[ MAX_PROFILER_THREADS ];

unsigned int    AutoProfile::m_displayStatsWaitMillis = 500;
bool            AutoFrameProfile::m_showStats = false;
ProfilerMode	AutoFrameProfile::m_mode = ProfilerMode::PM_EVERY_FRAME;
unsigned int	AutoFrameProfile::m_framesToShow = 100;

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
    assert( NumSamples( threadID ) <= MAX_PROFILER_SAMPLES );
	
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
UInt64      AutoProfile::QueryCounterFrequency   ()
{
    LARGE_INTEGER pf;

    QueryPerformanceFrequency( &pf );

    return pf.QuadPart;
}

// *******************************
//
UInt64          AutoProfile::QueryCounter()
{
    LARGE_INTEGER time;
    QueryPerformanceCounter( &time );

    return time.QuadPart;
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

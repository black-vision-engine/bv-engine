#include <assert.h>

#include "System/Time.h"

namespace bv
{

// *********************************
//
inline                     AutoProfile::AutoProfile     ( const char * name, AutoProfileType type, unsigned int threadID )
{
	m_threadID = threadID;
	ProfilerLiveSample & sample = m_threads[ m_threadID ].m_liveSamples[ m_threads[ m_threadID ].m_curFrame * MAX_PROFILER_SAMPLES + m_threads[ m_threadID ].m_curSample++ ];

    sample.name = name;
    sample.type = type;
	sample.depth = m_threads[ m_threadID ].m_curDepth++;
	sample.timeEnd = INVALID_TIME;
    sample.timeStart = QueryCounter();
}

// *********************************
//
inline                     AutoProfile::~AutoProfile    ()
{
	int sampleCounter = m_threads[ m_threadID ].m_curFrame * MAX_PROFILER_SAMPLES + m_threads[ m_threadID ].m_curSample - 1;
    
	while( m_threads[ m_threadID ].m_liveSamples[ sampleCounter ].timeEnd != INVALID_TIME )
		--sampleCounter;

	ProfilerLiveSample & sample = m_threads[ m_threadID ].m_liveSamples[ sampleCounter ];
    sample.timeEnd = QueryCounter();

	--m_threads[ m_threadID ].m_curDepth;
}

// *********************************
//
inline  AutoFrameProfile::AutoFrameProfile    ( unsigned int threadID )
{
	assert( threadID < MAX_PROFILER_THREADS );
	m_threadID = threadID;
    AutoProfile::StartFrame( threadID );
}

// *********************************
//
inline  AutoFrameProfile::~AutoFrameProfile   ()
{
    static auto startMillis = Time::Now();

	AutoProfile::EndFrame( m_threadID );

    auto timestamp = Time::Now();

	if( m_mode == ProfilerMode::PM_EVERY_FRAME )
		m_showStats = true;			// Force display every frame

    if( m_showStats )
    {
		m_displayCallback( "FRAME TIME - FORCED DISPLAY", m_threadID );
        startMillis = timestamp;
        m_showStats = false;
		AutoProfile::m_threads[ m_threadID ].m_framesToSend = 0;
    }
	else if( m_mode == ProfilerMode::PM_EVERY_N_FRAMES_AND_FORCE_DISPLAY && AutoProfile::m_threads[ m_threadID ].m_framesToSend >= MAX_PROFILER_FRAMES )
	{
		m_displayCallback( "FRAME TIME - REGULAR SAMPLE", m_threadID );
        startMillis = timestamp;
		AutoProfile::m_threads[ m_threadID ].m_framesToSend = 0;
	}
	else if( m_mode == ProfilerMode::PM_WAIT_TIME_AND_FORCE_DISPLAY && timestamp - startMillis > AutoProfile::GetStatsDisplayWaitMs() )
    {
		m_displayCallback( "FRAME TIME - REGULAR SAMPLE", m_threadID );
        startMillis = timestamp;
		AutoProfile::m_threads[ m_threadID ].m_framesToSend = 0;
    }
}

} //bv

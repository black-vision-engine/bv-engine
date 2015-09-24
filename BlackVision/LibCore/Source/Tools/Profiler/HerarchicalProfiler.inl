#include <assert.h>

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
	sample.timeEnd.QuadPart = INVALID_TIME;
    QueryPerformanceCounter( &sample.timeStart );
}

// *********************************
//
inline                     AutoProfile::~AutoProfile    ()
{
	int sampleCounter = m_threads[ m_threadID ].m_curFrame * MAX_PROFILER_SAMPLES + m_threads[ m_threadID ].m_curSample - 1;
    
	while( m_threads[ m_threadID ].m_liveSamples[ sampleCounter ].timeEnd.QuadPart != INVALID_TIME )
		--sampleCounter;

	ProfilerLiveSample & sample = m_threads[ m_threadID ].m_liveSamples[ sampleCounter ];
    QueryPerformanceCounter( &sample.timeEnd );

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
    static unsigned long startMillis = timeGetTime();

	AutoProfile::EndFrame( m_threadID );

    unsigned int timestamp = timeGetTime();

	m_showStats = true;		/// test

    if( m_showStats )
    {
        m_displayCallback( "FRAME TIME - FORCED DISPLAY" );
        startMillis = timestamp;
        m_showStats = false;
		AutoProfile::m_threads[ m_threadID ].m_framesToSend = 0;
    }
	//else if( AutoProfile::m_threads[ m_threadID ].m_framesToSend >= MAX_PROFILER_FRAMES )
	//{
	//	m_displayCallback( "FRAME TIME - REGULAR SAMPLE" );
 //       startMillis = timestamp;
	//	AutoProfile::m_threads[ m_threadID ].m_framesToSend = 0;
	//}
    else if( timestamp - startMillis > AutoProfile::GetStatsDisplayWaitMs() )
    {
        m_displayCallback( "FRAME TIME - REGULAR SAMPLE" );
        startMillis = timestamp;
		AutoProfile::m_threads[ m_threadID ].m_framesToSend = 0;
    }
}

} //bv

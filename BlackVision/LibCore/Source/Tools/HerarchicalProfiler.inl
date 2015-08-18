#include <limits>


#undef min
const int64_t INVALID_TIME = std::numeric_limits<int64_t>::min();

namespace bv
{

// *********************************
//
inline                     AutoProfile::AutoProfile     ( const char * name, AutoProfileType type )
{
    ProfilerLiveSample & sample = m_liveSamples[ m_curFrame * MAX_PROFILER_SAMPLES + m_curSample++ ];

    m_name = name;

    sample.name = name;
    sample.type = type;
	sample.depth = m_depth++;
	sample.timeEnd.QuadPart = INVALID_TIME;
    QueryPerformanceCounter( &sample.timeStart );
}

// *********************************
//
inline                     AutoProfile::~AutoProfile    ()
{
	int sampleCounter = m_curFrame * MAX_PROFILER_SAMPLES + m_curSample - 1;
    
	while( m_liveSamples[ sampleCounter ].timeEnd.QuadPart != INVALID_TIME )
		--sampleCounter;

	ProfilerLiveSample & sample = m_liveSamples[ sampleCounter ];
    sample.name = m_name;
    QueryPerformanceCounter( &sample.timeEnd );

	--m_depth;
}

// *********************************
//
inline  AutoFrameProfile::AutoFrameProfile    ()
{
    AutoProfile::StartFrame();
}

// *********************************
//
inline  AutoFrameProfile::~AutoFrameProfile   ()
{
    static unsigned long startMillis = timeGetTime();

    AutoProfile::EndFrame();

    unsigned int timestamp = timeGetTime();

    if( m_showStats )
    {
        m_displayCallback( "FRAME TIME - FORCED DISPLAY" );
        startMillis = timestamp;
        m_showStats = false;   
    }
    else if( timestamp - startMillis > AutoProfile::GetStatsDisplayWaitMs() )
    {
        m_displayCallback( "FRAME TIME - REGULAR SAMPLE" );
        startMillis = timestamp;
    }
}

} //bv

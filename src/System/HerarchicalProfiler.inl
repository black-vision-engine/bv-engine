namespace bv
{

// *********************************
//
inline                     AutoProfile::AutoProfile     ( const char * name, AutoProfileType type )
{
    ProfilerLiveSample & sample = m_liveSamples[ 2 * m_curFrame * MAX_PROFILER_SAMPLES + m_curSample++ ];

    m_name = name;

    sample.name = name;
    sample.type = type;
    sample.state = AutoProfileState::APS_START;
    QueryPerformanceCounter( &sample.timestamp );
}

// *********************************
//
inline                     AutoProfile::~AutoProfile    ()
{
    ProfilerLiveSample & sample = m_liveSamples[ 2 * m_curFrame * MAX_PROFILER_SAMPLES + m_curSample++ ];
    
    sample.name = m_name;
    sample.state = AutoProfileState::APS_END;
    QueryPerformanceCounter( &sample.timestamp );
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

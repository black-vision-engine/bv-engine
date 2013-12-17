namespace bv
{

// *********************************
//
inline                     AutoProfile::AutoProfile     ( const char * name, AutoProfileType type )
{
    ProfilerLiveSample & sample = m_liveSamples[ m_curSample++ ];

    sample.name = name;
    sample.type = type;
    sample.state = AutoProfileState::APS_START;
    QueryPerformanceCounter( &sample.timestamp );
}

// *********************************
//
inline                     AutoProfile::~AutoProfile    ()
{
    ProfilerLiveSample & sample = m_liveSamples[ m_curSample++ ];
    
    sample.state = AutoProfileState::APS_END;
    QueryPerformanceCounter( &sample.timestamp );
}

} //bv

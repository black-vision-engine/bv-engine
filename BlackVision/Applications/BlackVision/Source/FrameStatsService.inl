#include <cassert>


namespace bv
{

const unsigned int INVALID_FRAME = 0xFFFFFFFF;

// *********************************
//
inline void MovingAverageData::AddNextSample        ( const FrameStatsSample & sample, bool * minFlag, bool * maxFlag )
{
    {
        assert( minFlag );
        assert( maxFlag );

        const FrameStatsSample & last = samples.front();

        accumDuration += sample.duration - last.duration;
        accumDurationSquares += sample.duration * sample.duration - last.duration * last.duration;

        if ( sample.duration < minDuration )
        {
            minDuration = sample.duration;
            minDurationFrame = sample.frame;
            *minFlag = true;
        }

        if ( sample.duration > maxDuration )
        {
            maxDuration = sample.duration;
            maxDurationFrame = sample.frame;
            *maxFlag = true;
        }
    }

    samples.pop_front();
    samples.push_back( sample );
}


// *********************************
// Assumes there's at least one valid sample.
inline unsigned int MovingAverageData::CountUselessSamples() const
{
	// Skip all useless samples.
	unsigned int validSample = 0;
	while( samples[ validSample ].frame == INVALID_FRAME )
		++validSample;

	return validSample;
}

// *********************************
//
inline void    FrameStatsCalculator::StartSection   ( const char * name )
{
    FrameStatsSample & sample = m_stateBuffer[ name ];

    m_minFlags[ name ] = false;
    m_maxFlags[ name ] = false;

    sample.frame = m_frame;
    m_timer.Timestamp( &sample.startTime );
    sample.duration = -1.0;
}

// *********************************
//
inline void    FrameStatsCalculator::EndSection     ( const char * name )
{
    FrameStatsSample & sample = m_stateBuffer[ name ];
    assert( sample.duration == -1.0 );
    sample.duration = m_timer.CurElapsed( sample.startTime );

    //MovingAverageData & data = m_samplers[ name ]; //FIXME: why the heck does it make a copy???
    MovingAverageData * data = nullptr;
    if( m_samplers.find( name ) == m_samplers.end() )
    {
        data = new MovingAverageData( m_windowSize );
        m_samplers[ name ] = data;
        m_sectionsNames.push_back( name );
    }

    data = m_samplers[ name ];
    bool minFlag = false;
    bool maxFlag = false;
    data->AddNextSample( sample, &minFlag, &maxFlag );

    m_minFlags[ name ] = minFlag;
    m_maxFlags[ name ] = maxFlag;
}

} //bv

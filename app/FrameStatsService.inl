#include <cassert>


namespace bv
{

// *********************************
//
inline void MovingAverageData::AddNextSample        ( const FrameStatsSample & sample )
{
    {
        const FrameStatsSample & last = samples.front();

        accumDuration += sample.duration - last.duration;
        accumDurationSquares += sample.duration * sample.duration - last.duration * last.duration;

        if ( sample.duration < minDuration )
        {
            minDuration = sample.duration;
            minDurationFrame = sample.frame;
        }

        if ( sample.duration > maxDuration )
        {
            maxDuration = sample.duration;
            maxDurationFrame = sample.frame;
        }
    }

    samples.pop_front();
    samples.push_back( sample );
}

// *********************************
//
inline void    FrameStatsCalculator::StartSection   ( const char * name )
{
    FrameStatsSample & sample = m_stateBuffer[ name ];

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
        MovingAverageData * data = new MovingAverageData( m_windowSize );
        m_samplers[ name ] = data;
        m_sectionsNames.push_back( name );
    }

    data = m_samplers[ name ];
    data->AddNextSample( sample );
}

} //bv

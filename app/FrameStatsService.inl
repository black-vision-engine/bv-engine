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

    samples.pop();
    samples.push( sample );
}

// *********************************
//
inline void    FrameStatsCalculator::StartSection   ( const char * name, unsigned int frame )
{
    FrameStatsSample & sample = m_stateBuffer[ name ];

    sample.frame = frame;
    m_timer.Timestamp( &sample.startTime );
    sample.duration = -1.0;
}

// *********************************
//
inline void    FrameStatsCalculator::StopSection    ( const char * name, unsigned int frame )
{
    FrameStatsSample & sample = m_stateBuffer[ name ];
    assert( sample.duration == -1.0 );
    sample.duration = m_timer.CurElapsed( sample.startTime );

    MovingAverageData & data = m_samplers[ name ];
    if( data.samples.size() == 0 )
    {
        data.Initialize( m_windowSize );
    }

    data.AddNextSample( sample );
}

} //bv

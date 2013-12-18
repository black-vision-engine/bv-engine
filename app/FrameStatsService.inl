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
    MovingAverageData & data = m_samplers[ name ];

    if( data.samples.size() == 0 )
    {
        data.Initialize( m_windowSize );
    }

    FrameStatsSample sample;
    sample.frame = frame;
    m_timer.Timestamp( &sample.startTime );
}

// *********************************
//
inline void    FrameStatsCalculator::StopSection    ( const char * name, unsigned int frame )
{
    MovingAverageData & data = m_samplers[ name ];
    assert( data.samples.size() == m_windowSize );

    //FIXME: implement
}

} //bv

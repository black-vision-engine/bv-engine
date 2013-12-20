#include "FrameStatsService.h"

#include  <cassert>


namespace bv
{

// *********************************
//
MovingAverageData::MovingAverageData        ( unsigned int numSamples )
{
    Initialize( numSamples );
}

// *********************************
//
void MovingAverageData::Initialize          ( unsigned int numSamples )
{
    assert( samples.size() == 0 );

    ResetAccumStats();

    FrameStatsSample sample;
    sample.duration = 0.0;
    sample.frame = 0;

    for( unsigned int i = 0; i < numSamples; ++i )
    {
        samples.push( sample );
    } 
}

// *********************************
//
void MovingAverageData::ResetAccumStats     ()
{
    minDuration             = 10000000.0;
    minDurationFrame        = 0xFFFFFFFF;

    maxDuration             = 0.0;
    maxDurationFrame        = 0xFFFFFFFF;

    accumDuration           = 0.0;
    accumDurationSquares    = 0.0;
}

// *********************************
//
void MovingAverageData::RecalculateStats    ()
{
    ResetAccumStats();

    double numEltsDouble = (double) samples.size();

    std::list< FrameStatsSample > tmpSamples;

    while( !samples.empty() )
    {
        auto sample = samples.front();
        samples.pop();
        tmpSamples.push_back( sample );
    }

    for( auto sample : tmpSamples )
    {
        AccumulateSample( sample );

        samples.push( sample );
    }
}

// *********************************
//
void    MovingAverageData::AccumulateSample    ( const FrameStatsSample & sample )
{
    accumDuration += sample.duration;
    accumDurationSquares += sample.duration * sample.duration;

    if( sample.duration < minDuration )
    {
        minDuration = sample.duration;
        minDurationFrame = sample.frame;
    }

    if( sample.duration > maxDuration )
    {
        maxDuration = sample.duration;
        maxDurationFrame = sample.frame;
    }
}

// *********************************
//
double  MovingAverageData::ExpectedValue    () const
{
    return accumDuration / (double) samples.size();
}

// *********************************
//
double  MovingAverageData::Variance         () const
{
    double EX2 = ExpectedValue();
    EX2 *= EX2;

    return accumDurationSquares / (double) samples.size() - EX2;
}

// *********************************
//
double  MovingAverageData::MinVal           ( unsigned int * frame ) const
{
    if( frame )
        *frame = minDurationFrame;

    return minDuration;
}

// *********************************
//
double  MovingAverageData::MaxVal           ( unsigned int * frame ) const
{
    if( frame )
        *frame = maxDurationFrame;

    return maxDuration;
}

// *********************************
//
FrameStatsCalculator::FrameStatsCalculator  ( unsigned int movingAverageWindowSize )
    : m_windowSize( movingAverageWindowSize )
    , m_frame( 0 )
{
    assert( m_windowSize > 0 );
}

// *********************************
//
FrameStatsCalculator::~FrameStatsCalculator ()
{
    for( auto it : m_samplers )
    {
        delete it.second;
    }
}

// *********************************
//
unsigned int FrameStatsCalculator::CurFrame         () const
{
    return m_frame;
}

// *********************************
//
unsigned int FrameStatsCalculator:: WindowSize      () const
{
    return m_windowSize;
}

// *********************************
//
unsigned int FrameStatsCalculator::NextFrame        ()
{
    return m_frame++;
}

// *********************************
//
void    FrameStatsCalculator::ResetTimer            ()
{
    m_timer.Reinitialize();
}

// *********************************
//
void    FrameStatsCalculator::RecalculateStats        ()
{
    for ( auto it : m_samplers )
    {
        const char * name = it.first;
        it.second->RecalculateStats();
    }
}

// *********************************
//
double  FrameStatsCalculator::ExpectedValue           ( const char * name ) const
{
    assert( m_samplers.find( name ) != m_samplers.end() );

    const auto it = m_samplers.find( name );
    return it->second->ExpectedValue();
}

// *********************************
//
double  FrameStatsCalculator::Variance                ( const char * name ) const
{
    assert( m_samplers.find( name ) != m_samplers.end() );

    const auto it = m_samplers.find( name );
    return it->second->Variance();
}

// *********************************
//
double  FrameStatsCalculator::MinVal                  ( const char * name, unsigned int * frame ) const
{
    assert( m_samplers.find( name ) != m_samplers.end() );

    const auto it = m_samplers.find( name );
    return it->second->MinVal( frame );
}

// *********************************
//
double  FrameStatsCalculator::MaxVal                  ( const char * name, unsigned int * frame ) const
{
    assert( m_samplers.find( name ) != m_samplers.end() );

    const auto it = m_samplers.find( name );
    return it->second->MaxVal( frame );
}

// *********************************
//
FrameStatsSample    FrameStatsCalculator::RecentSample( const char * name ) const
{
    assert( m_samplers.find( name ) != m_samplers.end() );

    const auto it = m_samplers.find( name );
    return it->second->samples.front();
}

} //bv
